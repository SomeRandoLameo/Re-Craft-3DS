#include "T_World.h"

namespace terra {

    T_World::T_World(mc::protocol::packets::PacketDispatcher* dispatcher)
        : mc::protocol::packets::PacketHandler(dispatcher)
    {
        dispatcher->RegisterHandler(mc::protocol::State::Play, mc::protocol::play::MultiBlockChange, this);
        dispatcher->RegisterHandler(mc::protocol::State::Play, mc::protocol::play::BlockChange, this);
        dispatcher->RegisterHandler(mc::protocol::State::Play, mc::protocol::play::ChunkData, this);
        dispatcher->RegisterHandler(mc::protocol::State::Play, mc::protocol::play::UnloadChunk, this);
        dispatcher->RegisterHandler(mc::protocol::State::Play, mc::protocol::play::Explosion, this);
        dispatcher->RegisterHandler(mc::protocol::State::Play, mc::protocol::play::UpdateBlockEntity, this);
        dispatcher->RegisterHandler(mc::protocol::State::Play, mc::protocol::play::Respawn, this);
    }

    T_World::~T_World() {
        GetDispatcher()->UnregisterHandler(this);
    }

    bool T_World::SetBlock(mc::Vector3i position, u32 blockData) {
        ChunkColumnPtr chunk = GetChunk(position);
        if (!chunk) return false;

        mc::Vector3i relative(position);

        relative.x %= 16;
        relative.z %= 16;

        if (relative.x < 0)
            relative.x += 16;
        if (relative.z < 0)
            relative.z += 16;

        std::size_t index = (std::size_t)position.y / 16;
        if ((*chunk)[index] == nullptr) {
            ChunkPtr section = std::make_shared<T_Chunk>();

            (*chunk)[index] = section;
        }

        relative.y %= 16;
        (*chunk)[index]->SetBlock(relative, mc::block::BlockRegistry::GetInstance()->GetBlock(blockData));
        return true;
    }

    void T_World::HandlePacket(mc::protocol::packets::in::ExplosionPacket* packet) {
        mc::Vector3d position = packet->GetPosition();

        for (mc::Vector3s offset : packet->GetAffectedBlocks()) {
            mc::Vector3d absolute = position + ToVector3d(offset);

            mc::block::BlockPtr oldBlock = GetBlock(mc::ToVector3i(absolute));

            // Set all affected blocks to air
            SetBlock(ToVector3i(absolute), 0);

            mc::block::BlockPtr newBlock = mc::block::BlockRegistry::GetInstance()->GetBlock(0);
            NotifyListeners(&WorldListener::OnBlockChange, ToVector3i(absolute), newBlock, oldBlock);
        }
    }

    void T_World::HandlePacket(mc::protocol::packets::in::ChunkDataPacket* packet) {
        mc::world::ChunkColumnPtr lib_column = packet->GetChunkColumn();

        ChunkColumnPtr col = std::make_shared<ChunkColumn>(*lib_column);
        const ChunkColumnMetadata& meta = col->GetMetadata();
        ChunkCoord key(meta.x, meta.z);

        if (meta.continuous && meta.sectionmask == 0) {
            m_Chunks[key] = nullptr;
            return;
        }

        auto iter = m_Chunks.find(key);

        if (!meta.continuous) {
            // This isn't an entire column of chunks, so just update the existing chunk column with the provided chunks.
            for (s16 i = 0; i < ChunkColumn::ChunksPerColumn; ++i) {
                // The section mask says whether or not there is data in this chunk.
                if (meta.sectionmask & (1 << i)) {
                    (*iter->second)[i] = (*col)[i];
                }
            }
        } else {
            // This is an entire column of chunks, so just replace the entire column with the new one.
            m_Chunks[key] = col;
        }


        for (s32 i = 0; i < ChunkColumn::ChunksPerColumn; ++i) {
            ChunkPtr chunk = (*col)[i];

            NotifyListeners(&WorldListener::OnChunkLoad, chunk, meta, i);
        }
    }

