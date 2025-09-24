#ifndef NBTTAGSHORT_HPP
#define NBTTAGSHORT_HPP

#include "NBTBase.hpp"
#include <cstdint>
#include <memory>
#include <string>
#include <iostream>

class NBTTagShort : public NBTBase {
public:
    int16_t shortValue;

    explicit NBTTagShort(const std::string& key);
    NBTTagShort(const std::string& key, int16_t value);

    void writeTagContents(std::ostream& out) const override;
    void readTagContents(std::istream& in) override;
    uint8_t getType() const override;
    std::unique_ptr<NBTBase> cloneTag() const override;
    std::string toString() const;

    bool equals(const NBTBase* other) const;
};

#endif // NBTTAGSHORT_HPP
