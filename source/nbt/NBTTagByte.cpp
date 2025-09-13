#include "NBTTagByte.hpp"

NBTTagByte::NBTTagByte(const std::string& key)
    : NBTBase(key), byteValue(0) {}

NBTTagByte::NBTTagByte(const std::string& key, int8_t value)
    : NBTBase(key), byteValue(value) {}

void NBTTagByte::writeTagContents(std::ostream& out) const {
    out.write(reinterpret_cast<const char*>(&byteValue), sizeof(byteValue));
}

void NBTTagByte::readTagContents(std::istream& in) {
    in.read(reinterpret_cast<char*>(&byteValue), sizeof(byteValue));
}

uint8_t NBTTagByte::getType() const {
    return 1;
}

std::unique_ptr<NBTBase> NBTTagByte::cloneTag() const {
    return std::unique_ptr<NBTTagByte>(new NBTTagByte(getKey(), byteValue));
}

std::string NBTTagByte::toString() const {
    return std::to_string(byteValue);
}

bool NBTTagByte::equals(const NBTBase* other) const {
    if (!other) return false;
    if (getType() != other->getType()) return false;

    // Safe cast using getType() check
    const NBTTagByte* o = static_cast<const NBTTagByte*>(other);
    return byteValue == o->byteValue;
}
