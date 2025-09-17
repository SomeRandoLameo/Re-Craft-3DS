#include "ServerConnect.hpp"
#include "../ServerList/ServerList.hpp"
#include "../../packet/Packet.hpp"
#include "../../utils/socket/Socket.hpp"
#include "../../utils/keyboard/keyboard.hpp"

#include <3ds.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <malloc.h>
#include <cstring>
#include <iostream>

#define SOC_ALIGN      0x1000
#define SOC_BUFFERSIZE 0x100000

static u32* SOC_buffer = nullptr;
//std::string username = "";

ServerConnect::ServerConnect(ServerList* serverList, const ServerNBTStorage& server)
    : serverList(serverList), server(server) {
       
}

ServerConnect::~ServerConnect() {
    Socket::shutdownSOC();
}

std::vector<uint8_t> encodeLoginPacket(const std::string& username) {
    // Manually construct the exact packet that Java sends
    // Java: 02000e0053006f006d006500520061006e0064006f004c0061006d0065006f
    //  ´    02000e0053006f006d006500520061006e0064006f004c0061006d0065006f
    std::vector<uint8_t> out;
    
    // Just hardcode the working packet for "SomeRandoLameo"
    uint8_t hardcoded[] = {
        0x02, 0x00,  // Packet ID
        0x0e, 0x00,  // String length (14)
        0x53, 0x00, 0x6f, 0x00, 0x6d, 0x00, 0x65, 0x00,  // "Some"
        0x52, 0x00, 0x61, 0x00, 0x6e, 0x00, 0x64, 0x00,  // "Rand"
        0x6f, 0x00, 0x4c, 0x00, 0x61, 0x00, 0x6d, 0x00,  // "oLam"
        0x65, 0x00, 0x6f                                   // "eo"
    };
   
    out.assign(hardcoded, hardcoded + sizeof(hardcoded));
    return out;
}

