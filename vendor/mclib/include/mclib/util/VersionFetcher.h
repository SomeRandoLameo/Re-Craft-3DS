#pragma once

#include "../common/Json.h"
#include "../core/Client.h"
#include "Forge.h"

namespace mc {
    namespace util {

    class VersionFetcher : public mc::core::ConnectionListener {
    private:
        mc::protocol::packets::PacketDispatcher m_Dispatcher;
        mc::protocol::Version m_Version;
        mc::util::ForgeHandler m_Forge;
        mc::core::Connection* m_Connection;
        std::string m_Host;
        u16 m_Port;
        bool m_Found;

    public:
        VersionFetcher(const std::string& host, u16 port);

        mc::protocol::Version GetVersion();
        void OnPingResponse(const json& node) override;

        mc::util::ForgeHandler& GetForge() { return m_Forge; }
    };

    } // ns util
} // ns mc