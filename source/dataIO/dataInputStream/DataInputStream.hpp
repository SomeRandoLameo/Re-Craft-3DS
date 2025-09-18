#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <stdexcept>

class DataInputStream {
public:
    DataInputStream(const std::vector<uint8_t>& input);

    uint8_t readByte();

    uint16_t readShort();

    uint32_t readInt();

    uint64_t readLong();

    char16_t readChar();

    std::string readString(size_t maxLength);

    bool endOfFile() const;

private:
    const std::vector<uint8_t>& buffer;
    size_t pos;
};