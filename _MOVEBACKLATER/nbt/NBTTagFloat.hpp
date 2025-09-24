#ifndef NBTTAGFLOAT_HPP
#define NBTTAGFLOAT_HPP

#include "NBTBase.hpp"
#include <cstdint>
#include <memory>
#include <string>
#include <iostream>

class NBTTagFloat : public NBTBase {
public:
    float floatValue;

    explicit NBTTagFloat(const std::string& key);
    NBTTagFloat(const std::string& key, float value);

    void writeTagContents(std::ostream& out) const override;
    void readTagContents(std::istream& in) override;
    uint8_t getType() const override;
    std::unique_ptr<NBTBase> cloneTag() const override;
    std::string toString() const;

    bool equals(const NBTBase* other) const;
};

#endif // NBTTAGFLOAT_HPP
