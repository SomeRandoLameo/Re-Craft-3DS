#include "ServerNBTStorage.hpp"

// Constructor
ServerNBTStorage::ServerNBTStorage(const std::string& name, const std::string& host)
    : name(name), host(host), playerCount(""), motd(""), lag(0), polled(false) {}

// Serialize
NBTTagCompound ServerNBTStorage::getCompoundTag() const {
    NBTTagCompound tag;
    tag.setString("name", name);
    tag.setString("ip", host);
    return tag;
}

// Deserialize
ServerNBTStorage ServerNBTStorage::createServerNBTStorage(const NBTTagCompound& tag) {
    return ServerNBTStorage(tag.getString("name"), tag.getString("ip"));
}
