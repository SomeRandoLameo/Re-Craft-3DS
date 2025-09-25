#include "Socket.h"
#include "../common/DataBuffer.h"
#include "IPAddress.h"
#include "Network.h"
#include <3ds.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <errno.h>

namespace mc {
    namespace network {

        Socket::Socket(Type type)
                : m_Handle(INVALID_SOCKET),
                  m_Type(type),
                  m_Blocking(true), // 3DS sockets are blocking by default
                  m_Status(Disconnected)
        {

        }

        Socket::~Socket() {
            Disconnect();
        }

        void Socket::SetBlocking(bool block) {
            if (m_Handle == INVALID_SOCKET) {
                m_Blocking = block;
                return;
            }

            int flags = fcntl(m_Handle, F_GETFL, 0);
            if (flags < 0) return;

            if (block) {
                flags &= ~O_NONBLOCK;
            } else {
                flags |= O_NONBLOCK;
            }

            fcntl(m_Handle, F_SETFL, flags);
            m_Blocking = block;
        }

        bool Socket::IsBlocking() const noexcept {
            return m_Blocking;
        }

        Socket::Type Socket::GetType() const noexcept {
            return m_Type;
        }

        SocketHandle Socket::GetHandle() const noexcept {
            return m_Handle;
        }

        void Socket::SetStatus(Socket::Status status) {
            m_Status = status;
        }

        Socket::Status Socket::GetStatus() const noexcept {
            return m_Status;
        }

        bool Socket::Connect(const std::string& ip, uint16_t port) {
            IPAddress addr(ip);
            return Connect(addr, port);
        }

        std::size_t Socket::Send(const std::string& data) {
            return this->Send(reinterpret_cast<const unsigned char*>(data.c_str()), data.length());
        }

        std::size_t Socket::Send(DataBuffer& buffer) {
            std::string data = buffer.ToString();
            return this->Send(reinterpret_cast<const unsigned char*>(data.c_str()), data.length());
        }

        void Socket::Disconnect() {
            if (m_Handle != INVALID_SOCKET) {
                close(m_Handle);
                m_Handle = INVALID_SOCKET;
            }
            m_Status = Disconnected;
        }

    } // ns network
} // ns mc