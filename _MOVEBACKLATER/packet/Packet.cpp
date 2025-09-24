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
