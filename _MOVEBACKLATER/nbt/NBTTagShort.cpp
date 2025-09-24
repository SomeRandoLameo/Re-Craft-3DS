#include "NBTTagShort.hpp"
#include <arpa/inet.h> // for htons/ntohs

NBTTagShort::NBTTagShort(const std::string& key)
    : NBTBase(key), shortValue(0) {}

NBTTagShort::NBTTagShort(const std::string& key, int16_t value)
    : NBTBase(key), shortValue(value) {}

void NBTTagShort::writeTagContents(std::ostream& out) const {
    int16_t networkValue = htons(shortValue);
    out.write(reinterpret_cast<const char*>(&networkValue), sizeof(networkValue));
}

void NBTTagShort::readTagContents(std::istream& in) {
    int16_t networkValue;
    in.read(reinterpret_cast<char*>(&networkValue), sizeof(networkValue));
    shortValue = ntohs(networkValue);
}

uint8_t NBTTagShort::getType() const {
    return 2;
}

std::unique_ptr<NBTBase> NBTTagShort::cloneTag() const {
    // C++11 compatible
    return std::unique_ptr<NBTBase>(new NBTTagShort(getKey(), shortValue));
}

std::string NBTTagShort::toString() const {
    return std::to_string(shortValue);
}

bool NBTTagShort::equals(const NBTBase* other) const {
    if (!other) return false;
    if (!NBTBase::equals(other)) return false;

    // RTTI-free check
    if (other->getType() != getType()) return false;
    const NBTTagShort* o = static_cast<const NBTTagShort*>(other);
    return shortValue == o->shortValue;
}
