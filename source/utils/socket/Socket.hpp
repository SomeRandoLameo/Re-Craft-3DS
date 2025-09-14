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

    static std::vector<std::string> parseHost(const std::string& host);
    static int parseIntWithDefault(const std::string& str, int defaultValue);
    static bool isAllowedChar(char c);
    static std::string readString(int sock, int maxLength);
    static int utf16be_to_utf8(const uint8_t *in, int inLen, char *out, int outSize);
};

#endif