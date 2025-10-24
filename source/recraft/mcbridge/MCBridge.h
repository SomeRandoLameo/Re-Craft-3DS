#pragma once

#include "../../mclib/common/Common.h"
#include "../../mclib/core/Client.h"
#include "../../mclib/util/Forge.h"
#include "../../mclib/util/Hash.h"
#include "../../mclib/util/Utility.h"
#include "../../mclib/util/VersionFetcher.h"
#include <memory>
#include <iostream>
#include <atomic>
#include <functional>
#include <3ds.h>

class MCBridge {
public:
    MCBridge();
    ~MCBridge();

    bool connect();
    void disconnect();
    void update();


    void startBackgroundThread();
    void stopBackgroundThread();
    bool isConnected() const { return m_connected.load(); }
    bool isRunning() const { return m_running.load(); }

    //clanker suggested whatever the heck this is
    template<typename Func>
    bool withClient(Func&& func) {
        LightLock_Lock(&m_clientMutex);
        bool result = false;
        if (m_client && m_connected.load()) {
            try {
                func(m_client.get(), m_dispatcher.get());
                result = true;
            } catch (std::exception& e) {
                std::cout << "Client operation error: " << e.what() << std::endl;
            }
        }
        LightLock_Unlock(&m_clientMutex);
        return result;
    }

    mc::core::Client* getClient() { return m_client.get(); }
    void lockClient() { LightLock_Lock(&m_clientMutex); }
    void unlockClient() { LightLock_Unlock(&m_clientMutex); }

private:
    static void threadFunc(void* arg);
    void backgroundLoop();

    std::unique_ptr<mc::protocol::packets::PacketDispatcher> m_dispatcher;
    std::unique_ptr<mc::core::Client> m_client;

    Thread m_thread;
    void* m_threadStack;
    LightLock m_clientMutex;

    std::atomic<bool> m_running;
    std::atomic<bool> m_connected;
    std::atomic<bool> m_shouldStop;

    static constexpr size_t THREAD_STACK_SIZE = 64 * 1024;
    static constexpr int THREAD_PRIORITY = 0x30;
    static constexpr int THREAD_CORE = -2;
};