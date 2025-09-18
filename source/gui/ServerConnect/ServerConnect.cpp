#include "ServerConnect.hpp"
#include "../ServerList/ServerList.hpp"
#include "../../utils/socket/Socket.hpp"
#include "../../utils/keyboard/keyboard.hpp"

#include "../../dataIO/dataOutputStream/DataOutputStream.hpp"
#include "../../dataIO/dataInputStream/DataInputStream.hpp"

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
    DataOutputStream dos;
   
    dos.writeByte(2);           // Packet ID
    
    dos.writeString(username);  // username
   
    return dos.getBuffer();
}

std::vector<uint8_t> encodeLoginResponsePacket(const std::string& username) {
    DataOutputStream dos;
    
    dos.writeByte(1);           // Packet ID
    
    dos.writeInt(23);           // protocol version
    dos.writeString(username);  // username
    
    dos.writeLong(0);           // mapSeed 
    
    dos.writeString("");        // worldType string 
    /* TODO: If WorldType is implemented and the packets are somewhere else, implement this

		if(this.field_46032_d == null) {
			writeString("", var1);
		} else {
			writeString(this.field_46032_d.name(), var1);
		}

        */
    dos.writeInt(0);            // serverMode

    dos.writeByte(0);           // worldType
    dos.writeByte(0);           // difficulty
    dos.writeByte(0);           // worldHeight
    dos.writeByte(0);           // maxPlayers
    
    return dos.getBuffer();
}


std::vector<uint8_t> encodeKeepAlivePacket(int responseID){
    DataOutputStream dos;
    dos.writeInt(responseID);
    return dos.getBuffer();
}


//TODO: Use my own DataInputStream. this is temporary
int decodeKeepAlivePacket(const char* buffer, int bytes) {
    if (bytes < 9) { // Need at least 1 byte for packet ID + 8 bytes for UTF-16LE int
        return -1; // Error: packet too short
    }
    
    // Skip the first byte (packet ID 0x00)
    // Read the 4-byte integer in UTF-16LE format (8 bytes total)
    // Each byte is followed by a null byte in UTF-16LE
    int responseID = (buffer[1]) |           // Low byte of low word
                     (buffer[3] << 8) |      // High byte of low word  
                     (buffer[5] << 16) |     // Low byte of high word
                     (buffer[7] << 24);      // High byte of high word
    
    return responseID;
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

    auto loginPacket = encodeLoginPacket(username);
    
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
    
    if (send(sock, responsePacket.data(), responsePacket.size(), 0) < 0) {
        std::cerr << "Failed to send login response packet\n";
        close(sock);
        return;
    }

    // Game communication loop
    while (true) {


        //TODO: Fix sending KeepAlive packets. this doesnt work. I am tired
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
            bytes = recv(sock, buffer, sizeof(buffer), 0);
            if (bytes <= 0) {
                std::cout << "Server disconnected\n";
                break;
            }

            if (bytes > 0) {
                uint8_t firstByte = (unsigned char)buffer[0];
                std::cout << "First byte: " << (int)firstByte << std::endl;
                
                if (firstByte == 0) { // Keep-alive packet
                    //TODO: Whatever this is needs some attention
                    int keepAliveID = decodeKeepAlivePacket(buffer, bytes);
                    if (keepAliveID != -1) {
                        std::cout << "Keep-alive ID: " << keepAliveID << std::endl;
                        
                        // Send the response back
                        auto response = encodeKeepAlivePacket(keepAliveID);
                        send(sock, response.data(), response.size(), 0);
                        std::cout << "Sent keep-alive response\n";
                    }
                }
            }
        }
        
        // Check for input to exit
        hidScanInput();
        u32 kDown = hidKeysDown();
        if (kDown & KEY_B) {
            std::cout << "Exiting game loop\n";
            break;
        }
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