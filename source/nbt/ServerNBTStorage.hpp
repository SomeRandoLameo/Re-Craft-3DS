#pragma once
#include <string>
#include "NBTTagCompound.hpp"

class ServerNBTStorage {
public:
    // Fields
    std::string name;
    std::string host;
    std::string playerCount;
    std::string motd;
    long lag;
    bool polled;

    // Constructor
    ServerNBTStorage(const std::string& name, const std::string& host);

    // Serialize to NBT
    NBTTagCompound getCompoundTag() const;

    // Deserialize from NBT
    static ServerNBTStorage createServerNBTStorage(const NBTTagCompound& tag);
};
