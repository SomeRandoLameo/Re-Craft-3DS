#include <3ds.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <array>
#include <malloc.h>
#include <cstdio>
#include <cstring>
#include <cerrno>
#include <fcntl.h>
#include <arpa/inet.h>
#include <vector>

#include "utils/socket/Socket.hpp"

// NBT headers
#include "nbt/NBTBase.hpp"
#include "nbt/NBTTagCompound.hpp"
#include "nbt/NBTTagList.hpp"
#include "nbt/CompressedStreamTools.hpp"

// Server storage class
#include "serverStorage/ServerNBTStorage.hpp"

// Structure to hold server info
struct ServerInfo {
    std::string motd;
    int onlinePlayers = -1;
    int maxPlayers = -1;
    std::string playerCount;
    bool success = false;
};

NBTTagCompound* rootTag = nullptr;
std::vector<ServerNBTStorage> serverList;
size_t selectedServer = 0;

void loadServerList() {
    try {
        CompressedStreamTools tools;
        rootTag = tools.readMapFromFile("servers.dat");
        if (!rootTag) return;

        NBTTagList* serverTagList = rootTag->getTagList("servers");
        if (!serverTagList) return;

        serverList.clear();
        for (size_t i = 0; i < serverTagList->tagCount(); ++i) {
            NBTBase* base = serverTagList->tagAt(i);
            if (base->getType() == 10) { // Compound tag
                NBTTagCompound* compound = static_cast<NBTTagCompound*>(base);
                serverList.push_back(ServerNBTStorage::createServerNBTStorage(*compound));
            }
        }

    } catch (...) {
        std::cout << "Failed to load server list.\n";
    }
}

void saveServerList() {
    try {
        std::unique_ptr<NBTTagList> serverTagList(new NBTTagList());

        for (const auto& server : serverList) {
            serverTagList->addTag(std::unique_ptr<NBTBase>(
                new NBTTagCompound(server.getCompoundTag())
            ));
        }

        NBTTagCompound root;
        root.setTag("servers", std::move(serverTagList));

        CompressedStreamTools::saveMapToFileWithBackup(&root, "servers.dat");

    } catch (...) {
        std::cout << "Failed to save server list.\n";
    }
}

ServerInfo pollServer(std::string host) {
    Socket::initSOC();

    if (host.empty()) host = "192.168.2.101";

    ServerInfo result;

    std::vector<std::string> hostParts = Socket::parseHost(host);
    if (hostParts.size() > 2) hostParts = {host};
    const std::string& address = hostParts[0];
    int port = (hostParts.size() > 1) ? Socket::parseIntWithDefault(hostParts[1], 25565) : 25565;

    int sock = -1;

    try {
        sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
        if (sock < 0) throw std::runtime_error("Failed to create socket");

        struct sockaddr_in server;
        std::memset(&server, 0, sizeof(server));
        server.sin_family = AF_INET;
        server.sin_port = htons(port);

        if (inet_pton(AF_INET, address.c_str(), &server.sin_addr) <= 0)
            throw std::runtime_error("Invalid address");

        fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) | O_NONBLOCK);
        int ret = connect(sock, reinterpret_cast<struct sockaddr*>(&server), sizeof(server));
        if (ret < 0 && errno != EINPROGRESS) throw std::runtime_error("Connection failed");

        fd_set wfds;
        FD_ZERO(&wfds);
        FD_SET(sock, &wfds);
        struct timeval tv = {3, 0}; // 3s timeout
        ret = select(sock + 1, nullptr, &wfds, nullptr, &tv);
        if (ret <= 0 || !FD_ISSET(sock, &wfds)) throw std::runtime_error("Connection timeout");

        fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) & ~O_NONBLOCK);

        uint8_t handshake = 254;
        if (send(sock, &handshake, sizeof(handshake), 0) < 0)
            throw std::runtime_error("Failed to send handshake");

        uint8_t response_byte;
        if (recv(sock, &response_byte, 1, 0) != 1 || response_byte != 255)
            throw std::runtime_error("Bad message");

        std::string response = Socket::readString(sock, 256);
        const char section_sign = static_cast<char>(0xa7);

        std::vector<std::string> responseParts;
        std::string currentPart;
        for (char c : response) {
            if (c == section_sign) {
                if (!currentPart.empty()) {
                    responseParts.push_back(currentPart);
                    currentPart.clear();
                }
            } else {
                currentPart += c;
            }
        }
        if (!currentPart.empty()) responseParts.push_back(currentPart);

        result.motd = responseParts.empty() ? "" : responseParts[0];
        result.onlinePlayers = (responseParts.size() > 1) ? std::stoi(responseParts[1]) : -1;
        result.maxPlayers = (responseParts.size() > 2) ? std::stoi(responseParts[2]) : -1;

        if (result.onlinePlayers >= 0 && result.maxPlayers > 0)
            result.playerCount = std::to_string(result.onlinePlayers) + " / " + std::to_string(result.maxPlayers);
        else
            result.playerCount = "???";

        result.success = true;

    } catch (...) {
        result.success = false;
    }

    if (sock >= 0) close(sock);

    return result;
}

