#include "NBTTagCompound.hpp"

void NBTTagCompound::setString(const std::string& key, const std::string& value) {
    data[key] = value;
}

std::string NBTTagCompound::getString(const std::string& key) const {
    auto it = data.find(key);
    return it != data.end() ? it->second : "";
}
