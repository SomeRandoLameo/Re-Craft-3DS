#include "NBTBase.hpp"
#include "NBTTagEnd.hpp"
#include "NBTTagByte.hpp"
#include "NBTTagShort.hpp"
#include "NBTTagInt.hpp"
#include "NBTTagLong.hpp"
#include "NBTTagFloat.hpp"
#include "NBTTagDouble.hpp"
#include "NBTTagByteArray.hpp"
#include "NBTTagString.hpp"
#include "NBTTagList.hpp"
#include "NBTTagCompound.hpp"

#include <cstdint>

inline uint16_t htons(uint16_t x) {
    return (x << 8) | (x >> 8);
}

inline uint16_t ntohs(uint16_t x) {
    return htons(x); // same as above
}

NBTBase::NBTBase(const std::string& k) {
    key = k.empty() ? "" : k;
}

bool NBTBase::equals(const NBTBase* other) const {
    if (!other) return false;
    if (this->getType() != other->getType()) return false;
    return key == other->key;
}

NBTBase& NBTBase::setKey(const std::string& k) {
    key = k.empty() ? "" : k;
    return *this;
}

std::string NBTBase::getKey() const {
    return key.empty() ? "" : key;
}

std::unique_ptr<NBTBase> NBTBase::readTag(std::istream& in) {
    uint8_t type;
    in.read(reinterpret_cast<char*>(&type), sizeof(type));

    if (type == 0) {
        return std::unique_ptr<NBTTagEnd>(new NBTTagEnd());
    } else {
        uint16_t len;
        in.read(reinterpret_cast<char*>(&len), sizeof(len));
        len = ntohs(len);

        std::string key(len, '\0');
        in.read(&key[0], len);

        auto tag = createTagOfType(type, key);
        if (tag) tag->readTagContents(in);
        return tag;
    }
}

void NBTBase::writeTag(const NBTBase& tag, std::ostream& out) {
    uint8_t type = tag.getType();
    out.write(reinterpret_cast<const char*>(&type), sizeof(type));
    if (type != 0) {
        uint16_t len = htons(tag.getKey().size());
        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
        out.write(tag.getKey().c_str(), tag.getKey().size());
        tag.writeTagContents(out);
    }
}

std::unique_ptr<NBTBase> NBTBase::createTagOfType(uint8_t type, const std::string& key) {
    switch (type) {
        case 0:  return std::unique_ptr<NBTTagEnd>      (new NBTTagEnd());
        case 1:  return std::unique_ptr<NBTTagByte>     (new NBTTagByte(key));
        case 2:  return std::unique_ptr<NBTTagShort>    (new NBTTagShort(key));
        case 3:  return std::unique_ptr<NBTTagInt>      (new NBTTagInt(key));
        case 4:  return std::unique_ptr<NBTTagLong>     (new NBTTagLong(key));
        case 5:  return std::unique_ptr<NBTTagFloat>    (new NBTTagFloat(key));
        case 6:  return std::unique_ptr<NBTTagDouble>   (new NBTTagDouble(key));
        case 7:  return std::unique_ptr<NBTTagByteArray>(new NBTTagByteArray(key));
        case 8:  return std::unique_ptr<NBTTagString>   (new NBTTagString(key));
        case 9:  return std::unique_ptr<NBTTagList>     (new NBTTagList(key));
        case 10: return std::unique_ptr<NBTTagCompound> (new NBTTagCompound(key));
        default: return nullptr;
    }
}

std::string NBTBase::getTagName(uint8_t type) {
    switch (type) {
        case 0:  return "TAG_End";
        case 1:  return "TAG_Byte";
        case 2:  return "TAG_Short";
        case 3:  return "TAG_Int";
        case 4:  return "TAG_Long";
        case 5:  return "TAG_Float";
        case 6:  return "TAG_Double";
        case 7:  return "TAG_Byte_Array";
        case 8:  return "TAG_String";
        case 9:  return "TAG_List";
        case 10: return "TAG_Compound";
        default: return "UNKNOWN";
    }
}
