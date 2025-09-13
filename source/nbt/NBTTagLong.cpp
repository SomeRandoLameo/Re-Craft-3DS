#include "NBTTagLong.hpp"
#include <arpa/inet.h> // for htonl/ntohl

// Helper for 64-bit host <-> network order
static inline uint64_t htonll(uint64_t value) {
#if __BYTE_ORDER == __LITTLE_ENDIAN
    return (static_cast<uint64_t>(htonl(value & 0xFFFFFFFFULL)) << 32) |
           htonl(static_cast<uint32_t>(value >> 32));
#else
    return value;
#endif
}

static inline uint64_t ntohll(uint64_t value) {
#if __BYTE_ORDER == __LITTLE_ENDIAN
    return (static_cast<uint64_t>(ntohl(value & 0xFFFFFFFFULL)) << 32) |
           ntohl(static_cast<uint32_t>(value >> 32));
#else
    return value;
#endif
}

NBTTagLong::NBTTagLong(const std::string& key)
    : NBTBase(key), longValue(0) {}

NBTTagLong::NBTTagLong(const std::string& key, int64_t value)
    : NBTBase(key), longValue(value) {}

void NBTTagLong::writeTagContents(std::ostream& out) const {
    int64_t networkValue = static_cast<int64_t>(htonll(static_cast<uint64_t>(longValue)));
    out.write(reinterpret_cast<const char*>(&networkValue), sizeof(networkValue));
}

void NBTTagLong::readTagContents(std::istream& in) {
    int64_t networkValue;
    in.read(reinterpret_cast<char*>(&networkValue), sizeof(networkValue));
    longValue = static_cast<int64_t>(ntohll(static_cast<uint64_t>(networkValue)));
}

uint8_t NBTTagLong::getType() const {
    return 4;
}

std::unique_ptr<NBTBase> NBTTagLong::cloneTag() const {
    // C++11 compatible
    return std::unique_ptr<NBTBase>(new NBTTagLong(getKey(), longValue));
}

std::string NBTTagLong::toString() const {
    return std::to_string(longValue);
}

bool NBTTagLong::equals(const NBTBase* other) const {
    if (!other) return false;
    if (!NBTBase::equals(other)) return false;

    // RTTI-free check
    if (other->getType() != getType()) return false;
    const NBTTagLong* o = static_cast<const NBTTagLong*>(other);
    return longValue == o->longValue;
}
