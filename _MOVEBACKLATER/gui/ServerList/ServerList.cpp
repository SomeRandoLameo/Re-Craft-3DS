#include "ServerList.hpp"
#include "ServerPollingThread.hpp"

#include "../../nbt/NBTTagCompound.hpp"
#include "../../nbt/NBTTagList.hpp"
#include "../../nbt/CompressedStreamTools.hpp"
#include "../../packet/Packet.hpp"
#include "../../utils/keyboard/keyboard.hpp"

#include "../ServerConnect/ServerConnect.hpp"

#include "../Screen.hpp"

#include <3ds.h>
#include <iostream>
#include <sstream>
#include <fcntl.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdexcept>

int parseIntWithDefault(const std::string& str, int defaultValue) {
    try { return std::stoi(str); }
    catch (...) { return defaultValue; }
}

ServerList::ServerList() {
    //Socket::initSOC();
    pollingThread.reset(new ServerPollingThread(this));
}

ServerList::~ServerList() {
    if (pollingThread) {
        pollingThread->stop();
        pollingThread->join();
    }
    //Socket::shutdownSOC();
}

void ServerList::initGui() {
    this->load();
    
    // Start the polling thread
    if (pollingThread) {
        pollingThread->start();
    }

    this->drawScreen();
}

void ServerList::load() {
    try {
        CompressedStreamTools tools;
        rootTag = tools.readMapFromFile("servers.dat");
        if (!rootTag) return;

        NBTTagList* serverTagList = rootTag->getTagList("servers");
        if (!serverTagList) return;

        servers.clear();

        for (size_t i = 0; i < serverTagList->tagCount(); ++i) {
            NBTBase* base = serverTagList->tagAt(i);
            if (base->getType() == 10) { // Compound
                NBTTagCompound* compound = static_cast<NBTTagCompound*>(base);
                servers.push_back(ServerNBTStorage::createServerNBTStorage(*compound));
            }
        }

    } catch (...) {
        std::cout << "Failed to load server list.\n";
    }
}

void ServerList::save() {
    try {
        std::unique_ptr<NBTTagList> serverTagListBase(new NBTTagList());
        NBTTagList* serverTagList = serverTagListBase.get();

        for (const auto& server : servers) {
            serverTagList->addTag(std::unique_ptr<NBTTagCompound>(new NBTTagCompound(server.getCompoundTag())));
        }

        NBTTagCompound root;
        root.setTag("servers", std::move(serverTagListBase));
        CompressedStreamTools::saveMapToFileWithBackup(&root, "servers.dat");

    } catch (...) {
        std::cout << "Failed to save server list.\n";
    }
}

void ServerList::addServer(const std::string& name, const std::string& host) {
    servers.push_back(ServerNBTStorage(name, host));
    save();
}

void ServerList::deleteSelectedServer() {
    if (servers.empty()) return;
    servers.erase(servers.begin() + selectedServer);
    if (selectedServer >= servers.size() && selectedServer > 0)
        selectedServer--;
    save();
}



int utf16be_to_utf8(const uint8_t *in, int inLen, char *out, int outSize) {
    int i = 0, o = 0;
    while (i + 1 < inLen && o + 4 < outSize) {
        uint16_t code = (in[i] << 8) | in[i+1];
        i += 2;

        if (code < 0x80) {
            out[o++] = static_cast<char>(code);
        } else if (code < 0x800) {
            out[o++] = static_cast<char>(0xC0 | (code >> 6));
            out[o++] = static_cast<char>(0x80 | (code & 0x3F));
        } else {
            out[o++] = static_cast<char>(0xE0 | (code >> 12));
            out[o++] = static_cast<char>(0x80 | ((code >> 6) & 0x3F));
            out[o++] = static_cast<char>(0x80 | (code & 0x3F));
        }
    }
    out[o] = '\0';
    return o;
}
//TODO: Remove once DataInputStream/DataOutputStream has been implemented here
std::string readString(std::istream& in, int maxLen) {
  
    uint8_t lengthBytes[2];
    in.read(reinterpret_cast<char*>(lengthBytes), 2);
    if (in.gcount() != 2) {
        throw std::runtime_error("Failed to read string length");
    }

    int strLen = (lengthBytes[0] << 8) | lengthBytes[1];
    if (strLen > maxLen || strLen < 0) {
        throw std::runtime_error("Invalid string length");
    }

    // --- Read UTF-16BE data (2 bytes per char) ---
    std::vector<uint8_t> utf16Data(strLen * 2);
    in.read(reinterpret_cast<char*>(utf16Data.data()), strLen * 2);
    if (in.gcount() != strLen * 2) {
        throw std::runtime_error("Failed to read string data");
    }

    // --- Convert UTF-16BE → UTF-8 ---
    char utf8Buffer[1024];
    utf16be_to_utf8(utf16Data.data(), strLen * 2, utf8Buffer, sizeof(utf8Buffer));

    return std::string(utf8Buffer);
}

