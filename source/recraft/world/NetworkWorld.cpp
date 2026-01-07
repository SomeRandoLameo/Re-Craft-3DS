#include "world/NetworkWorld.h"
#include "gui/DebugUI.h"
#include "mcbridge/MCBridge.h"

NetworkWorld::NetworkWorld(World* world, mc::protocol::packets::PacketDispatcher* dispatcher)
    : mc::protocol::packets::PacketHandler(dispatcher)
{
    this->m_world = world;

    dispatcher->RegisterHandler(mc::protocol::State::Play, mc::protocol::play::MultiBlockChange, this);
    dispatcher->RegisterHandler(mc::protocol::State::Play, mc::protocol::play::BlockChange, this);
    dispatcher->RegisterHandler(mc::protocol::State::Play, mc::protocol::play::ChunkData, this);
    dispatcher->RegisterHandler(mc::protocol::State::Play, mc::protocol::play::UnloadChunk, this);
    dispatcher->RegisterHandler(mc::protocol::State::Play, mc::protocol::play::Explosion, this);
    dispatcher->RegisterHandler(mc::protocol::State::Play, mc::protocol::play::UpdateBlockEntity, this);
    dispatcher->RegisterHandler(mc::protocol::State::Play, mc::protocol::play::Respawn, this);
}

NetworkWorld::~NetworkWorld() {
    GetDispatcher()->UnregisterHandler(this);
}

//TODO: We still need some form of block registry to propperly read chunk block data and match it with craftus blocks. This is temporarly redirected through the MCBridge.
void NetworkWorld::HandlePacket(mc::protocol::packets::in::ChunkDataPacket* packet) {
    auto sourceColumn = packet->GetChunkColumn();
    auto meta = sourceColumn->GetMetadata();
    int chunkX = meta.x;
    int chunkZ = meta.z;

    ChunkColumnPtr column = m_world->GetChunkColumn(chunkX, chunkZ);

    // If not in cache, we need to allocate or find a free chunk
    if (!column) {
        for (int i = 0; i < CHUNKPOOL_SIZE; i++) {
            if (m_world->m_chunkChunkPool[i].x == chunkX &&
                m_world->m_chunkChunkPool[i].z == chunkZ) {
                column = &m_world->m_chunkChunkPool[i];
                break;
            }
        }

        if (!column && !m_world->m_freeChunkColums.empty()) {
            column = m_world->m_freeChunkColums.back();
            m_world->m_freeChunkColums.pop_back();

            column->~ChunkColumn();
            new (column) ChunkColumn(chunkX, chunkZ);
            column->genProgress = ChunkGen_Empty;
            column->references++;
        }

        if (!column) {
            return;
        }
    }

    for (s32 sectionY = 0; sectionY < ChunkColumn::ChunksPerColumn; ++sectionY) {
        ChunkPtr chunk = column->GetChunk(sectionY);
        if (!chunk) continue;

        for (s32 x = 0; x < Chunk::Size; ++x) {
            for (s32 z = 0; z < Chunk::Size; ++z) {
                for (s32 y = 0; y < Chunk::Size; ++y) {
                    mc::Vector3i sourcePos(x, y, z);
                    auto sourceBlock = sourceColumn->GetBlock(sourcePos);

                    chunk->SetBlock(x,y,z, MCBridge::MCLibBlockToCTBlock(sourceBlock->GetType()));
                    chunk->metadataLight[x][y][z] = 0;
                }
            }
        }

        ++chunk->revision;
    }

    ++column->revision;
    column->genProgress = ChunkGen_Finished;
}

void NetworkWorld::HandlePacket(mc::protocol::packets::in::UnloadChunkPacket* packet) {
    //m_world->
}

void NetworkWorld::HandlePacket(mc::protocol::packets::in::MultiBlockChangePacket* packet) {}

void NetworkWorld::HandlePacket(mc::protocol::packets::in::BlockChangePacket* packet) {}

void NetworkWorld::HandlePacket(mc::protocol::packets::in::ExplosionPacket* packet) {}

void NetworkWorld::HandlePacket(mc::protocol::packets::in::UpdateBlockEntityPacket* packet) {}

void NetworkWorld::HandlePacket(mc::protocol::packets::in::RespawnPacket* packet) {}
void NetworkWorld::OnTick(){

}
