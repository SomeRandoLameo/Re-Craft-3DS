#pragma once

#include "../../common/DataBuffer.h"
#include "../Protocol.h"
#include "Packet.h"

namespace mc {

    namespace core {
        class Connection;
    } // ns core

    namespace protocol {
        namespace packets {

            class PacketFactory {
            public:
                static Packet* CreatePacket(Protocol& protocol, State state, DataBuffer data, std::size_t length, core::Connection* connection = nullptr);
                static void FreePacket(Packet* packet);
            };

        } // ns packets
    } // ns protocol
} // ns mc