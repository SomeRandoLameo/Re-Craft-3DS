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

   
    char buffer[512];
    int bytes = recv(sock, buffer, sizeof(buffer), 0);
    if (bytes <= 0) {
        std::cerr << "Failed to receive server response\n";
        close(sock);
        return;
    }
    
    std::cout << "Received server response (" << bytes << " bytes)\n";
    
  
    auto responsePacket = encodeLoginResponsePacket(username);
    
    if (send(sock, responsePacket.data(), responsePacket.size(), 0) < 0) {
        std::cerr << "Failed to send login response packet\n";
        close(sock);
        return;
    }

    while (true) {

        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(sock, &readfds);
        
        struct timeval timeout;
        timeout.tv_sec = 1;
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
        
            //TODO: This is very unsafe, since it CAN read wrong values!
            int packetID = (unsigned char)buffer[0];
            
        
            if (packetID == 0) {
                bool debugKeepAlive = true;
                if(debugKeepAlive) std::cout << "Possible KeepAlive detected\n";

                char* packet_data = &buffer[1];
                int data_length = bytes - 1;
                
                if (data_length >= 4) {
                    unsigned char* bytes_ptr = (unsigned char*)packet_data;
                    
                    uint32_t java_int = (bytes_ptr[0] << 24) |  // Most significant byte
                                    (bytes_ptr[1] << 16) |  // 
                                    (bytes_ptr[2] << 8)  |  // 
                                    (bytes_ptr[3]);         // Least significant byte
                    
                    int32_t signed_value = (int32_t)java_int;
                    
                    if(debugKeepAlive) std::cout << "keepAlive value: " << signed_value << std::endl;

                    auto keepAliveResponse = encodeKeepAlivePacket(signed_value);
                    if(debugKeepAlive) std::cout << "sending: " << keepAliveResponse.data() << std::endl;
                    if (send(sock, keepAliveResponse.data(), keepAliveResponse.size(), 0) < 0) {
                        std::cerr << "Failed to send KeepAlive response\n";
                    } else {
                        if(debugKeepAlive) std::cout << "Sent KeepAlive response with value: " << signed_value << std::endl;
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