#pragma once

#include "../common/DataBuffer.h"
#include "../common/Json.h"
#include "../common/Types.h"
#include "AuthToken.h"
#include "ClientSettings.h"
#include "Compression.h"
#include "Encryption.h"
#include "../network/Socket.h"
#include "../protocol/Protocol.h"
#include "../protocol/packets/Packet.h"
#include "../protocol/packets/PacketHandler.h"
#include "../util/ObserverSubject.h"
#include "../util/Yggdrasil.h"

#include <string>
#include <queue>
#include <future>
#include <memory>

namespace mc {
    namespace core {

        class ConnectionListener {
        public:
            virtual ~ConnectionListener() { }

            virtual void OnSocketStateChange(network::Socket::Status newStatus) { }
            virtual void OnLogin(bool success) { }
            virtual void OnAuthentication(bool success, std::string error) { }
            virtual void OnPingResponse(const nlohmann::json& node) { }
        };

        class Connection : public protocol::packets::PacketHandler, public util::ObserverSubject<ConnectionListener> {
        private:
            std::unique_ptr<EncryptionStrategy> m_Encrypter;
            std::unique_ptr<CompressionStrategy> m_Compressor;
            std::unique_ptr<network::Socket> m_Socket;
            std::unique_ptr<util::Yggdrasil> m_Yggdrasil;
            ClientSettings m_ClientSettings;
            std::string m_Server;
            std::string m_Email;
            std::string m_Username;
            std::string m_Password;
            DataBuffer m_HandleBuffer;
            protocol::Protocol& m_Protocol;
            protocol::State m_ProtocolState;
            u16 m_Port;
            bool m_SentSettings;
            s32 m_Dimension;

            void AuthenticateClient(const std::wstring& serverId, const std::string& sharedSecret, const std::string& pubkey);
            protocol::packets::Packet* CreatePacket(DataBuffer& buffer);
            void SendSettingsPacket();

        public:
            Connection(protocol::packets::PacketDispatcher* dispatcher, protocol::Version version = protocol::Version::Minecraft_1_11_2);
            ~Connection();

            Connection(const Connection& other) = delete;
            Connection& operator=(const Connection& rhs) = delete;
            Connection(Connection&& other) = delete;
            Connection& operator=(Connection&& rhs) = delete;

            util::Yggdrasil* GetYggdrasil() { return m_Yggdrasil.get(); }
            network::Socket::Status GetSocketState() const;
            ClientSettings& GetSettings() noexcept { return m_ClientSettings; }
            s32 GetDimension() const noexcept { return m_Dimension; }
            protocol::State GetProtocolState() const { return m_ProtocolState; }

            void SendSettings() noexcept { m_SentSettings = false; }

            void HandlePacket(protocol::packets::in::KeepAlivePacket* packet);
            void HandlePacket(protocol::packets::in::PlayerPositionAndLookPacket* packet);
            void HandlePacket(protocol::packets::in::DisconnectPacket* packet);
            void HandlePacket(protocol::packets::in::EncryptionRequestPacket* packet);
            void HandlePacket(protocol::packets::in::LoginSuccessPacket* packet);
            void HandlePacket(protocol::packets::in::SetCompressionPacket* packet);
            void HandlePacket(protocol::packets::in::JoinGamePacket* packet);
            void HandlePacket(protocol::packets::in::RespawnPacket* packet);
            void HandlePacket(protocol::packets::in::UpdateHealthPacket* packet);
            void HandlePacket(protocol::packets::in::status::ResponsePacket* packet);

            bool Connect(const std::string& server, u16 port);
            void Disconnect();
            void CreatePacket();

            void Ping();
            bool Login(const std::string& username, const std::string& password);
            bool Login(const std::string& username, AuthToken token);

            template <typename T>
            void SendPacket(T&& packet) {
                s32 id = m_Protocol.GetPacketId(packet);
                packet.SetId(id);
                packet.SetProtocolVersion(m_Protocol.GetVersion());
                DataBuffer packetBuffer = packet.Serialize();
                DataBuffer compressed = m_Compressor->Compress(packetBuffer);
                DataBuffer encrypted = m_Encrypter->Encrypt(compressed);

                m_Socket->Send(encrypted);
            }

            template <typename T>
            void SendPacket(T* packet) {
                SendPacket(*packet);
            }
        };

    } // ns core
} // ns mc
