#include "Client.h"
#include "../util/Utility.h"

#include <iostream>

// These were changed in MSVC 2015. Redefine them so the old lib files link correctly.
// legacy_stdio_definitions.lib is supposed to define these but it doesn't seem to work.
#if (defined _MSC_VER) && _MSC_VER >= 1900
extern "C" { FILE __iob_func[3] = { *stdin,*stdout,*stderr }; }
int (WINAPIV * __vsnprintf)(char *, size_t, const char*, va_list) = _vsnprintf;
int (WINAPIV * _fprintf)(FILE* const, char const*, ...) = fprintf;
#endif

namespace mc {
    namespace core {

        Client::Client(protocol::packets::PacketDispatcher* dispatcher, protocol::Version version)
            : m_Dispatcher(dispatcher),
            m_Connection(m_Dispatcher, version),
            m_EntityManager(m_Dispatcher, version),
            m_PlayerManager(m_Dispatcher, &m_EntityManager),
            m_World(m_Dispatcher),
            m_PlayerController(std::make_unique<util::PlayerController>(&m_Connection, m_World, m_PlayerManager)),
            m_LastUpdate(0),
            m_Connected(false),
            m_InventoryManager(std::make_unique<inventory::InventoryManager>(m_Dispatcher, &m_Connection)),
            m_Hotbar(m_Dispatcher, &m_Connection, m_InventoryManager.get())
        {
            m_Connection.RegisterListener(this);
        }

        Client::~Client() {
            m_Connection.Disconnect();
            m_Connected = false;
            if (m_UpdateThread.joinable())
                m_UpdateThread.join();
            m_Connection.UnregisterListener(this);
        }

        void Client::OnSocketStateChange(network::Socket::Status newState) {
            m_Connected = (newState == network::Socket::Status::Connected);
        }

        void Client::Update() {
            try {
                std::cout << "attempting to create packet thread" << std::endl;
                m_Connection.CreatePacket();
                std::cout << "created packet thread" << std::endl;
            } catch (std::exception& e) {
                std::wcout << e.what() << std::endl;
            }
            std::cout << "creating player entity" << std::endl;
            entity::EntityPtr playerEntity = m_EntityManager.GetPlayerEntity();
            std::cout << "created player entity" << std::endl;
            if (playerEntity) {
                // Keep entity manager and player controller in sync
                std::cout << "setting player pos" << std::endl;
                playerEntity->SetPosition(m_PlayerController->GetPosition());
                std::cout << "set player pos" << std::endl;
            }

            s64 time = util::GetTime();
            if (time >= m_LastUpdate + (1000 / 20)) {
                std::cout << "updating player pos" << std::endl;
                m_PlayerController->Update();
                std::cout << "updated player pos" << std::endl;
                std::cout << "Notifying client tick listener" << std::endl;
                NotifyListeners(&ClientListener::OnTick);
                std::cout << "Notified client tick listener" << std::endl;

                m_LastUpdate = time;
            }
        }

        void Client::UpdateThread() {
            while (m_Connected) {
                Update();
                std::cout << "Updating Thread " << std::endl;
                //TODO: UNDEBUGIFY (this is supposed to be 1ms)
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        }

        bool Client::Login(const std::string& host, unsigned short port,
            const std::string& user, const std::string& password, UpdateMethod method)
        {
            if (m_UpdateThread.joinable()) {
                m_Connected = false;
                m_UpdateThread.join();
            }

            m_LastUpdate = 0;

            if (!m_Connection.Connect(host, port))
                throw std::runtime_error("Could not connect to server");

            if (!m_Connection.Login(user, password))
                return false;

            if (method == UpdateMethod::Threaded) {
                m_UpdateThread = std::thread(&Client::UpdateThread, this);
            } else if (method == UpdateMethod::Block) {
                UpdateThread();
            }
            return true;
        }

        bool Client::Login(const std::string& host, unsigned short port,
            const std::string& user, AuthToken token, UpdateMethod method)
        {
            if (m_UpdateThread.joinable()) {
                m_Connected = false;
                m_UpdateThread.join();
            }

            m_LastUpdate = 0;

            if (!m_Connection.Connect(host, port))
                throw std::runtime_error("Could not connect to server");

            if (!m_Connection.Login(user, token))
                return false;

            if (method == UpdateMethod::Threaded) {
                m_UpdateThread = std::thread(&Client::UpdateThread, this);
            } else if (method == UpdateMethod::Block) {
                UpdateThread();
            }
            return true;
        }

        void Client::Ping(const std::string& host, unsigned short port, UpdateMethod method) {
            std::cout << "starting update thread" << std::endl;
            if (m_UpdateThread.joinable()) {
                m_Connected = false;
                m_UpdateThread.join();
                std::cout << "started update thread" << std::endl;
            }

            std::cout << "starting server connection" << std::endl;
            if (!m_Connection.Connect(host, port)) {
                std::cout << "Could not connect to server" << std::endl;
                throw std::runtime_error("Could not connect to server");
            }

            std::cout << "started server connection" << std::endl;
            std::cout << "starting server ping" << std::endl;
            m_Connection.Ping();
            std::cout << "pinged server" << std::endl;

            if (method == UpdateMethod::Threaded) {
                std::cout << "starting Threaded update" << std::endl;
                m_UpdateThread = std::thread(&Client::UpdateThread, this);
            } else if (method == UpdateMethod::Block) {
                std::cout << "starting Blocking update" << std::endl;
                UpdateThread();
            }
        }

    } // ns core
} // ns mc
