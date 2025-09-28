#pragma once

#include "../mclib.h"
#include "AuthToken.h"
#include "Connection.h"
#include "PlayerManager.h"
#include "../entity/EntityManager.h"
#include "../inventory/Inventory.h"
#include "../inventory/Hotbar.h"
#include "../network/Network.h"
#include "../protocol/packets/PacketDispatcher.h"
#include "../util/ObserverSubject.h"
#include "../world/World.h"
#include <thread>

namespace mc {

    namespace util {

    class PlayerController;

    } // ns util

    namespace core {

    class ClientListener {
    public:
        virtual void OnTick() = 0;
    };

    enum class UpdateMethod {
        Block,
        Threaded,
        Manual
    };

    class Client : public util::ObserverSubject<ClientListener>, public core::ConnectionListener {
    private:
        protocol::packets::PacketDispatcher* m_Dispatcher;
        core::Connection m_Connection;
        entity::EntityManager m_EntityManager;
        core::PlayerManager m_PlayerManager;
        std::unique_ptr<inventory::InventoryManager> m_InventoryManager;
        inventory::Hotbar m_Hotbar;
        std::unique_ptr<util::PlayerController> m_PlayerController;
        world::World m_World;
        s64 m_LastUpdate;
        bool m_Connected;
        std::thread m_UpdateThread;

    public:
        Client(protocol::packets::PacketDispatcher* dispatcher, protocol::Version version = protocol::Version::Minecraft_1_11_2);
        ~Client();

        Client(const Client& rhs) = delete;
        Client& operator=(const Client& rhs) = delete;
        Client(Client&& rhs) = delete;
        Client& operator=(Client&& rhs) = delete;

        void OnSocketStateChange(network::Socket::Status newState);
        void UpdateThread();
        void Update();
        bool Login(const std::string& host, unsigned short port, const std::string& user, const std::string& password, UpdateMethod method = UpdateMethod::Block);
        bool Login(const std::string& host, unsigned short port, const std::string& user, AuthToken token, UpdateMethod method = UpdateMethod::Block);
        void Ping(const std::string& host, unsigned short port, UpdateMethod method = UpdateMethod::Block);

        protocol::packets::PacketDispatcher* GetDispatcher() { return m_Dispatcher; }
        core::Connection* GetConnection() { return &m_Connection; }
        core::PlayerManager* GetPlayerManager() { return &m_PlayerManager; }
        entity::EntityManager* GetEntityManager() { return &m_EntityManager; }
        inventory::InventoryManager* GetInventoryManager() { return m_InventoryManager.get(); }
        inventory::Hotbar& GetHotbar() { return m_Hotbar; }
        util::PlayerController* GetPlayerController() { return m_PlayerController.get(); }
        world::World* GetWorld() { return &m_World; }

    };

    } // ns core
} // ns mc