#include <iostream>

#include "Packet255KickDisconnect.hpp"

void Packet255KickDisconnect::readPacketData(DataInputStream input){
    //Packet ID should already be read as 0
	this->reason = input.readString(256);
}

void Packet255KickDisconnect::writePacketData(DataOutputStream& output){
    output.writeByte(this->packetID);        // Packet ID should ideally be set before, for now this is enough

    output.writeString(this->reason);   
}

int Packet255KickDisconnect::getPacketSize() {
    //hmmm, no byte ahead this time... lets try this anyways
    return static_cast<int>(this->reason.length()); // reason length
                                
}

