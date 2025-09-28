#pragma once

#include "IPAddress.h"
#include "Socket.h"
#include <cstdint>
#include <sys/socket.h>
#include <netinet/in.h>

namespace mc {
    namespace network {

        class TCPSocket : public Socket {
        private:
            IPAddress m_RemoteIP;
            uint16_t m_Port;
            sockaddr_in m_RemoteAddr;

        public:
            TCPSocket();

            bool Connect(const IPAddress& address, uint16_t port);
            std::size_t Send(const u8* data, std::size_t size);
            DataBuffer Receive(std::size_t amount);
            std::size_t Receive(DataBuffer& buffer, std::size_t amount);
        };

    } // ns network
} // ns mc