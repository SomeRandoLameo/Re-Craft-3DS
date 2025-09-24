#include "Packet3Chat.hpp"

void Packet3Chat::readPacketData(DataInputStream input) {
	 //Packet ID should already be read as 0
    this->message = input.readString(119);
}

void Packet3Chat::writePacketData(DataOutputStream& output) {
	output.writeByte(this->packetID);        // Packet ID should ideally be set before, for now this is enough

    output.writeString(this->message);
}

	
int Packet3Chat::getPacketSize() {
	return 2 + static_cast<int>(this->message.length()) * 2;
}

