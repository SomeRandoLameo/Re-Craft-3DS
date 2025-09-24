#ifndef NBTTAGCOMPOUND_HPP
#define NBTTAGCOMPOUND_HPP

#include "NBTBase.hpp"
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>

class NBTTagList;

class NBTTagCompound : public NBTBase {
public:
    std::unordered_map<std::string, std::unique_ptr<NBTBase>> tagMap;

    NBTTagCompound();
    explicit NBTTagCompound(const std::string& key);

    void writeTagContents(std::ostream& out) const override;
    void readTagContents(std::istream& in) override;
    uint8_t getType() const override;
    std::unique_ptr<NBTBase> cloneTag() const override;
    std::string toString() const;

    void setTag(const std::string& key, std::unique_ptr<NBTBase> tag);
    void setByte(const std::string& key, uint8_t value);
    void setShort(const std::string& key, int16_t value);
    void setInteger(const std::string& key, int32_t value);
    void setLong(const std::string& key, int64_t value);
    void setFloat(const std::string& key, float value);
    void setDouble(const std::string& key, double value);
    void setString(const std::string& key, const std::string& value);
    void setByteArray(const std::string& key, const std::vector<uint8_t>& value);
    void setCompoundTag(const std::string& key, std::unique_ptr<NBTTagCompound> value);
    void setBoolean(const std::string& key, bool value);

    NBTBase* getTag(const std::string& key) const;
    bool hasKey(const std::string& key) const;

    uint8_t getByte(const std::string& key) const;
    int16_t getShort(const std::string& key) const;
    int32_t getInteger(const std::string& key) const;
    int64_t getLong(const std::string& key) const;
    float getFloat(const std::string& key) const;
    double getDouble(const std::string& key) const;
    std::string getString(const std::string& key) const;
    std::vector<uint8_t> getByteArray(const std::string& key) const;
    NBTTagCompound* getCompoundTag(const std::string& key) const;
    NBTTagList* getTagList(const std::string& key) const;
    bool getBoolean(const std::string& key) const;

    bool equals(const NBTBase* other) const;
};

#endif // NBTTAGCOMPOUND_HPP
