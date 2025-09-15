#pragma once
#include <vector>
#include <string>
#include <memory>
#include <3ds.h>
#include "../../serverStorage/ServerNBTStorage.hpp"
#include "../../nbt/NBTTagCompound.hpp"

// Forward declaration
class ServerPollingThread;

struct ServerInfo {
    std::string motd;
    int onlinePlayers = -1;
    int maxPlayers = -1;
    std::string playerCount;
    bool success = false;
};

class ServerList {
private:
    std::vector<ServerNBTStorage> servers;
    size_t selectedServer = 0;
    NBTTagCompound* rootTag = nullptr;
    std::unique_ptr<ServerPollingThread> pollingThread;
    bool isPolling = false;

    void load();

public:
    ServerList();
    ~ServerList();

    void initGui();

    void save();

    void addServer(const std::string& name, const std::string& host);
    void deleteSelectedServer();
    
    ServerInfo pollServer(ServerNBTStorage& storage);
    
    // Asynchronous polling methods
    void pollAllAsync();
    void pollSelectedAsync();
    void updateAsyncPolls(); // Call this regularly to process completed polls
    bool isAsyncPolling() const { return isPolling; }
    size_t getPendingPollCount() const;
    void cancelAllPolls();

    void drawScreen() const;
    void moveSelectionUp();
    void moveSelectionDown();
    void updateControlls(u32 kDown);
    size_t getSelectedIndex() const { return selectedServer; }
    const std::vector<ServerNBTStorage>& getServers() const { return servers; }
    std::vector<ServerNBTStorage>& getServers() { return servers; } // Non-const version for thread access
};