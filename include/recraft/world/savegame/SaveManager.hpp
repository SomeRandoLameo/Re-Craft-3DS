#pragma once

#include "SuperChunk.hpp"
#include "entity/Player.hpp"
#include "world/WorkQueue.hpp"

class SaveManager {
public:
    SaveManager()  = default;
    ~SaveManager() = default;

    // Delete copy constructor and assignment operator
    SaveManager(const SaveManager&)            = delete;
    SaveManager& operator=(const SaveManager&) = delete;

    static void InitFileSystem();

    void Init(Player* player, World* world);

    void Load(char* path);
    void Unload();

    void LoadChunk(WorkQueue* queue, WorkerItem item);
    void SaveChunk(WorkQueue* queue, WorkerItem item);

    // Static wrappers for WorkQueue callbacks
    static void LoadChunkCallback(WorkQueue* queue, WorkerItem item, void* context);
    static void SaveChunkCallback(WorkQueue* queue, WorkerItem item, void* context);

private:
    Player*                  m_player = nullptr;
    World*                   m_world  = nullptr;
    std::vector<SuperChunk*> m_superchunks;

    SuperChunk* FetchSuperChunk(int x, int z);
};
