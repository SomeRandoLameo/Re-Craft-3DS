#ifndef NBTTAGBYTE_HPP
#define NBTTAGBYTE_HPP

#include "NBTBase.hpp"
#include <cstdint>
#include <memory>
#include <string>
#include <iostream>

class NBTTagByte : public NBTBase {
public:
    int8_t byteValue;

    explicit NBTTagByte(const std::string& key);
    NBTTagByte(const std::string& key, int8_t value);

    void writeTagContents(std::ostream& out) const override;
    void readTagContents(std::istream& in) override;
    uint8_t getType() const override;
    std::unique_ptr<NBTBase> cloneTag() const override;
    std::string toString() const;

    bool equals(const NBTBase* other) const;
};

#endif // NBTTAGBYTE_HPP
