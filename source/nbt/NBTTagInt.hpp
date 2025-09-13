#ifndef NBTTAGINT_HPP
#define NBTTAGINT_HPP

#include "NBTBase.hpp"
#include <cstdint>
#include <memory>
#include <string>
#include <iostream>

class NBTTagInt : public NBTBase {
public:
    int32_t intValue;

    explicit NBTTagInt(const std::string& key);
    NBTTagInt(const std::string& key, int32_t value);

    void writeTagContents(std::ostream& out) const override;
    void readTagContents(std::istream& in) override;
    uint8_t getType() const override;
    std::unique_ptr<NBTBase> cloneTag() const override;
    std::string toString() const;

    bool equals(const NBTBase* other) const;
};

#endif // NBTTAGINT_HPP
