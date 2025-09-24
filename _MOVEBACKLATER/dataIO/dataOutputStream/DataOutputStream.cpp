#include "DataOutputStream.hpp"
#include <sstream>
#include <iomanip>

void DataOutputStream::writeByte(uint8_t value) {
    buffer.push_back(value);
}

void DataOutputStream::writeShort(uint16_t value) {
    buffer.push_back((value >> 8) & 0xFF);
    buffer.push_back(value & 0xFF);
}

void DataOutputStream::writeInt(uint32_t value) {
    buffer.push_back((value >> 24) & 0xFF);
    buffer.push_back((value >> 16) & 0xFF);
    buffer.push_back((value >> 8) & 0xFF);
    buffer.push_back(value & 0xFF);
}

void DataOutputStream::writeLong(uint64_t value) {
    buffer.push_back((value >> 56) & 0xFF);
    buffer.push_back((value >> 48) & 0xFF);
    buffer.push_back((value >> 40) & 0xFF);
    buffer.push_back((value >> 32) & 0xFF);
    buffer.push_back((value >> 24) & 0xFF);
    buffer.push_back((value >> 16) & 0xFF);
    buffer.push_back((value >> 8) & 0xFF);
    buffer.push_back(value & 0xFF);
}


void DataOutputStream::writeChars(const std::string& s) {
    for (size_t i = 0; i < s.size(); ++i) {
        uint16_t v = static_cast<uint8_t>(s[i]);
        buffer.push_back((v >> 8) & 0xFF);
        buffer.push_back(v & 0xFF);
    }
}

void DataOutputStream::writeString(const std::string& s) {
    if (s.size() > 0x7FFF) { // Short.MAX_VALUE
        throw std::runtime_error("String too big");
    }
    writeShort(static_cast<uint16_t>(s.size())); 
    writeChars(s);                            
}

const std::vector<uint8_t>& DataOutputStream::getBuffer() const {
    return buffer;
}

std::string DataOutputStream::toString() const {
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (size_t i = 0; i < buffer.size(); ++i) {
        oss << std::setw(2) << static_cast<int>(buffer[i]);
        if (i + 1 < buffer.size()) {
            oss << " ";
        }
    }
    return oss.str();
}
