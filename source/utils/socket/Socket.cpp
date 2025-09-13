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

std::vector<std::string> Socket::parseHost(const std::string& host) {
    std::vector<std::string> hostParts;
    
    // Handle IPv6 addresses in brackets [address]:port
    if (host.front() == '[') {
        size_t endIndex = host.find(']');
        if (endIndex != std::string::npos) {
            std::string address = host.substr(1, endIndex - 1);
            std::string portPart = host.substr(endIndex + 1);
            
            // Trim whitespace
            portPart.erase(0, portPart.find_first_not_of(" \t"));
            portPart.erase(portPart.find_last_not_of(" \t") + 1);
            
            if (portPart.front() == ':' && portPart.length() > 1) {
                portPart = portPart.substr(1);
                hostParts = {address, portPart};
            } else {
                hostParts = {address};
            }
            return hostParts;
        }
    }
    
    // Regular host:port parsing
    size_t colonPos = host.find(':');
    if (colonPos != std::string::npos) {
        hostParts.push_back(host.substr(0, colonPos));
        hostParts.push_back(host.substr(colonPos + 1));
    } else {
        hostParts.push_back(host);
    }
    
    return hostParts;
}

int Socket::parseIntWithDefault(const std::string& str, int defaultValue) {
    try {
        return std::stoi(str);
    } catch (...) {
        return defaultValue;
    }
}

bool Socket::isAllowedChar(char c) {
    // Simplified character check (similar to ChatAllowedCharacters concept)
    return (c >= 32 && c <= 126) || c == '\n' || c == '\t';
}

std::string Socket::readString(int sock, int maxLength) {
    // Read string length (2 bytes, big endian)
    uint8_t lengthBytes[2];
    if (recv(sock, lengthBytes, 2, 0) != 2) {
        throw std::runtime_error("Failed to read string length");
    }
    
    int strLen = (lengthBytes[0] << 8) | lengthBytes[1];
    if (strLen > maxLength || strLen < 0) {
        throw std::runtime_error("Invalid string length");
    }
    
    // Read UTF-16BE string data
    std::vector<uint8_t> utf16Data(strLen * 2);
    int totalRead = 0;
    while (totalRead < strLen * 2) {
        int bytesRead = recv(sock, utf16Data.data() + totalRead, strLen * 2 - totalRead, 0);
        if (bytesRead <= 0) {
            throw std::runtime_error("Failed to read string data");
        }
        totalRead += bytesRead;
    }
    
    // Convert UTF-16BE to UTF-8
    char utf8Buffer[1024];
    utf16be_to_utf8(utf16Data.data(), strLen * 2, utf8Buffer, sizeof(utf8Buffer));
    
    return std::string(utf8Buffer);
}

int Socket::utf16be_to_utf8(const uint8_t *in, int inLen, char *out, int outSize) {
    int i = 0, o = 0;
    while (i + 1 < inLen && o + 4 < outSize) {
        uint16_t code = (in[i] << 8) | in[i+1];
        i += 2;

        if (code < 0x80) {
            out[o++] = static_cast<char>(code);
        } else if (code < 0x800) {
            out[o++] = static_cast<char>(0xC0 | (code >> 6));
            out[o++] = static_cast<char>(0x80 | (code & 0x3F));
        } else {
            out[o++] = static_cast<char>(0xE0 | (code >> 12));
            out[o++] = static_cast<char>(0x80 | ((code >> 6) & 0x3F));
            out[o++] = static_cast<char>(0x80 | (code & 0x3F));
        }
    }
    out[o] = '\0';
    return o;
}