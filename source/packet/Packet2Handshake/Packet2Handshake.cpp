#include <iostream>

#include "Packet2Handshake.hpp"

void Packet2Handshake::readPacketData(DataInputStream input){
    //Packet ID should already be read as 0
	this->username = input.readString(32);
}

void Packet2Handshake::writePacketData(DataOutputStream& output){
    output.writeByte(  this->packetID);        // Packet ID should ideally be set before, for now this is enough

    output.writeString(this->username);        // username
}

int Packet2Handshake::getPacketSize() {
    return 4                                  // protocolVersion (int)
        + static_cast<int>(this->username.length()) // username length
        + 4;                                   // 1 int padding
                                       
}

