#include "../../dataIO/dataOutputStream/DataOutputStream.hpp"
#include "../../dataIO/dataInputStream/DataInputStream.hpp"

class Packet3Chat {
public: 
    std::string message;

	
/*
    TODO: Implement sending once proper packet handling is ready
	public Packet3Chat(String var1) {
		if(var1.length() > 119) {
			var1 = var1.substring(0, 119);
		}

		this.message = var1;
	}
*/
	void readPacketData(DataInputStream var1);

	void writePacketData(DataOutputStream& var1);
/*
	void processPacket(NetHandler var1) {
		var1.handleChat(this);
	}
*/
	int getPacketSize();
private:
    //TODO: Rewrite to set when utilized
    int packetID = 3;
};
