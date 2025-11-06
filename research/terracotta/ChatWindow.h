#ifndef TERRACOTTA_CHAT_WINDOW_H_
#define TERRACOTTA_CHAT_WINDOW_H_

#include <mclib/protocol/packets/PacketDispatcher.h>
#include <mclib/protocol/packets/PacketHandler.h>

#include <string>
#include <deque>

namespace terra {

class ChatWindow : public mc::protocol::packets::PacketHandler {
public:
    ChatWindow(mc::protocol::packets::PacketDispatcher* dispatcher, mc::core::Connection* connection);
    ~ChatWindow();

    void HandlePacket(mc::protocol::packets::in::ChatPacket* packet);
    void Render();

private:
    mc::core::Connection* m_Connection;
    char m_InputText[256];
    std::deque<std::string> m_ChatBuffer;
};

} // ns terra

#endif
