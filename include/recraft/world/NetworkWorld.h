#pragma once

#include "CT_World.h"
#include "Client.h"
#include "mclib/protocol/packets/PacketHandler.h"

class NetworkWorld : public mc::protocol::packets::PacketHandler, public mc::core::ClientListener {
public:
    NetworkWorld(World* world, mc::protocol::packets::PacketDispatcher* dispatcher);
    ~NetworkWorld();

    void HandlePacket(mc::protocol::packets::in::ChunkDataPacket* packet) override;
    void HandlePacket(mc::protocol::packets::in::UnloadChunkPacket* packet) override;
    void HandlePacket(mc::protocol::packets::in::MultiBlockChangePacket* packet) override;
    void HandlePacket(mc::protocol::packets::in::BlockChangePacket* packet) override;
    void HandlePacket(mc::protocol::packets::in::ExplosionPacket* packet) override;
    void HandlePacket(mc::protocol::packets::in::UpdateBlockEntityPacket* packet) override;
    void HandlePacket(mc::protocol::packets::in::RespawnPacket* packet) override;

private:
    World* m_world = nullptr;

    void OnTick() override;
};