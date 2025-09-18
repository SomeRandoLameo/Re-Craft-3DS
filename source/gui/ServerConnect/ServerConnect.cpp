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
    std::vector<uint8_t> out;
   
    // Packet ID
    out.push_back(0x02);
    
    // String length (16-bit big-endian, matching Java's writeString format)
    uint16_t usernameLen = username.length();
    out.push_back((usernameLen >> 8) & 0xFF);
    out.push_back(usernameLen & 0xFF);
    
    // Write username in UTF-16 format (each char followed by 0x00)
    for (char c : username) {
        out.push_back(static_cast<uint8_t>(c));
        out.push_back(0x00);
    }
   
    return out;
}


std::vector<uint8_t> encodeLoginResponsePacket(const std::string& username) {
    std::vector<uint8_t> out;
    
    // Packet ID
    out.push_back(0x01);
    
    // protocol version
    uint32_t protocolVersion = 23;
    out.push_back((protocolVersion >> 24) & 0xFF);
    out.push_back((protocolVersion >> 16) & 0xFF);
    out.push_back((protocolVersion >> 8) & 0xFF);
    out.push_back(protocolVersion & 0xFF);
    
    // username length (big-endian)
    uint16_t usernameLen = username.length();
    out.push_back((usernameLen >> 8) & 0xFF);
    out.push_back(usernameLen & 0xFF);
    out.push_back(0x00); // Extra byte
    
    // username in UTF-16 
    for (char c : username) {
        out.push_back(static_cast<uint8_t>(c));
        out.push_back(0x00);
    }
    
    // null terminator for the string
    out.push_back(0x00);
    
    // remaining fields from the Java packet structure
    // mapSeed (8 bytes) - using 0 for now
    for (int i = 0; i < 8; i++) {
        out.push_back(0x00);
    }
    
    // worldType string (empty string = 2 bytes: 0x00, 0x00)
    out.push_back(0x00);
    out.push_back(0x00);
    
    // serverMode (4 bytes) - using 0
    for (int i = 0; i < 4; i++) {
        out.push_back(0x00);
    }
    
    out.push_back(0x00); // worldType
    out.push_back(0x00); // difficulty
    out.push_back(0x00); // worldHeight
    out.push_back(0x00); // maxPlayers
    
    return out;
}

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

    std::string username = "Nintendo3DS";
    
    std::cout << "Username length: " << username.length();
    std::cout << "Username: '" << username << "'" << std::endl;
    
    auto loginPacket = encodeLoginPacket(username);
    
    // Debug: Print the exact bytes being sent
    std::cout << "Login packet bytes: ";
    for (size_t i = 0; i < loginPacket.size(); i++) {
        printf("%02x", loginPacket[i]);
    }
    std::cout << std::endl;
    std::cout << "Login packet size: " << loginPacket.size() << " (should be 31)" << std::endl;
    
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