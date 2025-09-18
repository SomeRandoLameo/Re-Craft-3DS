#include "DataInputStream.hpp"

DataInputStream::DataInputStream(const std::vector<uint8_t>& input)
    : buffer(input), pos(0) { }

uint8_t DataInputStream::readByte() {
    if (pos >= buffer.size()) throw std::runtime_error("End of stream");
    return buffer[pos++];
}

uint16_t DataInputStream::readShort() {
    uint16_t result = (readByte() << 8) | readByte();
    return result;
}

uint32_t DataInputStream::readInt() {
    uint32_t result = (readByte() << 24) | (readByte() << 16) | (readByte() << 8) | readByte();
    return result;
}

uint64_t DataInputStream::readLong() {
    uint64_t result = 0;
    for (int i = 0; i < 8; ++i) {
        result = (result << 8) | readByte();
    }
    return result;
}

char16_t DataInputStream::readChar() {
    uint16_t v = readShort();
    return static_cast<char16_t>(v);
}

std::string DataInputStream::readString(size_t maxLength) {
    uint16_t length = readShort();
    if (length > maxLength) {
        throw std::runtime_error("Received string length longer than maximum allowed (" +
                                 std::to_string(length) + " > " + std::to_string(maxLength) + ")");
    } else if (length < 0) {
        throw std::runtime_error("Received string length is less than zero! Weird string!");
    }

    std::string result;
    for (size_t i = 0; i < length; ++i) {
        char16_t c = readChar();
        if (c <= 0x7F) {
            result += static_cast<char>(c);
        } else {
            result += '?';
        }
    }
    return result;
}

bool DataInputStream::endOfFile() const {
    return pos >= buffer.size();
}
