#include "Network.h"
#include <malloc.h>

namespace mc {
    namespace network {

        class NetworkInitializer {
        private:
            u32* m_SOCBuffer;
        public:
            MCLIB_API NetworkInitializer();
            MCLIB_API ~NetworkInitializer();

            NetworkInitializer(const NetworkInitializer& rhs) = delete;
            NetworkInitializer& operator=(const NetworkInitializer& rhs) = delete;
        };

        NetworkInitializer::NetworkInitializer()
                : m_SOCBuffer(nullptr)
        {
            m_SOCBuffer = (u32*)memalign(0x1000, 0x100000);
            if (m_SOCBuffer) {
                Result ret = socInit(m_SOCBuffer, 0x100000);
                if (R_FAILED(ret)) {
                    free(m_SOCBuffer);
                    m_SOCBuffer = nullptr;
                }
            }
        }

        NetworkInitializer::~NetworkInitializer() {
            if (m_SOCBuffer) {
                socExit();
                free(m_SOCBuffer);
                m_SOCBuffer = nullptr;
            }
        }

        NetworkInitializer initializer;

        IPAddresses Dns::Resolve(const std::string& host) {
            IPAddresses list;
            addrinfo hints = { 0 }, *addresses = nullptr;

            hints.ai_family = AF_INET;
            hints.ai_socktype = SOCK_STREAM;
            hints.ai_protocol = IPPROTO_TCP;

            if (getaddrinfo(host.c_str(), NULL, &hints, &addresses) != 0)
                return list;

            for (addrinfo *p = addresses; p != NULL; p = p->ai_next) {
                char straddr[512];
                inet_ntop(p->ai_family, &((sockaddr_in*)p->ai_addr)->sin_addr, straddr, sizeof(straddr));
                list.push_back(IPAddress(straddr));
            }

            if (addresses)
                freeaddrinfo(addresses);

            return list;
        }

    } // ns network
} // ns mc