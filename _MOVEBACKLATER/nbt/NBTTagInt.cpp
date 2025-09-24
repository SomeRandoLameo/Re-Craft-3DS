#include "NBTTagInt.hpp"
#include <arpa/inet.h> // for htonl/ntohl

NBTTagInt::NBTTagInt(const std::string& key)
    : NBTBase(key), intValue(0) {}

NBTTagInt::NBTTagInt(const std::string& key, int32_t value)
    : NBTBase(key), intValue(value) {}

void NBTTagInt::writeTagContents(std::ostream& out) const {
    int32_t networkValue = htonl(intValue);
    out.write(reinterpret_cast<const char*>(&networkValue), sizeof(networkValue));
}

void NBTTagInt::readTagContents(std::istream& in) {
    int32_t networkValue;
    in.read(reinterpret_cast<char*>(&networkValue), sizeof(networkValue));
    intValue = ntohl(networkValue);
}

uint8_t NBTTagInt::getType() const {
    return 3;
}

std::unique_ptr<NBTBase> NBTTagInt::cloneTag() const {
    // C++11 compatible
    return std::unique_ptr<NBTBase>(new NBTTagInt(getKey(), intValue));
}

std::string NBTTagInt::toString() const {
    return std::to_string(intValue);
}

bool NBTTagInt::equals(const NBTBase* other) const {
    if (!other) return false;
    if (!NBTBase::equals(other)) return false;

    // RTTI-free check
    if (other->getType() != getType()) return false;
    const NBTTagInt* o = static_cast<const NBTTagInt*>(other);
    return intValue == o->intValue;
}
