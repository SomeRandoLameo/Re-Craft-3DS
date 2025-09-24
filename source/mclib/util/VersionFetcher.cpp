#include "VersionFetcher.h"

#include "../common/Json.h"

#include <map>
#include <iostream>

namespace mc {
    namespace util {

        VersionFetcher::VersionFetcher(const std::string& host, u16 port)
            : m_Dispatcher(),
              m_Version(mc::protocol::Version::Minecraft_1_12_2),
              m_Forge(&m_Dispatcher, nullptr),
              m_Connection(nullptr),
              m_Host(host),
              m_Port(port),
              m_Found(false)
        {

        }

        void VersionFetcher::OnPingResponse(const json& node) {
            static const std::map<s32, mc::protocol::Version> mapping = {
                { 210, mc::protocol::Version::Minecraft_1_10_2 },
                { 315, mc::protocol::Version::Minecraft_1_11_0 },
                { 316, mc::protocol::Version::Minecraft_1_11_2 },
                { 335, mc::protocol::Version::Minecraft_1_12_0 },
                { 338, mc::protocol::Version::Minecraft_1_12_1 },
                { 340, mc::protocol::Version::Minecraft_1_12_2 },
                { 404, mc::protocol::Version::Minecraft_1_13_2 },
            };

            auto&& versionNode = node.value("version", json());

            if (versionNode.is_object()) {
                auto&& protocolNode = versionNode.value("protocol", json());

                if (protocolNode.is_number_integer()) {
                    s32 protocol = protocolNode.get<int>();

                    auto iter = mapping.lower_bound(protocol);
                    if (iter != mapping.end()) {
                        m_Version = iter->second;
                        m_Found = true;
                        m_Connection->Disconnect();
                    }
                }
            }

            m_Connection->Disconnect();
        }

        mc::protocol::Version VersionFetcher::GetVersion() {
            if (m_Found) return m_Version;

            std::cout << "Starting client" << std::endl;
            mc::core::Client m_Client(&m_Dispatcher);
            std::cout << "Started client" << std::endl;
            std::cout << "getting client connection" << std::endl;
            m_Connection = m_Client.GetConnection();
            std::cout << "gotten client connection" << std::endl;
            std::cout << "registering listener" << std::endl;
            m_Client.GetConnection()->RegisterListener(this);
            std::cout << "registered listener" << std::endl;
            std::cout << "pinging server" << std::endl;
            //TODO: Crash fix in here, probably socket crash, i dont know
            m_Client.Ping(m_Host, m_Port, mc::core::UpdateMethod::Block);

            std::cout << "pinged server" << std::endl;
            std::cout << "unregistering listener" << std::endl;
            m_Client.GetConnection()->UnregisterListener(this);
            std::cout << "unregistered listener" << std::endl;
            return m_Version;
        }

    } // ns util
} // ns mc
