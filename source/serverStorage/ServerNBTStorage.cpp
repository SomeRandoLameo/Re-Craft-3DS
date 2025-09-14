#include "ServerNBTStorage.hpp"
#include "../nbt/NBTTagCompound.hpp"
#include "../nbt/NBTTagByte.hpp"

ServerNBTStorage::ServerNBTStorage(const std::string& name, const std::string& host)
    : name(name), 
    host(host), 
    polled(false) {}

NBTTagCompound ServerNBTStorage::getCompoundTag() const {
    NBTTagCompound tag;
    tag.setString("name", name);
    tag.setString("ip", host);
    return tag;
}

ServerNBTStorage ServerNBTStorage::createServerNBTStorage(const NBTTagCompound& tag) {
    ServerNBTStorage storage(
        tag.getString("name"),
        tag.getString("ip")
    );
    return storage;
}
