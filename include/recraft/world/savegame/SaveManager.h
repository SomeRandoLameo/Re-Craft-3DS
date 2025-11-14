#pragma once

#include <cstdio>
extern "C" {
#include <vec/vec.h>
}
#include "../../entity/Player.h"
#include "SuperChunk.h"

class SaveManager {
public:
    SaveManager();
    ~SaveManager();

    // Delete copy constructor and assignment operator
    SaveManager(const SaveManager&) = delete;
    SaveManager& operator=(const SaveManager&) = delete;

    static void InitFileSystem();

    void Init(Player* player);
    void Deinit();

    void Load(char* path);
    void Unload();

    void LoadChunk(WorkQueue* queue, WorkerItem item);
    void SaveChunk(WorkQueue* queue, WorkerItem item);

    // Static wrappers for WorkQueue callbacks
    static void LoadChunkCallback(WorkQueue* queue, WorkerItem item, void* context);
    static void SaveChunkCallback(WorkQueue* queue, WorkerItem item, void* context);

private:
    Player* player;
    World* world;
    vec_t(SuperChunk*) superchunks;

    SuperChunk* FetchSuperChunk(int x, int z);
};