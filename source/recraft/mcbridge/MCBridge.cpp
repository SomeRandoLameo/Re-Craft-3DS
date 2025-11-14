#include "mcbridge/MCBridge.h"
#include <iostream>

MCBridge::MCBridge()
        : m_running(false)
        , m_connected(false)
        , m_shouldStop(false) {
}

MCBridge::~MCBridge() {
    stopBackgroundThread();
    disconnect();
}

bool MCBridge::connect() {
    if (m_connected.load()) {
        std::cout << "Already connected\n";
        return true;
    }

    const std::string server("192.168.2.176");
    const u16 port = 25565;
    const std::string username("Nintendo3DS");
    const std::string password("");

    try {
        mc::util::VersionFetcher versionFetcher(server, port);
        auto version = versionFetcher.GetVersion();
        mc::block::BlockRegistry::GetInstance()->RegisterVanillaBlocks(version);

        std::cout << "Connected successfully to server\n";

        LightLock_Lock(&m_clientMutex);

        m_dispatcher = std::make_unique<mc::protocol::packets::PacketDispatcher>();
        m_client = std::make_unique<mc::core::Client>(m_dispatcher.get(), version);

        versionFetcher.GetForge().SetConnection(m_client->GetConnection());

        m_client->GetPlayerController()->SetHandleFall(true);
        m_client->GetConnection()->GetSettings()
                .SetMainHand(mc::MainHand::Right)
                .SetViewDistance(static_cast<s32>(4));

        mc::core::AuthToken token;
        m_client->Login(server, port, username, password, mc::core::UpdateMethod::Manual);

        LightLock_Unlock(&m_clientMutex);

        m_connected.store(true);
        return true;

    } catch (std::exception& e) {
        std::cout << "Connection error: " << e.what() << std::endl;
        LightLock_Lock(&m_clientMutex);
        m_dispatcher.reset();
        m_client.reset();
        LightLock_Unlock(&m_clientMutex);
        m_connected.store(false);
        return false;
    }
}

void MCBridge::disconnect() {
    m_connected.store(false);
    LightLock_Lock(&m_clientMutex);
    m_client.reset();
    m_dispatcher.reset();
    LightLock_Unlock(&m_clientMutex);
    std::cout << "Disconnected from server\n";
}

void MCBridge::update() {
    LightLock_Lock(&m_clientMutex);
    if (m_client && m_connected.load()) {
        try {
            m_client->Update();
        } catch (std::exception& e) {
            std::cout << "Update error: " << e.what() << std::endl;
            m_connected.store(false);
        }
    }
    LightLock_Unlock(&m_clientMutex);
}

void MCBridge::startBackgroundThread() {
    if (m_running.load()) {
        std::cout << "Background thread already running\n";
        return;
    }

    m_shouldStop.store(false);

    m_threadStack = linearAlloc(THREAD_STACK_SIZE);
    if (!m_threadStack) {
        std::cout << "Failed to allocate thread stack\n";
        return;
    }

    m_thread = threadCreate(threadFunc, this,
                            THREAD_STACK_SIZE,
                            THREAD_PRIORITY,
                            THREAD_CORE,
                            false);

    if (m_thread == nullptr) {
        std::cout << "Failed to create thread\n";
        linearFree(m_threadStack);
        m_threadStack = nullptr;
        return;
    }

    m_running.store(true);
    std::cout << "Background thread started\n";
}

void MCBridge::stopBackgroundThread() {
    if (!m_running.load()) {
        return;
    }

    m_shouldStop.store(true);

    if (m_thread) {
        threadJoin(m_thread, U64_MAX);
        threadFree(m_thread);
        m_thread = nullptr;
    }

    if (m_threadStack) {
        linearFree(m_threadStack);
        m_threadStack = nullptr;
    }

    m_running.store(false);
    std::cout << "Background thread stopped\n";
}

void MCBridge::threadFunc(void* arg) {
    MCBridge* bridge = static_cast<MCBridge*>(arg);
    bridge->backgroundLoop();
}

void MCBridge::backgroundLoop() {
    std::cout << "Background loop started\n";

    while (!m_shouldStop.load()) {
        if (m_connected.load()) {
            update();
        }
        svcSleepThread(50000000LL); // 50ms
    }

    std::cout << "Background loop ended\n";
}


mc::inventory::Slot MCBridge::CTItemStackToMCLIBSlot(ItemStack in){
    return mc::inventory::Slot(in.block,in.amount,in.meta);
}

ItemStack MCBridge::MCLIBSlotToCTItemStack(mc::inventory::Slot in){
    return (ItemStack){in.GetItemId(),in.GetItemDamage(),in.GetItemCount()};
}