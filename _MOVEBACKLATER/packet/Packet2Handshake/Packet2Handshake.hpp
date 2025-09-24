#include "../../dataIO/dataOutputStream/DataOutputStream.hpp"
#include "../../dataIO/dataInputStream/DataInputStream.hpp"

class Packet2Handshake {
public: 
    //TODO: These values have been set by hand. No idea where minecraft sets them. not my problem atm.
    std::string username = "MISSINGNAME";

	void readPacketData(DataInputStream var1);

	void writePacketData(DataOutputStream& var1);

	int getPacketSize();

    bool debugPacket = false;
private:
    //TODO: Rewrite to set when utilized
    int packetID = 2;
};
