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

//TODO: Load a full chunk (16x16x16) with dirt blocks locally for testing
void NetworkWorld::Test() {

    ChunkColumnPtr column = m_world->GetChunkColumn(0 , 0);     // Get ChunkColumn at (0,0) from World
    ChunkPtr chunk = column->GetChunk(0);                       // Get the bottom chunk (y=0) (0 - 16 * 16 => MAX 256 blocks high)

    for (s32 x = 0; x < Chunk::Size; ++x) {                     // 0-15 for x
        for (s32 z = 0; z < Chunk::Size; ++z) {                 // 0-15 for Y
            for (s32 y = 0; y < Chunk::Size; ++y) {             // 0-15 for Z

                chunk->SetBlock(x,y,z, Block::Dirt);            // Set Block at (x,y,z) to Dirt
                chunk->metadataLight[x][y][z] = 0;              // Set metadata light to 0 (no light) (this doesnt really matter)
            }
        }
    }
}


//TODO: We still need some form of block registry to propperly read chunk block data and match it with craftus blocks. This is temporarly redirected through the MCBridge.
void NetworkWorld::HandlePacket(mc::protocol::packets::in::ChunkDataPacket* packet) {
    /*
    auto sourceColumn = packet->GetChunkColumn();
    auto meta = sourceColumn->GetMetadata();
    int chunkX = meta.x;
    int chunkZ = meta.z;

    ChunkColumnPtr column = m_world->GetChunkColumn(chunkX, chunkZ);

    // If not in cache, we need to allocate or find a free chunk
    if (!column) {
        for (int i = 0; i < World::ChunkPoolSize; i++) {
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
     */
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
