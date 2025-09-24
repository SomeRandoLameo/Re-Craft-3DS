#pragma once

#include "../../mclib/common/Common.h"
#include "../../dataIO/dataInputStream/DataInputStream.hpp"

class Packet0KeepAlive {
public: 
    u32 randomId;

    void readPacketData(DataInputStream input);
    mc::DataBuffer writePacketData();

    int getPacketSize();

    bool debugPacket = false;
private:
    //TODO: Rewrite to set when utilized
    u8 packetID = 0x00;
};