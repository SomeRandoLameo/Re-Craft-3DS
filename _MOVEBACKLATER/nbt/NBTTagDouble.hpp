#ifndef NBTTAGDOUBLE_HPP
#define NBTTAGDOUBLE_HPP

#include "NBTBase.hpp"
#include <cstdint>
#include <memory>
#include <string>
#include <iostream>

class NBTTagDouble : public NBTBase {
public:
    double doubleValue;

    explicit NBTTagDouble(const std::string& key);
    NBTTagDouble(const std::string& key, double value);

    void writeTagContents(std::ostream& out) const override;
    void readTagContents(std::istream& in) override;
    uint8_t getType() const override;
    std::unique_ptr<NBTBase> cloneTag() const override;
    std::string toString() const;

    bool equals(const NBTBase* other) const;
};

#endif // NBTTAGDOUBLE_HPP
