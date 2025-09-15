#include "ServerConnect.hpp"
#include "../ServerList/ServerList.hpp"   // for joinServer

ServerConnect::ServerConnect(ServerList* serverList, const ServerNBTStorage& server)
    : serverList(serverList), server(server) {}

void ServerConnect::drawScreen() const{
    consoleClear();
    std::cout << "\n=== Server Connect Screen ===\n";
    std::cout << "Server Name: " << server.name << "\n";
    std::cout << "Server Host: " << server.host << "\n";
    std::cout << "MOTD: " << (server.motd.empty() ? "(none)" : server.motd) << "\n";
    std::cout << "Players: " << (server.playerCount.empty() ? "???" : server.playerCount) << "\n";
    std::cout << "=============================\n";
    std::cout << "Press A to connect, B to go back.\n";
}

void ServerConnect::updateControls(u32 kDown) {
    if (kDown & KEY_A) {
        std::cout << "Attempting to connect to " << server.host << "...\n";

    }
    if (kDown & KEY_B) {
        Screen::currentScreen = serverList;
        Screen::currentScreen->drawScreen();
        // no UI handling yet — just log
    }
}