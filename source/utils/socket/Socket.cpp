#include "Socket.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>
#include <arpa/inet.h>
#include <errno.h>
#include <vector>
#include <3ds.h>

Socket::Socket() : sockfd(-1), timeoutMs(3000) {}

Socket::~Socket() {
    close();
}

void Socket::setTimeout(int ms) {
    // Store timeout for manual handling with select()
    this->timeoutMs = ms;
}

void Socket::setTcpNoDelay(bool enable) {
    // TCP_NODELAY is not available on 3DS - keep interface for compatibility
    (void)enable;
}

void Socket::setTrafficClass(int value) {
    // Not available on 3DS, but keep the interface
    (void)value;
}

bool Socket::connect(const std::string& host, int port, int timeoutMs) {
    // Create socket using the same pattern as the working example
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (sockfd < 0) {
        printf("socket() failed: %d %s\n", errno, strerror(errno));
        return false;
    }

    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    // Try direct IP conversion first
    int inet_result = inet_pton(AF_INET, host.c_str(), &server.sin_addr);
    if (inet_result <= 0) {
        // DNS resolution needed
        printf("Resolving hostname: %s\n", host.c_str());
        
        struct hostent* hostinfo = gethostbyname(host.c_str());
        if (!hostinfo) {
            printf("DNS resolution failed for %s: %d %s\n", host.c_str(), h_errno, strerror(h_errno));
            ::close(sockfd);
            sockfd = -1;
            return false;
        }
        
        if (hostinfo->h_length != 4 || hostinfo->h_addrtype != AF_INET) {
            printf("Invalid address format from DNS\n");
            ::close(sockfd);
            sockfd = -1;
            return false;
        }
        
        memcpy(&server.sin_addr, hostinfo->h_addr_list[0], hostinfo->h_length);
    }

    printf("Connecting to %s:%d (IP: %s)\n", 
           host.c_str(), port, inet_ntoa(server.sin_addr));

    // Set socket to non-blocking for timeout handling (like the example)
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) < 0) {
        printf("fcntl O_NONBLOCK failed: %d %s\n", errno, strerror(errno));
        ::close(sockfd);
        sockfd = -1;
        return false;
    }

    // Attempt connection
    int res = ::connect(sockfd, (struct sockaddr*)&server, sizeof(server));
    if (res < 0 && errno != EINPROGRESS && errno != EAGAIN) {
        printf("connect() failed immediately: %d %s\n", errno, strerror(errno));
        ::close(sockfd);
        sockfd = -1;
        return false;
    }

    if (res == 0) {
        // Connected immediately - restore blocking mode
        fcntl(sockfd, F_SETFL, flags);
        printf("Connection established immediately\n");
        return true;
    }

    // Wait for connection with timeout using select()
    fd_set writefds, errfds;
    FD_ZERO(&writefds);
    FD_ZERO(&errfds);
    FD_SET(sockfd, &writefds);
    FD_SET(sockfd, &errfds);

    struct timeval tv;
    tv.tv_sec = timeoutMs / 1000;
    tv.tv_usec = (timeoutMs % 1000) * 1000;

    res = select(sockfd + 1, nullptr, &writefds, &errfds, &tv);
    if (res <= 0) {
        if (res == 0) {
            printf("connect() timeout after %dms\n", timeoutMs);
        } else {
            printf("select() error: %d %s\n", errno, strerror(errno));
        }
        ::close(sockfd);
        sockfd = -1;
        return false;
    }

    // Check if there was an error
    if (FD_ISSET(sockfd, &errfds)) {
        printf("connect() error detected by select\n");
        ::close(sockfd);
        sockfd = -1;
        return false;
    }

    // Check SO_ERROR to see if connection was successful
    int so_error;
    socklen_t len = sizeof(so_error);
    if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &so_error, &len) < 0) {
        printf("getsockopt SO_ERROR failed: %d %s\n", errno, strerror(errno));
        ::close(sockfd);
        sockfd = -1;
        return false;
    }

    if (so_error != 0) {
        printf("connect() failed: %d %s\n", so_error, strerror(so_error));
        ::close(sockfd);
        sockfd = -1;
        return false;
    }

    // Restore blocking mode like the example does
    if (fcntl(sockfd, F_SETFL, flags) < 0) {
        printf("fcntl restore blocking failed: %d %s\n", errno, strerror(errno));
        ::close(sockfd);
        sockfd = -1;
        return false;
    }

    printf("Connection established successfully\n");
    return true;
}

int Socket::send(const void* data, size_t len) {
    if (sockfd < 0) return -1;
    
    // Use simple send like the working example
    ssize_t sent = ::send(sockfd, data, len, 0);
    if (sent < 0) {
        printf("send() failed: %d %s\n", errno, strerror(errno));
        return -1;
    }
    
    return sent;
}

int Socket::recv(void* buffer, size_t len) {
    if (sockfd < 0) return -1;
    
    ssize_t received = ::recv(sockfd, buffer, len, 0);
    if (received < 0) {
        printf("recv() failed: %d %s\n", errno, strerror(errno));
    }
    return received;
}

std::string Socket::readString(size_t maxLen) {
    if (sockfd < 0) return "";

    // Allocate buffer and read data
    std::vector<char> buffer(maxLen);
    ssize_t received = recv(buffer.data(), maxLen);
    
    if (received <= 0) {
        return "";
    }

    return std::string(buffer.data(), received);
}

void Socket::close() {
    if (sockfd >= 0) {
        ::close(sockfd);
        sockfd = -1;
    }
}