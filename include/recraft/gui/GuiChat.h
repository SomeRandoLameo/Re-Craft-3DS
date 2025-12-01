#pragma once
#include "gui/ImGuiManager.h"
#include "mclib/protocol/ProtocolState.h"
#include "mclib/protocol/packets/PacketDispatcher.h"
#include "Utility.h"


class GuiChat : public mc::protocol::packets::PacketHandler, public mc::core::ClientListener {
public:
    GuiChat(mc::protocol::packets::PacketDispatcher* dispatcher, mc::core::Client* client);
    ~GuiChat();

    void HandlePacket(mc::protocol::packets::in::ChatPacket* packet) override;
    void Render(bool* chatVisible);

private:
    mc::core::Client* m_Client;
    std::vector<std::string> m_messages;

    std::string Input();

    void SendMessage(std::string message);

    void OnTick() override;
};
