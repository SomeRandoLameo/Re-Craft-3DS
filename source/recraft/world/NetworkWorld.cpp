#include "world/NetworkWorld.h"
#include "gui/DebugUI.h"

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

//TODO: We still need some form of block registry to propperly read chunk block data and match it with craftus blocks.
void NetworkWorld::HandlePacket(mc::protocol::packets::in::ChunkDataPacket* packet) {
    // for every chunk in the column
    //for (s16 i = 0; i < CHUNKS_PER_COLUMN; ++i) {
        // for each block in the chunk
        for (s32 x = 0; x < 16; ++x) {
            for (s32 y = 0; y < 16; ++y) {
                for (s32 z = 0; z < 16; ++z) {
                    auto position = mc::Vector3i(x,y,z);
                    //if(block->GetType() != 0) DebugUI::Log("FOUND: %d", block->GetType());

                    m_world->SetBlock(position, packet->GetChunkColumn()->GetBlock(position)->GetType());
                }
            }
        }
    //}

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
