#include "../../dataIO/dataOutputStream/DataOutputStream.hpp"
#include "../../dataIO/dataInputStream/DataInputStream.hpp"

class Packet1Login {
public: 
    //TODO: These values have been set by hand. No idea where minecraft sets them. not my problem atm.
    uint32_t protocolVersion = 23;
	std::string username;
	uint64_t mapSeed = 0;
	//EnumWorldType field_46032_d;  TODO: IMPLEMENT ONCE WORLD TYPES EXIST
	uint32_t serverMode = 0;
	uint8_t worldType = 0;
	uint8_t difficultySetting = 0;
	uint8_t worldHeight = 0;
	uint8_t maxPlayers = 0;

    void readPacketData(DataInputStream input);
    void writePacketData(DataOutputStream& output);

    int getPacketSize();

    bool debugPacket = false;
private:
    //TODO: Rewrite to set when utilized
    int packetID = 1;
};