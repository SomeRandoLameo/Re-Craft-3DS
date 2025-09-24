#pragma once

#include <vector>
#include <string>
#include <cstdint>

class DataOutputStream {
public:
    
    void writeByte(uint8_t value);

    void writeShort(uint16_t value);

    void writeInt(uint32_t value);

    void writeLong(uint64_t value);

    void writeChars(const std::string& s);

    void writeString(const std::string& s);



    const std::vector<uint8_t>& getBuffer() const;

    std::string toString() const;

private:
    std::vector<uint8_t> buffer;
};

