#include "Packet.hpp"
#include "../nbt/NBTTagCompound.hpp"
#include "../nbt/CompressedStreamTools.hpp"

// Static maps
std::map<int, Packet::PacketFactory> Packet::packetIdToFactoryMap;
std::map<std::string, int> Packet::packetClassToIdMap;
std::set<int> Packet::clientPacketIdList;
std::set<int> Packet::serverPacketIdList;

Packet::Packet()
    : creationTimeMillis(std::chrono::system_clock::now().time_since_epoch() /
                         std::chrono::milliseconds(1)) {}

int Packet::getPacketId() const {
    auto it = packetClassToIdMap.find(this->getClassName());
    if (it == packetClassToIdMap.end()) {
        throw std::runtime_error("Unknown packet class ID");
    }
    return it->second;
}


void Packet::addIdClassMapping(int id, bool client, bool server,
                               const std::string& className, PacketFactory factory) {
    if (packetIdToFactoryMap.count(id)) {
        throw std::invalid_argument("Duplicate packet id: " + std::to_string(id));
    }
    if (packetClassToIdMap.count(className)) {
        throw std::invalid_argument("Duplicate packet class: " + className);
    }
    packetIdToFactoryMap[id] = factory;
    packetClassToIdMap[className] = id;
    if (client) clientPacketIdList.insert(id);
    if (server) serverPacketIdList.insert(id);
}

std::unique_ptr<Packet> Packet::getNewPacket(int id) {
    auto it = packetIdToFactoryMap.find(id);
    if (it == packetIdToFactoryMap.end()) return nullptr;
    return std::unique_ptr<Packet>(it->second());
}

std::unique_ptr<Packet> Packet::readPacket(std::istream& in, bool isServer) {
    int id = in.get();
    if (id == EOF) return nullptr;

    if (isServer && !serverPacketIdList.count(id)) {
        throw std::runtime_error("Bad packet id " + std::to_string(id));
    }
    if (!isServer && !clientPacketIdList.count(id)) {
        throw std::runtime_error("Bad packet id " + std::to_string(id));
    }

    auto packet = getNewPacket(id);
    if (!packet) throw std::runtime_error("Unknown packet id " + std::to_string(id));

    packet->readPacketData(in);
    return packet;
}

void Packet::writePacket(const Packet& packet, std::ostream& out) {
    out.put(static_cast<char>(packet.getPacketId()));
    packet.writePacketData(out);
}

void Packet::writeString(const std::string& str, std::ostream& out) {
    if (str.size() > 32767) throw std::runtime_error("String too big");

    std::vector<uint8_t> utf16Data;
    for (unsigned char c : str) {
        if (c < 0x80) {
            utf16Data.push_back(0);
            utf16Data.push_back(c);
        } else {
            // For simplicity, encode non-ASCII as '?'
            utf16Data.push_back(0);
            utf16Data.push_back('?');
        }
    }

    uint16_t len = static_cast<uint16_t>(str.size());
    
    uint8_t lenBytes[2] = { static_cast<uint8_t>((len >> 8) & 0xFF),
                             static_cast<uint8_t>(len & 0xFF) };
    out.write(reinterpret_cast<char*>(lenBytes), 2);
    
    out.write(reinterpret_cast<char*>(utf16Data.data()), utf16Data.size());
}


std::string Packet::readString(std::istream& in, int maxLen) {
  
    uint8_t lengthBytes[2];
    in.read(reinterpret_cast<char*>(lengthBytes), 2);
    if (in.gcount() != 2) {
        throw std::runtime_error("Failed to read string length");
    }

    int strLen = (lengthBytes[0] << 8) | lengthBytes[1];
    if (strLen > maxLen || strLen < 0) {
        throw std::runtime_error("Invalid string length");
    }

    // --- Read UTF-16BE data (2 bytes per char) ---
    std::vector<uint8_t> utf16Data(strLen * 2);
    in.read(reinterpret_cast<char*>(utf16Data.data()), strLen * 2);
    if (in.gcount() != strLen * 2) {
        throw std::runtime_error("Failed to read string data");
    }

    // --- Convert UTF-16BE → UTF-8 ---
    char utf8Buffer[1024];
    utf16be_to_utf8(utf16Data.data(), strLen * 2, utf8Buffer, sizeof(utf8Buffer));

    return std::string(utf8Buffer);
}

NBTTagCompound* Packet::readNBTPacket(std::istream& in) {
    int16_t len;
    in.read(reinterpret_cast<char*>(&len), sizeof(len));
    if (len < 0) return nullptr;

    std::vector<uint8_t> data(len);
    in.read(reinterpret_cast<char*>(data.data()), len);
    return CompressedStreamTools::loadMapFromByteArray(data);
}

void Packet::writeNBTPacket(NBTTagCompound* tag, std::ostream& out) {
    if (!tag) {
        int16_t neg = -1;
        out.write(reinterpret_cast<char*>(&neg), sizeof(neg));
    } else {
        auto bytes = CompressedStreamTools::writeMapToByteArray(tag);
        int16_t len = static_cast<int16_t>(bytes.size());
        out.write(reinterpret_cast<char*>(&len), sizeof(len));
        out.write(reinterpret_cast<char*>(bytes.data()), len);
    }
}

int Packet::utf16be_to_utf8(const uint8_t *in, int inLen, char *out, int outSize) {
    int i = 0, o = 0;
    while (i + 1 < inLen && o + 4 < outSize) {
        uint16_t code = (in[i] << 8) | in[i+1];
        i += 2;

        if (code < 0x80) {
            out[o++] = static_cast<char>(code);
        } else if (code < 0x800) {
            out[o++] = static_cast<char>(0xC0 | (code >> 6));
            out[o++] = static_cast<char>(0x80 | (code & 0x3F));
        } else {
            out[o++] = static_cast<char>(0xE0 | (code >> 12));
            out[o++] = static_cast<char>(0x80 | ((code >> 6) & 0x3F));
            out[o++] = static_cast<char>(0x80 | (code & 0x3F));
        }
    }
    out[o] = '\0';
    return o;
}

/*
// ItemStack handling (commented out until implemented)
ItemStack* Packet::readItemStack(std::istream& in) { return nullptr; }
void Packet::writeItemStack(ItemStack* stack, std::ostream& out) {}
*/
