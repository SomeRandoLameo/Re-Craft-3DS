#include "NBTTagEnd.hpp"

NBTTagEnd::NBTTagEnd()
    : NBTBase("") {}

void NBTTagEnd::readTagContents(std::istream& /*in*/) {
    // Nothing to read for TAG_End
}

void NBTTagEnd::writeTagContents(std::ostream& /*out*/) const {
    // Nothing to write for TAG_End
}

uint8_t NBTTagEnd::getType() const {
    return 0;
}

std::unique_ptr<NBTBase> NBTTagEnd::cloneTag() const {
    // C++11 version without make_unique
    return std::unique_ptr<NBTBase>(new NBTTagEnd());
}

std::string NBTTagEnd::toString() const {
    return "END";
}

bool NBTTagEnd::equals(const NBTBase* other) const {
    return NBTBase::equals(other);
}
