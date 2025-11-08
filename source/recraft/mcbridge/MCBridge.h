#pragma once
//MCLIB includes
#include "../../mclib/common/Common.h"
#include "../../mclib/core/Client.h"
#include "../../mclib/util/Forge.h"
#include "../../mclib/util/Hash.h"
#include "../../mclib/util/Utility.h"
#include "../../mclib/util/VersionFetcher.h"

//Craftus includes
#include "../inventory/ItemStack.h"
#include "../world/CT_World.h"  // Add this include for World type
#include "../world/T_World.h"   // Add this for terra::WorldListener

#include <memory>
#include <iostream>
#include <atomic>
#include <functional>
#include <unordered_map>
#include <3ds.h>

// Forward declarations
class MCBridge;

// Declare the listener class BEFORE MCBridge
class MCBridgeWorldListener : public terra::WorldListener {
public:
    MCBridgeWorldListener(World* ctWorld, MCBridge* bridge)
            : m_ctWorld(ctWorld), m_bridge(bridge) {}

    void OnChunkLoad(terra::ChunkPtr chunk, const terra::ChunkColumnMetadata& meta, u16 yIndex) override;
    void OnBlockChange(mc::Vector3i position, mc::block::BlockPtr newBlock, mc::block::BlockPtr oldBlock) override;

private:
    World* m_ctWorld;
    MCBridge* m_bridge;
};

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

    // Sync methods
    void SyncChunkToCT(World* ctWorld, int chunkX, int chunkZ);
    Block MCLIBBlockToCT(u32 mcBlockId);
    void SyncBlockChangeToCT(World* ctWorld, mc::Vector3i pos, mc::block::BlockPtr block);

    void RegisterWorldListener(World* ctWorld);
    void UnregisterWorldListener();

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

    mc::inventory::Slot CTItemStackToMCLIBSlot(ItemStack in);
    ItemStack MCLIBSlotToCTItemStack(mc::inventory::Slot in);

private:
    static void threadFunc(void* arg);
    void backgroundLoop();
    void InitBlockMapping();

    std::unique_ptr<mc::protocol::packets::PacketDispatcher> m_dispatcher;
    std::unique_ptr<mc::core::Client> m_client;

    Thread m_thread;
    void* m_threadStack;
    LightLock m_clientMutex;

    std::atomic<bool> m_running;
    std::atomic<bool> m_connected;
    std::atomic<bool> m_shouldStop;

    // Block mapping
    std::unordered_map<u32, Block> m_blockMapping;

    // World listener
    std::unique_ptr<MCBridgeWorldListener> m_worldListener;
    World* m_syncedWorld;

    static constexpr size_t THREAD_STACK_SIZE = 64 * 1024;
    static constexpr int THREAD_PRIORITY = 0x30;
    static constexpr int THREAD_CORE = -2;
};