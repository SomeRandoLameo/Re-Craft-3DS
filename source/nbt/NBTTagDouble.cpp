#include "NBTTagDouble.hpp"
#include <cstring>     // memcpy
#include <arpa/inet.h>

// endian helpers for 64-bit
static inline uint64_t htonll(uint64_t value) {
#if __BYTE_ORDER == __LITTLE_ENDIAN
    return (((uint64_t)htonl(value & 0xFFFFFFFFULL)) << 32) | htonl(value >> 32);
#else
    return value;
#endif
}

static inline uint64_t ntohll(uint64_t value) {
#if __BYTE_ORDER == __LITTLE_ENDIAN
    return (((uint64_t)ntohl(value & 0xFFFFFFFFULL)) << 32) | ntohl(value >> 32);
#else
    return value;
#endif
}

NBTTagDouble::NBTTagDouble(const std::string& key)
    : NBTBase(key), doubleValue(0.0) {}

NBTTagDouble::NBTTagDouble(const std::string& key, double value)
    : NBTBase(key), doubleValue(value) {}

void NBTTagDouble::writeTagContents(std::ostream& out) const {
    uint64_t raw;
    std::memcpy(&raw, &doubleValue, sizeof(raw));
    raw = htonll(raw);
    out.write(reinterpret_cast<const char*>(&raw), sizeof(raw));
}

void NBTTagDouble::readTagContents(std::istream& in) {
    uint64_t raw;
    in.read(reinterpret_cast<char*>(&raw), sizeof(raw));
    raw = ntohll(raw);
    std::memcpy(&doubleValue, &raw, sizeof(raw));
}

uint8_t NBTTagDouble::getType() const {
    return 6;
}

std::unique_ptr<NBTBase> NBTTagDouble::cloneTag() const {
    return std::unique_ptr<NBTBase>(new NBTTagDouble(getKey(), doubleValue));
}

std::string NBTTagDouble::toString() const {
    return std::to_string(doubleValue);
}

bool NBTTagDouble::equals(const NBTBase* other) const {
    if (!other || !NBTBase::equals(other)) return false;
    if (other->getType() != 6) return false; // 6 = double
    const NBTTagDouble* o = static_cast<const NBTTagDouble*>(other);
    return this->doubleValue == o->doubleValue;
}

