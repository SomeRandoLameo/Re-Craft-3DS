#pragma once
#include <string>
#include <cstdint>

class Socket {
public:
    Socket();
    ~Socket();

    void setTimeout(int ms);
    void setTcpNoDelay(bool enable);
    void setTrafficClass(int value);

    bool connect(const std::string& host, int port, int timeoutMs);
    int send(const void* data, size_t len);
    int recv(void* buffer, size_t len);
    std::string readString(size_t maxLen);

    void close();

private:
    int sockfd;
    int timeoutMs = 3000;
};