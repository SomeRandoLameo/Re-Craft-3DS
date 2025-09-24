#include "CompressedStreamTools.hpp"
#include "NBTBase.hpp"
#include "miniz.h"
#include <vector>
#include <fstream>
#include <stdexcept>
#include <cstdint>
#include <cstring>
#include <sstream> // Only for temporary std::istringstream / std::ostringstream

//------------------ Memory adapter ------------------//
// Wrap memory buffer as std::istream
class MemoryIStream : public std::istringstream {
public:
    MemoryIStream(const std::vector<uint8_t>& data)
        : std::istringstream(std::string(data.begin(), data.end()), std::ios::binary) {}
};

// Wrap memory buffer as std::ostream
class MemoryOStream : public std::ostringstream {
public:
    MemoryOStream() : std::ostringstream(std::ios::binary) {}
    std::vector<uint8_t> getData() const {
        std::string str = this->str();
        return std::vector<uint8_t>(str.begin(), str.end());
    }
};

//------------------ CompressedStreamTools ------------------//

NBTTagCompound* CompressedStreamTools::readFromMemory(const uint8_t* data, size_t size) {
    std::vector<uint8_t> buffer(data, data + size);
    MemoryIStream in(buffer);
    std::unique_ptr<NBTBase> baseTag = NBTBase::readTag(in);
    NBTTagCompound* compound = static_cast<NBTTagCompound*>(baseTag.release());
    if (!compound) throw std::runtime_error("Root tag must be a named compound tag");
    return compound;
}

std::vector<uint8_t> CompressedStreamTools::writeToMemory(NBTTagCompound* nbt) {
    MemoryOStream out;
    NBTBase::writeTag(*nbt, out);
    return out.getData();
}

NBTTagCompound* CompressedStreamTools::loadGzippedCompoundFromMemory(const std::vector<uint8_t>& data) {
    mz_ulong decompressedSize = 1024 * 1024; // 1 MB
    std::vector<uint8_t> buffer(decompressedSize);
    int ret = mz_uncompress(buffer.data(), &decompressedSize, data.data(), data.size());
    if (ret != MZ_OK) throw std::runtime_error("Failed to decompress NBT data");
    buffer.resize(decompressedSize);
    return readFromMemory(buffer.data(), buffer.size());
}

std::vector<uint8_t> CompressedStreamTools::writeMapToGzippedMemory(NBTTagCompound* nbt) {
    std::vector<uint8_t> uncompressed = writeToMemory(nbt);
    mz_ulong compressedSize = mz_compressBound(uncompressed.size());
    std::vector<uint8_t> compressed(compressedSize);
    int ret = mz_compress2(compressed.data(), &compressedSize,
                           uncompressed.data(), uncompressed.size(),
                           MZ_BEST_COMPRESSION);
    if (ret != MZ_OK) throw std::runtime_error("Failed to compress NBT data");
    compressed.resize(compressedSize);
    return compressed;
}

void CompressedStreamTools::saveMapToFileWithBackup(NBTTagCompound* nbt, const std::string& filename) {
    std::string tmp = filename + "_tmp";
    saveMapToFile(nbt, tmp);
    std::remove(filename.c_str());
    if (std::rename(tmp.c_str(), filename.c_str()) != 0)
        throw std::runtime_error("Failed to rename temporary NBT file");
}

void CompressedStreamTools::saveMapToFile(NBTTagCompound* nbt, const std::string& filename) {
    std::vector<uint8_t> data = writeToMemory(nbt);
    std::ofstream out(filename.c_str(), std::ios::binary);
    if (!out) throw std::runtime_error("Failed to open file for writing");
    out.write(reinterpret_cast<const char*>(data.data()), data.size());
}

NBTTagCompound* CompressedStreamTools::readMapFromFile(const std::string& filename) {
    std::ifstream in(filename.c_str(), std::ios::binary | std::ios::ate);
    if (!in) throw std::runtime_error("File not found");

    size_t size = in.tellg();
    in.seekg(0);
    std::vector<uint8_t> buffer(size);
    in.read(reinterpret_cast<char*>(buffer.data()), size);
    return readFromMemory(buffer.data(), buffer.size());
}

NBTTagCompound* CompressedStreamTools::loadMapFromByteArray(const std::vector<uint8_t>& data) {
    return loadGzippedCompoundFromMemory(data);
}

std::vector<uint8_t> CompressedStreamTools::writeMapToByteArray(NBTTagCompound* nbt) {
    return writeMapToGzippedMemory(nbt);
}
