#include "ServerPollingThread.hpp"
#include <iostream>

ServerPollingThread::ServerPollingThread(ServerList* list) : serverList(list) {
    LightLock_Init(&queueMutex);
    LightLock_Init(&completedMutex);
    LightEvent_Init(&newRequestEvent, RESET_ONESHOT);
}

ServerPollingThread::~ServerPollingThread() {
    stop();
    join();
    
    clearRequests();
    
    LightEvent_Clear(&newRequestEvent);
}

std::shared_ptr<ServerPollRequest> ServerPollingThread::pollServerAsync(size_t serverIndex, ServerNBTStorage* storage) {
    if (!storage || !running) {
        return nullptr;
    }
    
    auto request = std::make_shared<ServerPollRequest>();
    request->serverIndex = serverIndex;
    request->serverStorage = storage;
    request->isComplete = false;
    
    LightLock_Lock(&queueMutex);
    requestQueue.push(request);
    LightLock_Unlock(&queueMutex);
    
    // Signal the worker thread that there's a new request
    LightEvent_Signal(&newRequestEvent);
    
    return request;
}

void ServerPollingThread::pollAllServersAsync() {
    if (!serverList || !running) {
        return;
    }
    
    const auto& servers = serverList->getServers();
    
    // Clear any existing requests first
    clearRequests();
    
    // Queue all servers for polling
    for (size_t i = 0; i < servers.size(); ++i) {
        // Note: We need a non-const reference, so we'll access through serverList
        auto& mutableServers = const_cast<std::vector<ServerNBTStorage>&>(servers);
        pollServerAsync(i, &mutableServers[i]);
    }
}

void ServerPollingThread::processCompletedPolls() {
    LightLock_Lock(&completedMutex);
    
    while (!completedQueue.empty()) {
        auto request = completedQueue.front();
        completedQueue.pop();
        
        // Update the server storage with the poll results
        if (request->serverStorage) {
            if (request->result.success) {
                request->serverStorage->motd = request->result.motd;
                request->serverStorage->playerCount = request->result.playerCount;
            } else {
                request->serverStorage->motd = "";
                request->serverStorage->playerCount = "???";
            }
        }
    }
    
    LightLock_Unlock(&completedMutex);
}

size_t ServerPollingThread::getPendingCount() {
    LightLock_Lock(&queueMutex);
    size_t count = requestQueue.size();
    LightLock_Unlock(&queueMutex);
    
    LightLock_Lock(&completedMutex);
    count += completedQueue.size();
    LightLock_Unlock(&completedMutex);
    
    return count;
}

void ServerPollingThread::clearRequests() {
    LightLock_Lock(&queueMutex);
    while (!requestQueue.empty()) {
        requestQueue.pop();
    }
    LightLock_Unlock(&queueMutex);
    
    LightLock_Lock(&completedMutex);
    while (!completedQueue.empty()) {
        completedQueue.pop();
    }
    LightLock_Unlock(&completedMutex);
}

void ServerPollingThread::run() {
    std::cout << "Server polling thread started\n";
    
    while (running) {

        LightEvent_WaitTimeout(&newRequestEvent, 1000000000LL); // 1 second in nanoseconds
        
        if (!running) {
            break;
        }
        
        // Process all pending requests
        std::shared_ptr<ServerPollRequest> request;
        
        while (running) {
            // Get next request from queue
            LightLock_Lock(&queueMutex);
            if (requestQueue.empty()) {
                LightLock_Unlock(&queueMutex);
                break;
            }
            
            request = requestQueue.front();
            requestQueue.pop();
            LightLock_Unlock(&queueMutex);
            
            // Process the request
            if (request) {
                processRequest(request);
                
                // Add to completed queue
                LightLock_Lock(&completedMutex);
                completedQueue.push(request);
                LightLock_Unlock(&completedMutex);
            }
        }
    }
    
    std::cout << "Server polling thread stopped\n";
}

void ServerPollingThread::processRequest(std::shared_ptr<ServerPollRequest> request) {
    if (!request || !request->serverStorage || !serverList) {
        return;
    }
    
    try {
    
        ServerNBTStorage tempStorage = *(request->serverStorage);
        request->result = serverList->pollServer(tempStorage);
        request->isComplete = true;
        
        // Debug output
        if (request->result.success) {
            std::cout << "Poll successful for " << request->serverStorage->name 
                      << ": " << request->result.playerCount << "\n";
        } else {
            std::cout << "Poll failed for " << request->serverStorage->name << "\n";
        }
        
    } catch (...) {
        std::cout << "Exception during server poll for " << request->serverStorage->name << "\n";
        request->result.success = false;
        request->isComplete = true;
    }
}