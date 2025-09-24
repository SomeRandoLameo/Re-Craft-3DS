#pragma once

#include <string>
#include <cstdint>
#include <stdexcept>
#include <cstring>

class DataInputStream {
public:
    // Constructor takes raw buffer and its size
    DataInputStream(const uint8_t* data, size_t size);
    DataInputStream(const char* data, size_t size);

    uint8_t readByte();

    uint16_t readShort();

    uint32_t readInt();

    uint64_t readLong();

    char16_t readChar();
    
    std::string readString(size_t maxLength = 32767);
    
    uint32_t readVarInt();
    
    uint64_t readVarLong();
    
    bool endOfStream() const;
    size_t bytesRemaining() const;
    size_t getPosition() const;
    
    void skip(size_t count);

private:
    const uint8_t* buffer;
    size_t size;
    size_t pos;
    
    void checkBounds(size_t bytes_needed) const;
};