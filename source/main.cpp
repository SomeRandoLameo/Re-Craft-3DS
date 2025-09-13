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
#include <malloc.h>


#include "utils/socket/Socket.hpp"

void pollServer(std::string host) {
    // Initialize SOC subsystem
    Socket::initSOC();

    // fallback if user cancels
    if (host.empty()) {
        std::cout << "Host was empty, Polling default\n";
        host = "192.168.2.101"; // default/fallback IP
    }

    std::cout << "Connecting to " << host << "...\n";

    ServerInfo result;
    
    // Parse host similar to Kotlin version
    std::vector<std::string> hostParts = Socket::parseHost(host);
    
    if (hostParts.size() > 2) {
        hostParts = {host};
    }
    
    const std::string& address = hostParts[0];
    int port = (hostParts.size() > 1) ? Socket::parseIntWithDefault(hostParts[1], 25565) : 25565;
    
    int sock = -1;
    
    try {
        // Create socket
        sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
        if (sock < 0) {
            throw std::runtime_error("Failed to create socket");
        }
        
        // Note: 3DS doesn't support SO_RCVTIMEO, SO_SNDTIMEO, TCP_NODELAY
        // So we skip: socket.soTimeout, socket.tcpNoDelay, socket.trafficClass
        

        struct sockaddr_in server;
        std::memset(&server, 0, sizeof(server));
        server.sin_family = AF_INET;
        server.sin_port = htons(port);
        
        if (inet_pton(AF_INET, address.c_str(), &server.sin_addr) <= 0) {
            throw std::runtime_error("Invalid address");
        }
        
        // Set non-blocking for connection timeout simulation
        fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) | O_NONBLOCK);
        
        int ret = connect(sock, reinterpret_cast<struct sockaddr*>(&server), sizeof(server));
        if (ret < 0 && errno != EINPROGRESS) {
            throw std::runtime_error("Connection failed");
        }
        
        // Wait for connection with timeout (simulating 3000ms timeout)
        fd_set wfds;
        FD_ZERO(&wfds);
        FD_SET(sock, &wfds);
        
        struct timeval tv = {3, 0}; // 3 seconds timeout
        ret = select(sock + 1, nullptr, &wfds, nullptr, &tv);
        if (ret <= 0 || !FD_ISSET(sock, &wfds)) {
            throw std::runtime_error("Connection timeout");
        }
        
        // Set back to blocking mode
        fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) & ~O_NONBLOCK);
        
        // Send handshake (equivalent to output.write(254))
        uint8_t handshake = 254;
        if (send(sock, &handshake, sizeof(handshake), 0) < 0) {
            throw std::runtime_error("Failed to send handshake");
        }
        
        // Read response (equivalent to input.read())
        uint8_t response_byte;
        if (recv(sock, &response_byte, 1, 0) != 1 || response_byte != 255) {
            throw std::runtime_error("Bad message");
        }
        
        // Read string response
        std::string response = Socket::readString(sock, 256);
        
        // Define section sign character
        const char section_sign = static_cast<char>(0xa7); // § character in Latin-1
        
        // Filter characters similar to Kotlin version
        /*
        for (char& c : response) {
            if (c != section_sign && !Socket::isAllowedChar(c)) {
                c = ' ';
            }
        }
        */
        
        // Split response by § character
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
        if (!currentPart.empty()) {
            responseParts.push_back(currentPart);
        }
        
        // Parse MOTD and player info
        std::string motd = responseParts.empty() ? "" : responseParts[0];
        int onlinePlayers = -1;
        int maxPlayers = -1;
        
        try {
            if (responseParts.size() > 1) {
                onlinePlayers = std::stoi(responseParts[1]);
            }
            if (responseParts.size() > 2) {
                maxPlayers = std::stoi(responseParts[2]);
            }
        } catch (...) {
            // Ignore parsing errors, similar to Kotlin version
        }
        
        // Set result values (matching Kotlin format)
        result.motd = motd;  // §7 in Latin-1
        result.onlinePlayers = onlinePlayers;
        result.maxPlayers = maxPlayers;
        
        if (onlinePlayers >= 0 && maxPlayers > 0) {
            result.playerCount = "" + std::to_string(onlinePlayers) + 
                               " / " + std::to_string(maxPlayers);
        } else {
            result.playerCount = "???"; 
        }
        
        result.success = true;
        
    } catch (...) {
        // Error handling - result.success remains false
    }
    
    // Cleanup (equivalent to finally block)
    if (sock >= 0) {
        close(sock);
    }
    
    if (result.success) {
        std::cout << "Connected successfully!\n";
        std::cout << "MOTD: " << result.motd << "\n";
        std::cout << "Players: " << result.playerCount << "\n";
    } else {
        std::cout << "Failed to connect to server\n";
    }

}

std::string getServerAddress() {
    SwkbdState swkbd;
    swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 2, -1);
    swkbdSetValidation(&swkbd, SWKBD_NOTEMPTY, 0, 0);
    swkbdSetHintText(&swkbd, "Enter Server IP");

    std::array<char, 256> buffer{};
    swkbdInputText(&swkbd, buffer.data(), buffer.size());
    
    return std::string(buffer.data());
}

// Fail and exit with message
void failExit(const char *message) {
    std::cout << message << "\n";
    std::cout << "Press B to exit\n";
    while (aptMainLoop()) {
        gspWaitForVBlank();
        hidScanInput();
        if (hidKeysDown() & KEY_B) std::exit(0);
    }
}

int main(int argc, char **argv) {
    // Initialize graphics and console
    gfxInitDefault();
    consoleInit(GFX_TOP, nullptr);
    std::atexit(gfxExit);
    
    std::cout << "Re:Craft 3DS\n";
    std::cout << "Press A to poll a server\n";
    std::cout << "Press START to exit.\n";
    
    while (aptMainLoop()) {
        gspWaitForVBlank();
        hidScanInput();
        
        u32 kDown = hidKeysDown();
        
        if (kDown & KEY_A) {
            pollServer(getServerAddress());
            std::cout << "Finished polling. Press A to poll a new Server.\n";
        }
        
        if (kDown & KEY_START) {
            break;
        }
    }
    
    return 0;
}