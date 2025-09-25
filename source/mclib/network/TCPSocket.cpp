#include "TCPSocket.h"
#include "../common/DataBuffer.h"
#include <3ds.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <cstring>
#include <iostream>

namespace mc {
    namespace network {

        TCPSocket::TCPSocket()
                : Socket(Socket::TCP), m_Port(0)
        {
            m_Handle = INVALID_SOCKET;
            memset(&m_RemoteAddr, 0, sizeof(m_RemoteAddr));
        }

        bool TCPSocket::Connect(const IPAddress& address, unsigned short port) {
            if (this->GetStatus() == Connected)
                return true;

            // Create socket
            m_Handle = socket(AF_INET, SOCK_STREAM, 0);
            if (m_Handle < 0) {
                this->SetStatus(Error);
                return false;
            }

            // Setup address structure
            memset(&m_RemoteAddr, 0, sizeof(m_RemoteAddr));
            m_RemoteAddr.sin_family = AF_INET;
            m_RemoteAddr.sin_port = htons(port);

            // Convert IP address string to binary form
            if (inet_pton(AF_INET, address.ToString().c_str(), &m_RemoteAddr.sin_addr) <= 0) {
                close(m_Handle);
                m_Handle = INVALID_SOCKET;
                this->SetStatus(Error);
                return false;
            }

            // Attempt connection
            if (connect(m_Handle, (struct sockaddr*)&m_RemoteAddr, sizeof(m_RemoteAddr)) < 0) {
                int error = errno;
                close(m_Handle);
                m_Handle = INVALID_SOCKET;
                this->SetStatus(Error);
                return false;
            }

            // Set blocking mode if needed
            this->SetBlocking(this->IsBlocking());

            this->SetStatus(Connected);
            m_RemoteIP = address;
            m_Port = port;

            return true;
        }

        size_t TCPSocket::Send(const unsigned char* data, size_t size) {
            if (this->GetStatus() != Connected || m_Handle == INVALID_SOCKET)
                return 0;

            size_t sent = 0;

            while (sent < size) {
                ssize_t cur = send(m_Handle, reinterpret_cast<const char*>(data + sent), size - sent, 0);
                if (cur < 0) {
                    int error = errno;
                    if (error == EAGAIN || error == EWOULDBLOCK) {
                        // Non-blocking socket would block, return what we've sent so far
                        break;
                    }
                    // Connection error
                    Disconnect();
                    return sent;
                } else if (cur == 0) {
                    // Connection closed
                    Disconnect();
                    return sent;
                }
                sent += cur;
            }

            return sent;
        }

        std::size_t TCPSocket::Receive(DataBuffer& buffer, std::size_t amount) {
            if (this->GetStatus() != Connected || m_Handle == INVALID_SOCKET) {
                buffer.Clear();
                return 0;
            }

            buffer.Resize(amount);
            buffer.SetReadOffset(0);

            ssize_t received = recv(m_Handle, (char*)&buffer[0], amount, 0);

            if (received < 0) {
                int error = errno;
                if (error == EAGAIN || error == EWOULDBLOCK) {
                    // No data available (non-blocking mode)
                    buffer.Clear();
                    return 0;
                }
                // Connection error
                Disconnect();
                buffer.Clear();
                return 0;
            } else if (received == 0) {
                // Connection closed by peer
                Disconnect();
                buffer.Clear();
                return 0;
            }

            buffer.Resize(received);
            return received;
        }

        DataBuffer TCPSocket::Receive(std::size_t amount) {
            if (this->GetStatus() != Connected || m_Handle == INVALID_SOCKET) {
                return DataBuffer();
            }

            std::unique_ptr<char[]> buf(new char[amount]);

            ssize_t received = recv(m_Handle, buf.get(), amount, 0);

            if (received < 0) {
                int error = errno;
                if (error == EAGAIN || error == EWOULDBLOCK) {
                    // No data available (non-blocking mode)
                    return DataBuffer();
                }
                // Connection error
                Disconnect();
                return DataBuffer();
            } else if (received == 0) {
                // Connection closed by peer
                Disconnect();
                return DataBuffer();
            }

            return DataBuffer(std::string(buf.get(), received));
        }

    } // ns network
} // ns mc