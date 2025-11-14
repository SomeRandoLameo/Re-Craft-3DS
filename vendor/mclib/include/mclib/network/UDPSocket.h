#ifndef NETWORK_UDP_SOCKET_H_
#define NETWORK_UDP_SOCKET_H_

#include "IPAddress.h"
#include "Socket.h"

#include <cstdint>

namespace mc {
namespace network {

class UDPSocket : public Socket {
private:
    IPAddress m_RemoteIP;
    uint16_t m_Port;
    sockaddr_in m_RemoteAddr;

public:
    UDPSocket();

    bool Connect(const IPAddress& address, uint16_t port);
    std::size_t Send(const u8* data, std::size_t size);
    DataBuffer Receive(std::size_t amount);
};

} // ns network
} // ns mc

#endif
