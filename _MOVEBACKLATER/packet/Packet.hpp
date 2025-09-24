#pragma once

#include <cstdint>
#include <map>
#include <set>
#include <functional>
#include <memory>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <chrono>
#include <sstream>
#include <cstring>

// Forward declarations
// class NetHandler; // not implemented yet
class NBTTagCompound;

class Packet {
public:
    using PacketFactory = std::function<Packet*()>;

    // Registry maps
    static std::map<int, PacketFactory> packetIdToFactoryMap;
    static std::map<std::string, int> packetClassToIdMap;
    static std::set<int> clientPacketIdList;
    static std::set<int> serverPacketIdList;

    // Instance properties
    const long long creationTimeMillis;
    bool isChunkDataPacket = false;

    Packet();
    virtual ~Packet() {}

    // Abstract interface
    virtual void readPacketData(std::istream& in) const = 0;
    virtual void writePacketData(std::ostream& out) const = 0;
    // virtual void processPacket(NetHandler* handler) = 0; // commented until NetHandler exists
    virtual int getPacketSize() const = 0;
    virtual const char* getClassName() const = 0;


    // ID functions
    int getPacketId() const;
    static void addIdClassMapping(int id, bool client, bool server, const std::string& className, PacketFactory factory);
    static std::unique_ptr<Packet> getNewPacket(int id);

    // Serialization helpers
    static std::unique_ptr<Packet> readPacket(std::istream& in, bool isServer);
    static void writePacket(const Packet& packet, std::ostream& out);

protected:
    // NBT helpers
    NBTTagCompound* readNBTPacket(std::istream& in);
    void writeNBTPacket(NBTTagCompound* tag, std::ostream& out);

};
