#include "NBTTagCompound.hpp"
#include "NBTBase.hpp"

#include "NBTTagByte.hpp"
#include "NBTTagShort.hpp"
#include "NBTTagInt.hpp"
#include "NBTTagLong.hpp"
#include "NBTTagFloat.hpp"
#include "NBTTagDouble.hpp"
#include "NBTTagString.hpp"
#include "NBTTagByteArray.hpp"
#include "NBTTagList.hpp"


#include <stdexcept>

NBTTagCompound::NBTTagCompound() : NBTBase("") {}
NBTTagCompound::NBTTagCompound(const std::string& key) : NBTBase(key) {}

void NBTTagCompound::writeTagContents(std::ostream& out) const {
    for (auto it = tagMap.begin(); it != tagMap.end(); ++it) {
        const std::unique_ptr<NBTBase>& tag = it->second;

        NBTBase::writeTag(*tag, out); 
    }
    out.put(0); 
}

void NBTTagCompound::readTagContents(std::istream& in) {
    tagMap.clear();
    while (true) {
        std::unique_ptr<NBTBase> tag = NBTBase::readTag(in);
        if (tag->getType() == 0) break; 
        tagMap[tag->getKey()] = std::move(tag);
    }
}

uint8_t NBTTagCompound::getType() const {
    return 10;
}

std::unique_ptr<NBTBase> NBTTagCompound::cloneTag() const {

    std::unique_ptr<NBTTagCompound> clone(new NBTTagCompound(getKey()));

    for (const auto& pair : tagMap) {
        const std::string& key = pair.first;
        const std::unique_ptr<NBTBase>& tag = pair.second;
        clone->tagMap[key] = tag->cloneTag();
    }

    return std::move(clone);
}

std::string NBTTagCompound::toString() const {
    return std::to_string(tagMap.size()) + " entries";
}

void NBTTagCompound::setTag(const std::string& key, std::unique_ptr<NBTBase> tag) {
    tag->setKey(key);
    tagMap[key] = std::move(tag);
}

void NBTTagCompound::setByte(const std::string& key, uint8_t value) {
    setTag(key, std::unique_ptr<NBTTagByte>(new NBTTagByte(key, value)));

}

void NBTTagCompound::setShort(const std::string& key, int16_t value) {
    setTag(key, std::unique_ptr<NBTTagShort>(new NBTTagShort(key, value)));
}

void NBTTagCompound::setInteger(const std::string& key, int32_t value) {
    setTag(key, std::unique_ptr<NBTTagInt>(new NBTTagInt(key, value)));
}

void NBTTagCompound::setLong(const std::string& key, int64_t value) {
    setTag(key, std::unique_ptr<NBTTagLong>(new NBTTagLong(key, value)));
}

void NBTTagCompound::setFloat(const std::string& key, float value) {
    setTag(key, std::unique_ptr<NBTTagFloat>(new NBTTagFloat(key, value)));
}

void NBTTagCompound::setDouble(const std::string& key, double value) {
    setTag(key, std::unique_ptr<NBTTagDouble>(new NBTTagDouble(key, value)));
}

void NBTTagCompound::setString(const std::string& key, const std::string& value) {
    setTag(key, std::unique_ptr<NBTTagString>(new NBTTagString(key, value)));
}

void NBTTagCompound::setByteArray(const std::string& key, const std::vector<uint8_t>& value) {
    setTag(key, std::unique_ptr<NBTTagByteArray>(new NBTTagByteArray(key, value)));
}

void NBTTagCompound::setCompoundTag(const std::string& key, std::unique_ptr<NBTTagCompound> value) {
    value->setKey(key);
    setTag(key, std::move(value));
}

void NBTTagCompound::setBoolean(const std::string& key, bool value) {
    setByte(key, value ? 1 : 0);
}

NBTBase* NBTTagCompound::getTag(const std::string& key) const {
    auto it = tagMap.find(key);
    return it != tagMap.end() ? it->second.get() : nullptr;
}

bool NBTTagCompound::hasKey(const std::string& key) const {
    return tagMap.find(key) != tagMap.end();
}

uint8_t NBTTagCompound::getByte(const std::string& key) const {
    NBTBase* tag = getTag(key);
    if (tag && tag->getType() == 1) { 
        return static_cast<NBTTagByte*>(tag)->byteValue;
    }
    return 0;
}

int16_t NBTTagCompound::getShort(const std::string& key) const {
    NBTBase* tag = getTag(key);
    return (tag && tag->getType() == 2) ? static_cast<NBTTagShort*>(tag)->shortValue : 0;
}

int32_t NBTTagCompound::getInteger(const std::string& key) const {
    NBTBase* tag = getTag(key);
    return (tag && tag->getType() == 3) ? static_cast<NBTTagInt*>(tag)->intValue : 0;
}

int64_t NBTTagCompound::getLong(const std::string& key) const {
    NBTBase* tag = getTag(key);
    return (tag && tag->getType() == 4) ? static_cast<NBTTagLong*>(tag)->longValue : 0;
}

float NBTTagCompound::getFloat(const std::string& key) const {
    NBTBase* tag = getTag(key);
    return (tag && tag->getType() == 5) ? static_cast<NBTTagFloat*>(tag)->floatValue : 0.0f;
}

double NBTTagCompound::getDouble(const std::string& key) const {
    NBTBase* tag = getTag(key);
    return (tag && tag->getType() == 6) ? static_cast<NBTTagDouble*>(tag)->doubleValue : 0.0;
}

std::string NBTTagCompound::getString(const std::string& key) const {
    NBTBase* tag = getTag(key);
    return (tag && tag->getType() == 8) ? static_cast<NBTTagString*>(tag)->stringValue : "";
}

std::vector<uint8_t> NBTTagCompound::getByteArray(const std::string& key) const {
    NBTBase* tag = getTag(key);
    return (tag && tag->getType() == 7) ? static_cast<NBTTagByteArray*>(tag)->byteArray : std::vector<uint8_t>{};
}

NBTTagCompound* NBTTagCompound::getCompoundTag(const std::string& key) const {
    NBTBase* tag = getTag(key);
    return (tag && tag->getType() == 10) ? static_cast<NBTTagCompound*>(tag) : nullptr;
}

NBTTagList* NBTTagCompound::getTagList(const std::string& key) const {
    NBTBase* tag = getTag(key);
    return (tag && tag->getType() == 9) ? static_cast<NBTTagList*>(tag) : nullptr;
}

bool NBTTagCompound::getBoolean(const std::string& key) const {
    return getByte(key) != 0;
}

bool NBTTagCompound::equals(const NBTBase* other) const {
    if (!other || !NBTBase::equals(other)) return false;

    if (other->getType() != 10) return false;
    const NBTTagCompound* o = static_cast<const NBTTagCompound*>(other);

    if (tagMap.size() != o->tagMap.size()) return false;

    for (auto it1 = tagMap.begin(); it1 != tagMap.end(); ++it1) {
        const std::string& key = it1->first;
        const NBTBase* tag = it1->second.get();

        auto it2 = o->tagMap.find(key);
        if (it2 == o->tagMap.end() || !tag->equals(it2->second.get())) return false;
    }

    return true;
}

