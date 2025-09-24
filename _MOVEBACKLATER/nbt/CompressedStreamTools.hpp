#pragma once
#include <string>
#include <vector>
#include "NBTTagCompound.hpp"

class CompressedStreamTools {
public:
    // Memory-based NBT
    static NBTTagCompound* loadGzippedCompoundFromMemory(const std::vector<uint8_t>& data);
    static std::vector<uint8_t> writeMapToGzippedMemory(NBTTagCompound* nbt);

    // Raw gzipped helpers (Java parity)
    static NBTTagCompound* loadMapFromByteArray(const std::vector<uint8_t>& data);
    static std::vector<uint8_t> writeMapToByteArray(NBTTagCompound* nbt);

    // File-based
    static void saveMapToFileWithBackup(NBTTagCompound* nbt, const std::string& filename);
    static void saveMapToFile(NBTTagCompound* nbt, const std::string& filename);
    static NBTTagCompound* readMapFromFile(const std::string& filename);

    // Raw memory (uncompressed)
    static NBTTagCompound* readFromMemory(const uint8_t* data, size_t size);
    static std::vector<uint8_t> writeToMemory(NBTTagCompound* nbt);
};
