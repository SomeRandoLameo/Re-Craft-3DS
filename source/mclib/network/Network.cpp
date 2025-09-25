// Network.cpp - 3DS Implementation
#include "Network.h"
#include <3ds.h>
#include <malloc.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

namespace mc {
    namespace network {

        class NetworkInitializer {
        private:
            static u32* SOC_buffer;
            static bool initialized;
        public:
            MCLIB_API NetworkInitializer();
            MCLIB_API ~NetworkInitializer();

            NetworkInitializer(const NetworkInitializer& rhs) = delete;
            NetworkInitializer& operator=(const NetworkInitializer& rhs) = delete;
        };

        u32* NetworkInitializer::SOC_buffer = nullptr;
        bool NetworkInitializer::initialized = false;

        NetworkInitializer::NetworkInitializer() {
            if (!initialized) {
                // Allocate buffer for SOC service (0x100000 bytes recommended)
                SOC_buffer = (u32*)memalign(0x1000, 0x100000);
                if (SOC_buffer) {
                    // Initialize SOC service
                    Result ret = socInit(SOC_buffer, 0x100000);
                    if (R_SUCCEEDED(ret)) {
                        initialized = true;
                    }
                }
            }
        }

        NetworkInitializer::~NetworkInitializer() {
            if (initialized) {
                socExit();
                if (SOC_buffer) {
                    free(SOC_buffer);
                    SOC_buffer = nullptr;
                }
                initialized = false;
            }
        }

        NetworkInitializer initializer;

        IPAddresses Dns::Resolve(const std::string& host) {
            IPAddresses list;
            struct addrinfo hints = { 0 };
            struct addrinfo* addresses = nullptr;

            hints.ai_family = AF_INET;
            hints.ai_socktype = SOCK_STREAM;
            hints.ai_protocol = IPPROTO_TCP;

            int ret = getaddrinfo(host.c_str(), nullptr, &hints, &addresses);
            if (ret != 0) {
                return list; // Return empty list on failure
            }

            for (struct addrinfo* p = addresses; p != nullptr; p = p->ai_next) {
                if (p->ai_family == AF_INET) {
                    struct sockaddr_in* sockaddr_ipv4 = (struct sockaddr_in*)p->ai_addr;
                    char straddr[INET_ADDRSTRLEN];

                    if (inet_ntop(AF_INET, &(sockaddr_ipv4->sin_addr), straddr, INET_ADDRSTRLEN)) {
                        list.push_back(IPAddress(straddr));
                    }
                }
            }

            if (addresses) {
                freeaddrinfo(addresses);
            }

            return list;
        }

    } // ns network
} // ns mc