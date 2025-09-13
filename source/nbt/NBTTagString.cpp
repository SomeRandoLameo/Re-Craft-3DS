#include "NBTTagString.hpp"
#include <arpa/inet.h> // for htons/ntohs
#include <stdexcept>

NBTTagString::NBTTagString(const std::string& key)
    : NBTBase(key), stringValue("") {}

NBTTagString::NBTTagString(const std::string& key, const std::string& value)
    : NBTBase(key), stringValue(value) {
    if (value.empty()) {
        throw std::invalid_argument("Empty string not allowed");
    }
}

void NBTTagString::writeTagContents(std::ostream& out) const {
    uint16_t len = htons(static_cast<uint16_t>(stringValue.size()));
    out.write(reinterpret_cast<const char*>(&len), sizeof(len));
    out.write(stringValue.data(), stringValue.size());
}

void NBTTagString::readTagContents(std::istream& in) {
    uint16_t len;
    in.read(reinterpret_cast<char*>(&len), sizeof(len));
    len = ntohs(len);
    stringValue.resize(len);
    if (len > 0) {
        in.read(&stringValue[0], len);
    }
}

uint8_t NBTTagString::getType() const {
    return 8;
}

std::unique_ptr<NBTBase> NBTTagString::cloneTag() const {
    // C++11 compatible
    return std::unique_ptr<NBTBase>(new NBTTagString(getKey(), stringValue));
}

std::string NBTTagString::toString() const {
    return stringValue;
}

bool NBTTagString::equals(const NBTBase* other) const {
    if (!other) return false;
    if (!NBTBase::equals(other)) return false;

    // RTTI-free check
    if (other->getType() != getType()) return false;
    const NBTTagString* o = static_cast<const NBTTagString*>(other);
    return stringValue == o->stringValue;
}
