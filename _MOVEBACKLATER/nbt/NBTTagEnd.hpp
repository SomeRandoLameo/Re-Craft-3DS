#ifndef NBTTAGEND_HPP
#define NBTTAGEND_HPP

#include "NBTBase.hpp"
#include <memory>
#include <string>
#include <iostream>

class NBTTagEnd : public NBTBase {
public:
    NBTTagEnd();

    void readTagContents(std::istream& in) override;
    void writeTagContents(std::ostream& out) const override;
    uint8_t getType() const override;
    std::unique_ptr<NBTBase> cloneTag() const override;
    std::string toString() const;

    bool equals(const NBTBase* other) const;
};

#endif // NBTTAGEND_HPP
