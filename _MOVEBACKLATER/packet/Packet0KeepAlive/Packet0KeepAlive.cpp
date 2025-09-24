#include <iostream>

#include "Packet0KeepAlive.hpp"

void Packet0KeepAlive::readPacketData(DataInputStream input){
    //Packet ID should already be read as 0
    uint32_t randInt = input.readInt();

    if(this->debugPacket) std::cout << "Packet input: "<< randInt << std::endl;

    this->randomId = randInt;
}

mc::DataBuffer Packet0KeepAlive::writePacketData(){
    mc::DataBuffer buffer;
    buffer << this->packetID;           // Packet ID should ideally be set before, for now this is enough
    buffer << this->randomId;

    if(this->debugPacket) std::cout << "Packet output: "<< this->randomId << std::endl;
    return buffer;
}

int Packet0KeepAlive::getPacketSize(){
    // 4 bytes for randomId, packetID doesnt count here
    return 4;
}
