#include "gui/GuiChat.h"
#include <cuchar>

GuiChat::GuiChat(mc::protocol::packets::PacketDispatcher *dispatcher, mc::core::Client *client)
        : mc::protocol::packets::PacketHandler(dispatcher), m_Client(client)
{
    using namespace mc::protocol;

    m_Client->RegisterListener(this);
    dispatcher->RegisterHandler(State::Play, play::Chat, this);
    dispatcher->RegisterHandler(State::Play, play::PluginMessage, this);
}

GuiChat::~GuiChat() {
    GetDispatcher()->UnregisterHandler(this);
    m_Client->UnregisterListener(this);
}

void GuiChat::Render(bool* chatVisible) {
    ImGuiManager::GetInstance()->RegisterCallback("Chat", [&]() {

        ImGui::Begin("Chat",chatVisible);

            if(ImGui::Button("Send Message")) SendMessage(Input());

            ImGui::BeginChild("Scrolling");
                for (auto message : m_messages) {
                    ImGui::Text("%s", message.c_str());
                }
            ImGui::EndChild();

        ImGui::End();

    });
}

std::string GuiChat::Input(){
    /* TODO: This Keyboard blocks the main thread, causing the client to disconnect.
             We need a custom Keyboard that works asynchronously to prevent this behavour.
    */
    SwkbdState swkbd;

    // Minecrafts Chat & Chat Packets allows messages up to 256 bytes
    char inputText[256] = {0};

    swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 2, -1);
    swkbdInputText(&swkbd, inputText, sizeof(inputText));

    return inputText;
}

void GuiChat::SendMessage(std::string message){
    if(!message.empty()){
        mc::protocol::packets::out::ChatPacket packet(message);
        m_Client->GetConnection()->SendPacket(&packet);
    }
}

void GuiChat::HandlePacket(mc::protocol::packets::in::ChatPacket* packet) {
    std::string message = mc::util::ParseChatNode(packet->GetChatData());
    message = mc::util::StripChatMessage(message);
    if (!message.empty()) m_messages.push_back(message);
}

void GuiChat::OnTick() {

}