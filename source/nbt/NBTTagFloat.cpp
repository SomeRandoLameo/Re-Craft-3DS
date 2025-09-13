#include "NBTTagFloat.hpp"
#include <arpa/inet.h> // for htonl/ntohl
#include <cstring>     // for memcpy

NBTTagFloat::NBTTagFloat(const std::string& key)
    : NBTBase(key), floatValue(0.0f) {}

NBTTagFloat::NBTTagFloat(const std::string& key, float value)
    : NBTBase(key), floatValue(value) {}

void NBTTagFloat::writeTagContents(std::ostream& out) const {
    uint32_t raw;
    std::memcpy(&raw, &floatValue, sizeof(raw));
    raw = htonl(raw);
    out.write(reinterpret_cast<const char*>(&raw), sizeof(raw));
}

void NBTTagFloat::readTagContents(std::istream& in) {
    uint32_t raw;
    in.read(reinterpret_cast<char*>(&raw), sizeof(raw));
    raw = ntohl(raw);
    std::memcpy(&floatValue, &raw, sizeof(raw));
}

uint8_t NBTTagFloat::getType() const {
    return 5;
}

std::unique_ptr<NBTBase> NBTTagFloat::cloneTag() const {
    // C++11 compatible
    return std::unique_ptr<NBTBase>(new NBTTagFloat(getKey(), floatValue));
}

std::string NBTTagFloat::toString() const {
    return std::to_string(floatValue);
}

bool NBTTagFloat::equals(const NBTBase* other) const {
    if (!other) return false;
    if (!NBTBase::equals(other)) return false;

    // Use getType() instead of dynamic_cast
    if (other->getType() != getType()) return false;
    const NBTTagFloat* o = static_cast<const NBTTagFloat*>(other);
    return floatValue == o->floatValue;
}
