#include "DataInputStream.hpp"

DataInputStream::DataInputStream(const uint8_t* data, size_t size)
    : buffer(data), size(size), pos(0) {
    if (!data && size > 0) {
        throw std::runtime_error("Invalid buffer: null pointer with non-zero size");
    }
}

DataInputStream::DataInputStream(const char* data, size_t size)
    : buffer(reinterpret_cast<const uint8_t*>(data)), size(size), pos(0) {
    if (!data && size > 0) {
        throw std::runtime_error("Invalid buffer: null pointer with non-zero size");
    }
}

void DataInputStream::checkBounds(size_t bytes_needed) const {
    if (pos + bytes_needed > size) {
        throw std::runtime_error("End of stream: trying to read " + 
                                std::to_string(bytes_needed) + " bytes, but only " + 
                                std::to_string(size - pos) + " bytes remaining");
    }
}

uint8_t DataInputStream::readByte() {
    checkBounds(1);
    return buffer[pos++];
}

uint16_t DataInputStream::readShort() {
    checkBounds(2);
    // Java uses big-endian (network byte order)
    uint16_t result = (static_cast<uint16_t>(buffer[pos]) << 8) | 
                      static_cast<uint16_t>(buffer[pos + 1]);
    pos += 2;
    return result;
}

uint32_t DataInputStream::readInt() {
    checkBounds(4);
    // Java uses big-endian (network byte order)
    uint32_t result = (static_cast<uint32_t>(buffer[pos]) << 24) |
                      (static_cast<uint32_t>(buffer[pos + 1]) << 16) |
                      (static_cast<uint32_t>(buffer[pos + 2]) << 8) |
                      static_cast<uint32_t>(buffer[pos + 3]);
    pos += 4;
    return result;
}

uint64_t DataInputStream::readLong() {
    checkBounds(8);
    // Java uses big-endian (network byte order)
    uint64_t result = 0;
    for (int i = 0; i < 8; ++i) {
        result = (result << 8) | static_cast<uint64_t>(buffer[pos + i]);
    }
    pos += 8;
    return result;
}

char16_t DataInputStream::readChar() {
    return static_cast<char16_t>(readShort());
}

std::string DataInputStream::readString(size_t maxLength) {
    uint16_t length = readShort();
    
    if (length > maxLength) {
        throw std::runtime_error("String length " + std::to_string(length) + 
                                " exceeds maximum " + std::to_string(maxLength));
    }
    
    checkBounds(length * 2); // Java chars are 2 bytes each
    
    std::string result;
    result.reserve(length); // Optimize memory allocation
    
    for (uint16_t i = 0; i < length; ++i) {
        char16_t c = readChar();
        // Convert UTF-16 to ASCII (basic conversion)
        if (c <= 0x7F) {
            result += static_cast<char>(c);
        } else if (c <= 0x7FF) {
            // Basic UTF-8 encoding for 2-byte chars
            result += static_cast<char>(0xC0 | (c >> 6));
            result += static_cast<char>(0x80 | (c & 0x3F));
        } else {
            // For simplicity, replace with '?' for chars > 2 bytes
            result += '?';
        }
    }
    
    return result;
}

uint32_t DataInputStream::readVarInt() {
    uint32_t result = 0;
    int shift = 0;
    
    while (true) {
        if (shift >= 32) {
            throw std::runtime_error("VarInt too long");
        }
        
        uint8_t byte = readByte();
        result |= static_cast<uint32_t>(byte & 0x7F) << shift;
        
        if ((byte & 0x80) == 0) {
            break;
        }
        
        shift += 7;
    }
    
    return result;
}

uint64_t DataInputStream::readVarLong() {
    uint64_t result = 0;
    int shift = 0;
    
    while (true) {
        if (shift >= 64) {
            throw std::runtime_error("VarLong too long");
        }
        
        uint8_t byte = readByte();
        result |= static_cast<uint64_t>(byte & 0x7F) << shift;
        
        if ((byte & 0x80) == 0) {
            break;
        }
        
        shift += 7;
    }
    
    return result;
}

bool DataInputStream::endOfStream() const {
    return pos >= size;
}

size_t DataInputStream::bytesRemaining() const {
    return pos < size ? size - pos : 0;
}

size_t DataInputStream::getPosition() const {
    return pos;
}

void DataInputStream::skip(size_t count) {
    checkBounds(count);
    pos += count;
}