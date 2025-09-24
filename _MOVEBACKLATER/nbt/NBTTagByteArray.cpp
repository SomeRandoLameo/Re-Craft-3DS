#include "NBTTagByteArray.hpp"
#include <arpa/inet.h>   // htonl / ntohl
#include <algorithm>     // std::equal

NBTTagByteArray::NBTTagByteArray(const std::string& key)
    : NBTBase(key) {}

NBTTagByteArray::NBTTagByteArray(const std::string& key, const std::vector<uint8_t>& data)
    : NBTBase(key), byteArray(data) {}

void NBTTagByteArray::writeTagContents(std::ostream& out) const {
    int32_t len = htonl(static_cast<int32_t>(byteArray.size()));
    out.write(reinterpret_cast<const char*>(&len), sizeof(len));
    if (!byteArray.empty()) {
        out.write(reinterpret_cast<const char*>(byteArray.data()), byteArray.size());
    }
}

void NBTTagByteArray::readTagContents(std::istream& in) {
    int32_t len;
    in.read(reinterpret_cast<char*>(&len), sizeof(len));
    len = ntohl(len);
    byteArray.resize(len);
    if (len > 0) {
        in.read(reinterpret_cast<char*>(byteArray.data()), len);
    }
}

uint8_t NBTTagByteArray::getType() const {
    return 7;
}

std::unique_ptr<NBTBase> NBTTagByteArray::cloneTag() const {
    return std::unique_ptr<NBTTagByteArray>(new NBTTagByteArray(getKey(), byteArray));
}

std::string NBTTagByteArray::toString() const {
    return "[" + std::to_string(byteArray.size()) + " bytes]";
}

bool NBTTagByteArray::equals(const NBTBase* other) const {
    if (!other) return false;
    if (!NBTBase::equals(other)) return false;

    const auto* o = static_cast<const NBTTagByteArray*>(other);
    return o && (this->byteArray == o->byteArray);
}
