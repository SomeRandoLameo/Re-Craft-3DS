#include "Socket.hpp"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <malloc.h>
#include <array>
#include <algorithm>
#include <sstream>

#define SOC_ALIGN       0x1000
#define SOC_BUFFERSIZE  0x100000

// Static member definitions
u32* Socket::SOC_buffer = nullptr;
bool Socket::socInitialized = false;

void Socket::initSOC() {
    if (socInitialized) return;
    
    SOC_buffer = static_cast<u32*>(memalign(SOC_ALIGN, SOC_BUFFERSIZE));
    if (!SOC_buffer) {
        std::cerr << "Failed to allocate SOC buffer\n";
        return;
    }

    int ret = socInit(SOC_buffer, SOC_BUFFERSIZE);
    if (ret != 0) {
        std::cerr << "socInit failed: 0x" << std::hex << ret << std::dec << "\n";
        return;
    }

    socInitialized = true;
    std::atexit(shutdownSOC);
}

void Socket::shutdownSOC() {
    if (socInitialized) {
        std::cout << "Shutting down SOC...\n";
        socExit();
        if (SOC_buffer) {
            free(SOC_buffer);
            SOC_buffer = nullptr;
        }
        socInitialized = false;
    }
}


bool Socket::isAllowedChar(char c) {
    // Simplified character check (similar to ChatAllowedCharacters concept)
    return (c >= 32 && c <= 126) || c == '\n' || c == '\t';
}
