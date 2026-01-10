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
    for (s32 x = 0; x < Chunk::Size; ++x) {                     // 0-15 for x
        for (s32 z = 0; z < Chunk::Size; ++z) {                 // 0-15 for Y
            for (s32 y = 0; y < Chunk::Size; ++y) {             // 0-15 for Z
                m_world->SetBlock(mc::Vector3i(x,y,z), Block::Dirt);            // Set Block at (x,y,z) to Dirt
            }
        }
    }
}


//TODO: We still need some form of block registry to propperly read chunk block data and match it with craftus blocks. This is temporarly redirected through the MCBridge.
void NetworkWorld::HandlePacket(mc::protocol::packets::in::ChunkDataPacket* packet) {
    auto sourceColumn = packet->GetChunkColumn();
    auto meta = sourceColumn->GetMetadata();
    int chunkX = meta.x;
    int chunkZ = meta.z;

    // Calculate the base world position for this chunk column
    int baseWorldX = chunkX * Chunk::Size;
    int baseWorldZ = chunkZ * Chunk::Size;

    for (s32 sectionY = 0; sectionY < ChunkColumn::ChunksPerColumn; ++sectionY) {
        // Calculate base Y position for this chunk section
        int baseWorldY = sectionY * Chunk::Size;

        for (s32 x = 0; x < Chunk::Size; ++x) {
            for (s32 z = 0; z < Chunk::Size; ++z) {
                for (s32 y = 0; y < Chunk::Size; ++y) {

                    mc::Vector3i columnRelativePos(x, baseWorldY + y, z);
                    auto sourceBlock = sourceColumn->GetBlock(columnRelativePos);

                    mc::Vector3i worldPos(
                        baseWorldX + x,
                        baseWorldY + y,
                        baseWorldZ + z
                    );

                    m_world->SetBlock(worldPos, MCBridge::MCLibBlockToCTBlock(sourceBlock->GetType()));
                }
            }
        }
    }
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