std::string getKeyboardInput(std::string prompt) {
    SwkbdState swkbd;
    swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 2, -1);
    swkbdSetValidation(&swkbd, SWKBD_NOTEMPTY, 0, 0);
    swkbdSetHintText(&swkbd, prompt.c_str());

    std::array<char, 256> buffer{};
    swkbdInputText(&swkbd, buffer.data(), buffer.size());
    
    return std::string(buffer.data());
}

void drawMainScreen() {
    consoleClear();
    std::cout << "Re:Craft 3DS\n";
    std::cout << "A:Add  B:Del  X:Poll  Y:Reload  START:Exit\n\n";

    if (serverList.empty()) {
        std::cout << "(No servers added)\n";
    } else {
        for (size_t i = 0; i < serverList.size(); ++i) {
            std::string marker = (i == selectedServer) ? ">" : " ";
            std::cout << marker << " " << serverList[i].name << " (" << serverList[i].host << ")\n";
            
            // Display MOTD if available
            if (!serverList[i].motd.empty()) {
                std::cout << "   " << serverList[i].motd << "\n";
            }
            
            // Display player count if available
            if (!serverList[i].playerCount.empty() && serverList[i].playerCount != "???") {
                std::cout << "   " << serverList[i].playerCount << "\n";
            } else {
                std::cout << "   Can´t reach server\n";
            }
            std::cout << "\n"; // Extra line for spacing
        }
    }
}

void deleteSelectedServer() {
    if (serverList.empty()) return;
    serverList.erase(serverList.begin() + selectedServer);
    if (selectedServer >= serverList.size() && selectedServer > 0)
        selectedServer--;
    saveServerList();
}

void pollAllServers() {
    if (serverList.empty()) return;
    
    std::cout << "Polling all servers...\n";
    
    for (size_t i = 0; i < serverList.size(); ++i) {
        std::cout << "Polling " << serverList[i].name << "...\n";
        ServerInfo result = pollServer(serverList[i].host);
        
        if (result.success) {
            serverList[i].motd = result.motd;
            serverList[i].playerCount = result.playerCount;
        } else {
            serverList[i].motd = "";
            serverList[i].playerCount = "???";
        }
    }
}

int main(int argc, char **argv) {
    gfxInitDefault();
    consoleInit(GFX_TOP, nullptr);
    std::atexit(gfxExit);

    loadServerList();
    pollAllServers();
    drawMainScreen();

    while (aptMainLoop()) {
        gspWaitForVBlank();
        hidScanInput();
        u32 kDown = hidKeysDown();

       

        if (kDown & KEY_UP) {
            if (selectedServer > 0) selectedServer--;
            drawMainScreen();
        }
        if (kDown & KEY_DOWN) {
            if (selectedServer + 1 < serverList.size()) selectedServer++;
            drawMainScreen();
        }
        if (kDown & KEY_A) {
            std::string serverName = getKeyboardInput("Enter Server Name");
            std::string serverIP = getKeyboardInput("Enter Server IP");

            serverList.push_back(ServerNBTStorage(serverName, serverIP));
            saveServerList();
            drawMainScreen();
        }
        if (kDown & KEY_B) {
            deleteSelectedServer();
            drawMainScreen();
        }
        if (kDown & KEY_X) {
            pollAllServers();
            drawMainScreen();
        }
        if (kDown & KEY_Y) {
            loadServerList();
            if (selectedServer >= serverList.size()) selectedServer = 0;
            pollAllServers();
            drawMainScreen();
        }
    
        if (kDown & KEY_START) break;
    }

    return 0;
}