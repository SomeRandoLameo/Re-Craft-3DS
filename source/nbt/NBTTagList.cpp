#include "NBTTagList.hpp"
#include "NBTBase.hpp"
#include <arpa/inet.h> // htonl / ntohl

NBTTagList::NBTTagList()
    : NBTBase(""), tagType(1) {}

NBTTagList::NBTTagList(const std::string& key)
    : NBTBase(key), tagType(1) {}

void NBTTagList::writeTagContents(std::ostream& out) const {
    uint8_t actualType = 1;
    if (!tagList.empty()) {
        actualType = tagList[0]->getType();
    }
    out.put(static_cast<char>(actualType));

    uint32_t sizeBE = htonl(static_cast<uint32_t>(tagList.size()));
    out.write(reinterpret_cast<const char*>(&sizeBE), sizeof(sizeBE));

    for (const auto& tag : tagList) {
        tag->writeTagContents(out);
    }
}

void NBTTagList::readTagContents(std::istream& in) {
    tagType = static_cast<uint8_t>(in.get());
    uint32_t sizeBE;
    in.read(reinterpret_cast<char*>(&sizeBE), sizeof(sizeBE));
    uint32_t count = ntohl(sizeBE);

    tagList.clear();
    for (uint32_t i = 0; i < count; ++i) {
        std::unique_ptr<NBTBase> tag = NBTBase::createTagOfType(tagType, "");
        tag->readTagContents(in);
        tagList.push_back(std::move(tag));
    }
}

uint8_t NBTTagList::getType() const {
    return 9;
}

std::string NBTTagList::toString() const {
    return std::to_string(tagList.size()) + " entries of type " + NBTBase::getTagName(tagType);
}

void NBTTagList::addTag(std::unique_ptr<NBTBase> tag) {
    tagType = tag->getType();
    tagList.push_back(std::move(tag));
}

NBTBase* NBTTagList::tagAt(size_t index) const {
    if (index >= tagList.size()) return nullptr;
    return tagList[index].get();
}

size_t NBTTagList::tagCount() const {
    return tagList.size();
}

std::unique_ptr<NBTBase> NBTTagList::cloneTag() const {
    // C++11 compatible
    std::unique_ptr<NBTTagList> clone(new NBTTagList(getKey()));
    clone->tagType = tagType;
    for (const auto& tag : tagList) {
        clone->tagList.push_back(tag->cloneTag());
    }
    return std::unique_ptr<NBTBase>(clone.release());
}

bool NBTTagList::equals(const NBTBase* other) const {
    if (!other) return false;
    if (!NBTBase::equals(other)) return false;

    if (other->getType() != getType()) return false;
    const NBTTagList* o = static_cast<const NBTTagList*>(other);

    if (tagType != o->tagType || tagList.size() != o->tagList.size())
        return false;

    for (size_t i = 0; i < tagList.size(); ++i) {
        if (!tagList[i]->equals(o->tagList[i].get())) return false;
    }
    return true;
}
