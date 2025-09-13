#ifndef NBTBASE_HPP
#define NBTBASE_HPP

#include <string>
#include <memory>
#include <cstdint>
#include <iostream>

class NBTBase {
protected:
    std::string key;

public:
    explicit NBTBase(const std::string& key = "");
    virtual ~NBTBase() = default;

    // Abstract methods
    virtual void writeTagContents(std::ostream& out) const = 0;
    virtual void readTagContents(std::istream& in) = 0;
    virtual uint8_t getType() const = 0;
    virtual std::unique_ptr<NBTBase> cloneTag() const = 0;

    // Utility methods
    bool equals(const NBTBase* other) const;
    NBTBase& setKey(const std::string& key);
    std::string getKey() const;

    // Static helpers
    static std::unique_ptr<NBTBase> readTag(std::istream& in);
    static void writeTag(const NBTBase& tag, std::ostream& out);
    static std::unique_ptr<NBTBase> createTagOfType(uint8_t type, const std::string& key);
    static std::string getTagName(uint8_t type);
};

#endif // NBTBASE_HPP