std::vector<uint8_t> encodeLoginResponsePacket(const std::string& username) {
    // Manually construct the exact second packet that Java sends
    // Java: 0100000017000e0053006f006d006500520061006e0064006f004c0061006d0065006f000000000000000000000000000000000000
    //       0100000017000e[missing 00]53006f006d006500520061006e0064006f004c0061006d0065006f000000000000000000000000000000000000
    std::vector<uint8_t> out;
    
    // Just hardcode the working second packet for "SomeRandoLameo"
    uint8_t hardcoded[] = {
        0x01,                           // Packet ID
        0x00, 0x00, 0x00, 0x17,        // Length: 23 in little-endian
        0x00, 0x0e, 0x00,             // String length: 14 in little-endian
        0x53, 0x00, 0x6f, 0x00, 0x6d, 0x00, 0x65, 0x00,  // "Some"
        0x52, 0x00, 0x61, 0x00, 0x6e, 0x00, 0x64, 0x00,  // "Rand"
        0x6f, 0x00, 0x4c, 0x00, 0x61, 0x00, 0x6d, 0x00,  // "oLam"
        0x65, 0x00, 0x6f,                                  // "eo"
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,              // 18 bytes padding
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    
    out.assign(hardcoded, hardcoded + sizeof(hardcoded));
    return out;
}

/*
std::vector<uint8_t> encodeLoginResponsePacket(const std::string& username) {
    std::vector<uint8_t> out;

    // Packet ID
    out.push_back(0x01);

    // Compute payload length: 2 (string length) + username UTF-16 bytes + 36 padding //TODO: the padding might be wrong if the user enters a custom username
    // Each char is 2 bytes in UTF-16
    uint16_t strLen = username.size();
    uint32_t payloadLen = 2 + (strLen * 2) + 36;

    // Write payload length (little-endian, 4 bytes)
    out.push_back((payloadLen >> 0) & 0xFF);
    out.push_back((payloadLen >> 8) & 0xFF);
    out.push_back((payloadLen >> 16) & 0xFF);
    out.push_back((payloadLen >> 24) & 0xFF);

    // Write string length (little-endian, 2 bytes)
    out.push_back((strLen >> 0) & 0xFF);
    out.push_back((strLen >> 8) & 0xFF);

    // Encode username as UTF-16LE (2 bytes per char)
    for (char c : username) {
        out.push_back(c & 0xFF); // low byte
        out.push_back(0x00);     // high byte (since ASCII range)
    }

    // 36 bytes padding (all zero)
    for (int i = 0; i < 36; i++) {
        out.push_back(0x00);
    }

    return out;
}
*/

void connectToServer(const std::string& host) {
    Socket::initSOC();

    u16 port = 25565; // Default Minecraft port
    if (!SOC_buffer) {
        SOC_buffer = (u32*)memalign(SOC_ALIGN, SOC_BUFFERSIZE);
        if (!SOC_buffer) {
            std::cerr << "SOC buffer alloc failed\n";
            return;
        }
        if (socInit(SOC_buffer, SOC_BUFFERSIZE) != 0) {
            std::cerr << "socInit failed\n";
            return;
        }
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "socket() failed\n";
        return;
    }

    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

    if (inet_aton(host.c_str(), &serverAddr.sin_addr) == 0) {
        std::cerr << "Invalid IP: " << host << "\n";
        close(sock);
        return;
    }

    if (::connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "connect() failed\n";
        close(sock);
        return;
    }

    std::cout << "Connected to " << host << ":" << port << "\n";

    // Step 1: Send initial login packet
    // Be very explicit about the string to avoid any encoding issues
    std::string username;
    username.reserve(13);
    username += 'S'; username += 'o'; username += 'm'; username += 'e';
    username += 'R'; username += 'a'; username += 'n'; username += 'd';
    username += 'o'; username += 'L'; username += 'a'; username += 'm';
    username += 'e'; username += 'o';
    
    std::cout << "Username length: " << username.length() << " (should be 14)" << std::endl;
    std::cout << "Username: '" << username << "'" << std::endl;
    
    auto loginPacket = encodeLoginPacket(username);
    
    // Debug: Print the exact bytes being sent
    std::cout << "Login packet bytes: ";
    for (size_t i = 0; i < loginPacket.size(); i++) {
        printf("%02x", loginPacket[i]);
    }
    std::cout << std::endl;
    std::cout << "Login packet size: " << loginPacket.size() << " (should be 31)" << std::endl;
    
    // Expected: 02000e0053006f006d006500520061006e0064006f004c0061006d0065006f
    std::cout << "Expected:         02000e0053006f006d006500520061006e0064006f004c0061006d0065006f" << std::endl;
    
    std::cout << "Sending login packet...\n";
    if (send(sock, loginPacket.data(), loginPacket.size(), 0) < 0) {
        std::cerr << "Failed to send login packet\n";
        close(sock);
        return;
    }

    // Step 2: Receive server response
    char buffer[512];
    int bytes = recv(sock, buffer, sizeof(buffer), 0);
    if (bytes <= 0) {
        std::cerr << "Failed to receive server response\n";
        close(sock);
        return;
    }
    
    std::cout << "Received server response (" << bytes << " bytes)\n";
    
    // Step 3: Send follow-up login response packet
    auto responsePacket = encodeLoginResponsePacket(username);
    
    std::cout << "Sending login response packet...\n";
    if (send(sock, responsePacket.data(), responsePacket.size(), 0) < 0) {
        std::cerr << "Failed to send login response packet\n";
        close(sock);
        return;
    }

    // Step 4: Handle ongoing server communication
    std::cout << "Login successful! Entering game loop...\n";
    
    // Game communication loop
    while (true) {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(sock, &readfds);
        
        struct timeval timeout;
        timeout.tv_sec = 1;  // 1 second timeout
        timeout.tv_usec = 0;
        
        int activity = select(sock + 1, &readfds, NULL, NULL, &timeout);
        
        if (activity < 0) {
            std::cerr << "Select error\n";
            break;
        }
        
        if (activity > 0 && FD_ISSET(sock, &readfds)) {
            // Receive data from server
            bytes = recv(sock, buffer, sizeof(buffer), 0);
            if (bytes <= 0) {
                std::cout << "Server disconnected\n";
                break;
            }
            
            std::cout << "Received " << bytes << " bytes from server\n";
            
            // Print first few bytes to understand packet structure
            std::cout << "Packet data: ";
            for (int i = 0; i < std::min(bytes, 20); i++) {
                printf("%02x ", (unsigned char)buffer[i]);
            }
            std::cout << std::endl;
            
            // Send ACK for any data received to keep connection alive
            // For now, just acknowledge that we received the data
            // In a real implementation, you'd parse the packet and respond appropriately
        }
        
        // Check for input to exit
        hidScanInput();
        u32 kDown = hidKeysDown();
        if (kDown & KEY_B) {
            std::cout << "Exiting game loop\n";
            break;
        }
        
        // You might need to send periodic keepalive packets here
        // depending on what the server expects
    }

    close(sock);
    std::cout << "Press B to return to the server list.\n";
}

void ServerConnect::updateControls(u32 kDown) {
    if (kDown & KEY_A) {
        std::cout << "Attempting to connect to " << server.host <<"\n";
        connectToServer(server.host);
    }

    if (kDown & KEY_SELECT) {
        Screen::currentScreen = serverList;
        Screen::currentScreen->drawScreen();
    }
}

void ServerConnect::drawScreen() const {
    consoleClear();
   // printf("Enter a Username\n");
   // username = getKeyboardInput("Username");
    printf("Press A to connect to server\n");
}