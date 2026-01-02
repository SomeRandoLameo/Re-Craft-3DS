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

    for (s32 x = 0; x < 16; ++x) {
        for (s32 z = 0; z < 16; ++z) {
            for (s32 y = 0; y < 16; ++y) {

                mc::Vector3i localPos(x, y, z);

                ChunkColumnPtr column = m_world->GetChunkColumn(0, 0);
                if (!column) continue;

                ChunkPtr chunk = column->GetChunk(y);
                if (!chunk) continue;

                auto sourceBlock = sourceColumn->GetBlock(localPos);
                chunk->blocks[x][y][z] = MCBridge::MCLibBlockToCTBlock(sourceBlock->GetType());
                column->SetMetadata(localPos, 0);
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
