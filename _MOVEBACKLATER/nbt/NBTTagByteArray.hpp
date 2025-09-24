#ifndef NBTTAGBYTEARRAY_HPP
#define NBTTAGBYTEARRAY_HPP

#include "NBTBase.hpp"
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <iostream>

class NBTTagByteArray : public NBTBase {
public:
    std::vector<uint8_t> byteArray;

    explicit NBTTagByteArray(const std::string& key);
    NBTTagByteArray(const std::string& key, const std::vector<uint8_t>& data);

    void writeTagContents(std::ostream& out) const override;
    void readTagContents(std::istream& in) override;
    uint8_t getType() const override;
    std::unique_ptr<NBTBase> cloneTag() const override;
    std::string toString() const;
    bool equals(const NBTBase* other) const;
};

#endif // NBTTAGBYTEARRAY_HPP
