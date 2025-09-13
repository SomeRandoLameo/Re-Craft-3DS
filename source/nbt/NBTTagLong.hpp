#ifndef NBTTAGLONG_HPP
#define NBTTAGLONG_HPP

#include "NBTBase.hpp"
#include <cstdint>
#include <memory>
#include <string>
#include <iostream>

class NBTTagLong : public NBTBase {
public:
    int64_t longValue;

    explicit NBTTagLong(const std::string& key);
    NBTTagLong(const std::string& key, int64_t value);

    void writeTagContents(std::ostream& out) const override;
    void readTagContents(std::istream& in) override;
    uint8_t getType() const override;
    std::unique_ptr<NBTBase> cloneTag() const override;
    std::string toString() const;

    bool equals(const NBTBase* other) const;
};

#endif // NBTTAGLONG_HPP
