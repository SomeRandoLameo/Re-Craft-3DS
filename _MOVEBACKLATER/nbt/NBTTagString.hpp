#ifndef NBTTAGSTRING_HPP
#define NBTTAGSTRING_HPP

#include "NBTBase.hpp"
#include <cstdint>
#include <memory>
#include <string>
#include <iostream>

class NBTTagString : public NBTBase {
public:
    std::string stringValue;

    explicit NBTTagString(const std::string& key);
    NBTTagString(const std::string& key, const std::string& value);

    void writeTagContents(std::ostream& out) const override;
    void readTagContents(std::istream& in) override;
    uint8_t getType() const override;
    std::unique_ptr<NBTBase> cloneTag() const override;
    std::string toString() const;

    bool equals(const NBTBase* other) const;
};

#endif // NBTTAGSTRING_HPP
