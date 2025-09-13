#ifndef NBTTAGLIST_HPP
#define NBTTAGLIST_HPP

#include "NBTBase.hpp"
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <iostream>

class NBTTagList : public NBTBase {
public:
    std::vector<std::unique_ptr<NBTBase>> tagList;
    uint8_t tagType;

    NBTTagList();
    explicit NBTTagList(const std::string& key);

    void writeTagContents(std::ostream& out) const override;
    void readTagContents(std::istream& in) override;
    uint8_t getType() const override;
    std::unique_ptr<NBTBase> cloneTag() const override;
    std::string toString() const;

    void addTag(std::unique_ptr<NBTBase> tag);
    NBTBase* tagAt(size_t index) const;
    size_t tagCount() const;
    bool equals(const NBTBase* other) const;
};

#endif // NBTTAGLIST_HPP
