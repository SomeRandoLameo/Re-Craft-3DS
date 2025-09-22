#include "ServerConnect.hpp"
#include "../ServerList/ServerList.hpp"
#include "../../utils/socket/Socket.hpp"
#include "../../utils/keyboard/keyboard.hpp"

#include "../../dataIO/dataOutputStream/DataOutputStream.hpp"
#include "../../dataIO/dataInputStream/DataInputStream.hpp"

//TODO: Some form of networkmanager or packetRegistry, this sucks
#include "../../packet/Packet0KeepAlive/Packet0KeepAlive.hpp"
#include "../../packet/Packet1Login/Packet1Login.hpp"
#include "../../packet/Packet2Handshake/Packet2Handshake.hpp"
#include "../../packet/Packet3Chat/Packet3Chat.hpp"
#include "../../packet/Packet255KickDisconnect/Packet255KickDisconnect.hpp"

#include <3ds.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <malloc.h>
#include <cstring>
#include <iostream>
#include <iomanip>

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

bool sendPacket(int sock, const std::vector<uint8_t>& packet, const std::string& packetName) {
    if (send(sock, packet.data(), packet.size(), 0) < 0) {
        std::cerr << "Failed to send " << packetName << " packet\n";
        return false;
    }
    return true;
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

    Packet2Handshake handshake;
    handshake.username = "Nintendo3DS";
    
    DataOutputStream hsData;
    handshake.writePacketData(hsData);

    if (!sendPacket(sock, hsData.getBuffer(), "Packet2Handshake")) {
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

    Packet1Login login;
    login.username = handshake.username;

    DataOutputStream liData;
    login.writePacketData(liData);

    if (!sendPacket(sock, liData.getBuffer(), "Packet1Login")) {
        close(sock);
        return;
    }
//TODO: This needs to be moved inside some form of nethandler
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
        
            bytes = recv(sock, buffer, sizeof(buffer), 0);
            if (bytes <= 0) {
                std::cout << "Server disconnected\n";
                break;
            }
        
            DataInputStream stream(buffer, bytes);
            
            try {
                //packet ID should be first byte, this can be really unstable tho...
                uint8_t packetID = stream.readByte();

                // Packet0KeepAlive 
                if (packetID == 0) {
                    Packet0KeepAlive packet;

                    bool debugKeepAlive = false;
                    
                    packet.debugPacket = debugKeepAlive;

                    if(debugKeepAlive) std::cout << "Possible KeepAlive detected\n";
                    //this really shouldnt be nessessarry
                    if (stream.bytesRemaining() >= 4) {

                        packet.readPacketData(stream);
                        
                        //not every packet should create its own output stream...
                        DataOutputStream packetData;
                        packet.writePacketData(packetData);

                        auto keepAliveResponse = packetData.getBuffer();

                        if(debugKeepAlive) {
                            std::cout << "Packetbuffer: ";
                            for(const auto& byte : keepAliveResponse) {
                                std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte) << " ";
                            }
                            std::cout << std::dec << std::endl;
                        }

                        if (!sendPacket(sock, keepAliveResponse, "KeepAlive response")) {
                            std::cerr << "Failed to send KeepAlive response\n";
                        } 

                    } else {
                        std::cerr << "KeepAlive packet too short\n";
                    }
                }
                if(packetID == 3){
                    Packet3Chat packet;

                    //PacketData is too big sometimes
                    packet.readPacketData(stream);
                        
                    std::cout << packet.message << "\n";
                }
                if (packetID == 255) {
                    Packet255KickDisconnect packet;

                    //PacketData is too big sometimes
                    packet.readPacketData(stream);
                        
                    std::cout << packet.reason << "\n";
                
                }
                
            } catch (const std::exception& e) {
                std::cerr << "Error parsing packet: " << e.what() << std::endl;
            }
        }
        
        hidScanInput();
        u32 kDown = hidKeysDown();
        if (kDown & KEY_B) {
            std::cout << "Exiting game loop\n";
            break;
        }
    }
    close(sock);
    std::cout << "Press A to reconnect.\n";
    std::cout << "Press SELECT to return to the server list.\n";
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