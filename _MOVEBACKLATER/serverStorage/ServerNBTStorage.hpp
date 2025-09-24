#pragma once
#include <string>
#include "../nbt/NBTTagCompound.hpp"

class ServerNBTStorage {
public:
    std::string name;
    std::string host;
    std::string playerCount;
    std::string motd;
    long long lag;
    bool polled;

    ServerNBTStorage(const std::string& name, const std::string& host);

    NBTTagCompound getCompoundTag() const;
    static ServerNBTStorage createServerNBTStorage(const NBTTagCompound& tag);
};
