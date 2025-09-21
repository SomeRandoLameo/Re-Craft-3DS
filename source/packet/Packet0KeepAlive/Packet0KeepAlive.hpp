#include "../../dataIO/dataOutputStream/DataOutputStream.hpp"
#include "../../dataIO/dataInputStream/DataInputStream.hpp"

class Packet0KeepAlive {
public: 
    uint32_t randomId;

    void readPacketData(DataInputStream input);
    void writePacketData(DataOutputStream& output);

    int getPacketSize();

    bool debugPacket = false;
private:
    //TODO: Rewrite to set when utilized
    int packetID = 0;
};