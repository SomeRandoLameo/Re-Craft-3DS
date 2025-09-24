#pragma once
#include "../../utils/thread/MiniThread.hpp"
#include "ServerList.hpp"
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>

struct ServerPollRequest {
    size_t serverIndex;
    ServerNBTStorage* serverStorage;
    bool isComplete = false;
    ServerInfo result;
};

class ServerPollingThread : public MiniThread {
private:
    std::queue<std::shared_ptr<ServerPollRequest>> requestQueue;
    std::queue<std::shared_ptr<ServerPollRequest>> completedQueue;
    LightLock queueMutex;
    LightLock completedMutex;
    LightEvent newRequestEvent;
    ServerList* serverList;

public:
    ServerPollingThread(ServerList* list);
    virtual ~ServerPollingThread();

    // Queue a single server for polling
    std::shared_ptr<ServerPollRequest> pollServerAsync(size_t serverIndex, ServerNBTStorage* storage);
    
    // Queue all servers for polling
    void pollAllServersAsync();
    
    // Check for completed polls and update servers
    void processCompletedPolls();
    
    // Get number of pending requests
    size_t getPendingCount();
    
    // Clear all pending requests
    void clearRequests();

protected:
    virtual void run() override;

private:
    void processRequest(std::shared_ptr<ServerPollRequest> request);
};