ServerInfo ServerList::pollServer(ServerNBTStorage& storage) {

    //TODO: Refactor using dataIO
    std::string host = storage.host;
    if (host.empty()) host = "192.168.2.101";

    std::vector<std::string> hostParts;
    if (host.front() == '[') {
        size_t endIndex = host.find(']');
        if (endIndex != std::string::npos) {
            std::string address = host.substr(1, endIndex - 1);
            std::string portPart = host.substr(endIndex + 1);
            portPart.erase(0, portPart.find_first_not_of(" \t"));
            portPart.erase(portPart.find_last_not_of(" \t") + 1);
            if (!portPart.empty() && portPart.front() == ':' && portPart.length() > 1)
                portPart = portPart.substr(1);
            hostParts = {address, portPart};
        }
    } else {
        size_t colonPos = host.find(':');
        if (colonPos != std::string::npos) {
            hostParts.push_back(host.substr(0, colonPos));
            hostParts.push_back(host.substr(colonPos + 1));
        } else hostParts.push_back(host);
    }

   
    ServerInfo result;
    if (hostParts.size() > 2) hostParts = {host};
    const std::string& address = hostParts[0];
    int port = (hostParts.size() > 1) ? parseIntWithDefault(hostParts[1], 25565) : 25565;

    int sock = -1;
    try {
        sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
        if (sock < 0) throw std::runtime_error("Failed to create socket");

        struct sockaddr_in server{};
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
        struct timeval tv{3, 0};
        ret = select(sock + 1, nullptr, &wfds, nullptr, &tv);
        if (ret <= 0 || !FD_ISSET(sock, &wfds)) throw std::runtime_error("Connection timeout");

        fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) & ~O_NONBLOCK);

        uint8_t handshake = 254;
        if (send(sock, &handshake, sizeof(handshake), 0) < 0) throw std::runtime_error("Failed to send handshake");

        uint8_t response_byte;
        if (recv(sock, &response_byte, 1, 0) != 1 || response_byte != 255)
            throw std::runtime_error("Bad message");

        char buf[512];
        int len = recv(sock, buf, sizeof(buf), 0);
        if (len <= 0) throw std::runtime_error("Failed to read response string");

        std::istringstream in(std::string(buf, len), std::ios::binary);
        std::string response = readString(in, 256);

        const char section_sign = static_cast<char>(0xa7);
        std::vector<std::string> responseParts;
        std::string currentPart;
        for (char c : response) {
            if (c == section_sign) {
                if (!currentPart.empty()) { responseParts.push_back(currentPart); currentPart.clear(); }
            } else currentPart += c;
        }
        if (!currentPart.empty()) responseParts.push_back(currentPart);

        result.motd = responseParts.empty() ? "" : responseParts[0];
        result.onlinePlayers = (responseParts.size() > 1) ? std::stoi(responseParts[1]) : -1;
        result.maxPlayers = (responseParts.size() > 2) ? std::stoi(responseParts[2]) : -1;
        result.playerCount = (result.onlinePlayers >= 0 && result.maxPlayers > 0) ?
                             std::to_string(result.onlinePlayers) + " / " + std::to_string(result.maxPlayers) : "???";
        result.success = true;

    } catch (...) { result.success = false; }

    if (sock >= 0) close(sock);

    this->drawScreen();
    return result;
}

void ServerList::pollAllAsync() {
    if (!pollingThread || servers.empty()) {
        return;
    }
    
    isPolling = true;
    pollingThread->pollAllServersAsync();
}

void ServerList::updateAsyncPolls() {
    if (!pollingThread) {
        return;
    }
    
    // Process any completed polls
    pollingThread->processCompletedPolls();
    
    // Check if polling is complete
    if (isPolling && pollingThread->getPendingCount() == 0) {
        isPolling = false;
        
        this->drawScreen();
    }

   
}

size_t ServerList::getPendingPollCount() const {
    if (!pollingThread) {
        return 0;
    }
    return pollingThread->getPendingCount();
}

void ServerList::drawScreen() const {
    consoleClear();
    std::cout << "Re:Craft 3DS\n";
    std::cout << "A:Add  B:Del  X:Poll  Y:Reload \nSELECT:Connect  START:Exit\n";
    
    // Show polling status
    if (isPolling) {
        size_t pending = getPendingPollCount();
        std::cout << "Polling... (" << pending << " remaining)\n";
    }
    std::cout << "\n";

    if (servers.empty()) {
        std::cout << "(No servers added)\n";
        return;
    }

    for (size_t i = 0; i < servers.size(); ++i) {
        std::string marker = (i == selectedServer) ? ">" : " ";
        std::cout << marker << " " << servers[i].name << " (" << servers[i].host << ")\n";
        std::cout << "   " << (servers[i].motd.empty() ? "" : servers[i].motd) << "\n";
        std::cout << "   " << (servers[i].playerCount.empty() ? "???": servers[i].playerCount) << "\n\n";
    }
}

void ServerList::updateControls(u32 kDown) {
    if (kDown & KEY_UP) { 
        this->moveSelectionUp(); 
        this->drawScreen(); 
    }

    if (kDown & KEY_DOWN) { 
        this->moveSelectionDown(); 
        this->drawScreen(); 
    }

    if (kDown & KEY_A) {
        std::string name = getKeyboardInput("Enter Server Name");
        std::string host = getKeyboardInput("Enter Server IP");
        this->addServer(name, host);
        this->drawScreen();
    }

    if (kDown & KEY_B) { 
        this->deleteSelectedServer(); 
        this->drawScreen(); 
    }
    
    if (kDown & KEY_X) { 
        this->pollAllAsync();
        this->drawScreen(); 
    }
    
    
    if (kDown & KEY_Y) { 
        this->load(); 
        this->pollAllAsync();
        this->drawScreen(); 
    }

    if (kDown & KEY_SELECT) {
        if (!servers.empty() && selectedServer < servers.size()) {
            currentScreen = new ServerConnect(this, servers[selectedServer]);
            currentScreen->drawScreen();
        }
    }
    
    this->updateAsyncPolls();
    
    static bool lastPollingState = false;
    if (isPolling != lastPollingState) {
        lastPollingState = isPolling;
        this->drawScreen();
    }
}

void ServerList::moveSelectionUp() {
    if (selectedServer > 0) selectedServer--;
}

void ServerList::moveSelectionDown() {
    if (selectedServer + 1 < servers.size()) selectedServer++;
}