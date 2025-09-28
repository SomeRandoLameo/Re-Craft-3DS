#pragma once

#include <3ds.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#include "Socket.h"
#include "IPAddress.h"
#include "UDPSocket.h"
#include "TCPSocket.h"

namespace mc {
    namespace network {

        class Dns {
        public:
            static IPAddresses Resolve(const std::string& host);
        };

    } // ns network
} // ns mc