    void T_World::HandlePacket(mc::protocol::packets::in::MultiBlockChangePacket* packet) {
        mc::Vector3i chunkStart(packet->GetChunkX() * 16, 0, packet->GetChunkZ() * 16);
        auto iter = m_Chunks.find(ChunkCoord(packet->GetChunkX(), packet->GetChunkZ()));
        if (iter == m_Chunks.end())
            return;

        ChunkColumnPtr chunk = iter->second;
        if (!chunk)
            return;

        const auto& changes = packet->GetBlockChanges();
        for (const auto& change : changes) {
            mc::Vector3i relative(change.x, change.y, change.z);

            std::size_t index = change.y / 16;
            mc::block::BlockPtr oldBlock = mc::block::BlockRegistry::GetInstance()->GetBlock(0);
            if ((*chunk)[index] == nullptr) {
                ChunkPtr section = std::make_shared<T_Chunk>();

                (*chunk)[index] = section;
            } else {
                oldBlock = chunk->GetBlock(relative);
            }

            mc::block::BlockPtr newBlock = mc::block::BlockRegistry::GetInstance()->GetBlock(change.blockData);

            mc::Vector3i blockChangePos = chunkStart + relative;

            relative.y %= 16;
            if (newBlock->GetType() != oldBlock->GetType()) {
                chunk->RemoveBlockEntity(chunkStart + relative);
                (*chunk)[index]->SetBlock(relative, newBlock);
                NotifyListeners(&WorldListener::OnBlockChange, blockChangePos, newBlock, oldBlock);
            }
        }
    }

    void T_World::HandlePacket(mc::protocol::packets::in::BlockChangePacket* packet) {
        mc::block::BlockPtr newBlock = mc::block::BlockRegistry::GetInstance()->GetBlock((u16)packet->GetBlockId());
        auto pos = packet->GetPosition();
        mc::block::BlockPtr oldBlock = GetBlock(pos);

        SetBlock(packet->GetPosition(), packet->GetBlockId());

        NotifyListeners(&WorldListener::OnBlockChange, packet->GetPosition(), newBlock, oldBlock);

        ChunkColumnPtr col = GetChunk(pos);
        if (col) {
            col->RemoveBlockEntity(packet->GetPosition());
        }
    }

    void T_World::HandlePacket(mc::protocol::packets::in::UpdateBlockEntityPacket* packet) {
        mc::Vector3i pos = packet->GetPosition();

        ChunkColumnPtr col = GetChunk(pos);

        if (!col) return;

        col->RemoveBlockEntity(pos);

        mc::block::BlockEntityPtr entity = packet->GetBlockEntity();
        if (entity)
            col->AddBlockEntity(entity);
    }

    void T_World::HandlePacket(mc::protocol::packets::in::UnloadChunkPacket* packet) {
        ChunkCoord coord(packet->GetChunkX(), packet->GetChunkZ());

        auto iter = m_Chunks.find(coord);

        if (iter == m_Chunks.end())
            return;

        ChunkColumnPtr chunk = iter->second;
        NotifyListeners(&WorldListener::OnChunkUnload, chunk);

        m_Chunks.erase(iter);
    }

    // Clear all chunks because the server will resend the chunks after this.
    void T_World::HandlePacket(mc::protocol::packets::in::RespawnPacket* packet) {
        for (auto entry : m_Chunks) {
            ChunkColumnPtr chunk = entry.second;

            NotifyListeners(&WorldListener::OnChunkUnload, chunk);
        }

        m_Chunks.clear();
    }

    ChunkColumnPtr T_World::GetChunk(const mc::Vector3i& pos) const {
        s32 chunk_x = (s32)std::floor(pos.x / 16.0);
        s32 chunk_z = (s32)std::floor(pos.z / 16.0);

        ChunkCoord key(chunk_x, chunk_z);

        auto iter = m_Chunks.find(key);

        if (iter == m_Chunks.end()) return nullptr;

        return iter->second;
    }

    mc::block::BlockPtr T_World::GetBlock(const mc::Vector3i& pos) const {
        ChunkColumnPtr col = GetChunk(pos);

        if (!col) return mc::block::BlockRegistry::GetInstance()->GetBlock(0);

        s64 chunk_x = pos.x % 16;
        s64 chunk_z = pos.z % 16;

        if (chunk_x < 0)
            chunk_x += 16;
        if (chunk_z < 0)
            chunk_z += 16;

        return col->GetBlock(mc::Vector3i(chunk_x, pos.y, chunk_z));
    }

    mc::block::BlockEntityPtr T_World::GetBlockEntity(mc::Vector3i pos) const {
        ChunkColumnPtr col = GetChunk(pos);

        if (!col) return nullptr;

        return col->GetBlockEntity(pos);
    }

    std::vector<mc::block::BlockEntityPtr> T_World::GetBlockEntities() const {
        std::vector<mc::block::BlockEntityPtr> blockEntities;

        for (auto iter = m_Chunks.begin(); iter != m_Chunks.end(); ++iter) {
            if (iter->second == nullptr) continue;
            std::vector<mc::block::BlockEntityPtr> chunkBlockEntities = iter->second->GetBlockEntities();
            if (chunkBlockEntities.empty()) continue;
            blockEntities.insert(blockEntities.end(), chunkBlockEntities.begin(), chunkBlockEntities.end());
        }

        return blockEntities;
    }

} // ns terra
