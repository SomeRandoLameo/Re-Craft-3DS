#pragma once
#include <string>
#include <unordered_map>

class NBTTagCompound {
public:
    NBTTagCompound() = default;

    void setString(const std::string& key, const std::string& value);
    std::string getString(const std::string& key) const;

private:
    std::unordered_map<std::string, std::string> data;
};
