#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <3ds.h>
#include <string>
#include <cstdint>
#include <vector>

class Socket {
private:
    static u32 *SOC_buffer;
    static bool socInitialized;

public:
    // Static methods for SOC management
    static void initSOC();
    static void shutdownSOC();

    static bool isAllowedChar(char c);
};

#endif