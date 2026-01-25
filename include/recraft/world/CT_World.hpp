#pragma once

#include "misc/Xorshift.hpp"
#include "world/CT_Chunk.hpp"
#include "world/WorldConf.hpp"


#define UNDEADCHUNKS_COUNT (2 * World::ChunkCacheSize + World::ChunkCacheSize * World::ChunkCacheSize)

class ChunkWorker;
class SaveManager;
class WorkQueue;

class World {
public:
    constexpr static int Height         = 256;
    constexpr static int ChunkCacheSize = 9;
    constexpr static int ChunkPoolSize  = World::ChunkCacheSize * World::ChunkCacheSize + UNDEADCHUNKS_COUNT;
    constexpr static int NameSize       = 12;


    World(WorkQueue* workqueue);
    ~World() = default;

    void Reset();
    void Tick();

    ChunkColumnPtr LoadChunk(int x, int z);
    void           UnloadChunk(ChunkColumnPtr chunk);

    ChunkColumnPtr GetChunkColumn(int x, int z);

    mc::block::BlockEntityPtr GetBlockEntity(mc::Vector3i pos);

    Block GetBlock(mc::Vector3i position);
    void  SetBlock(mc::Vector3i position, Block block);
    u8    GetMetadata(mc::Vector3i position);
    void  SetMetadata(mc::Vector3i position, u8 metadata);

    void SetBlockAndMeta(mc::Vector3i position, Block block, u8 metadata);

    void UpdateChunkCache(int originX, int originZ);

    int GetHeight(int x, int z);

    // Public member access (if needed)
    int                      GetHighestBlock() const { return m_HighestBlock; }
    const char*              GetName() const { return name; }
    const GeneratorSettings& GetGenSettings() const { return genSettings; }
    int                      GetWeather() const { return m_weather; }
    void                     SetWeather(int newWeather) { m_weather = newWeather; }

    bool online = false;

    int               cacheTranslationX, cacheTranslationZ;
    GeneratorSettings genSettings;

    ChunkColumnPtr columnCache[ChunkCacheSize][ChunkCacheSize];
    char           name[NameSize];


    VBOCache vboCache;

    void Release(ChunkWorker* chunkWorker, SaveManager* savemgr);

private:
    int m_HighestBlock;

    ChunkColumn                 m_chunkChunkPool[ChunkPoolSize];
    std::vector<ChunkColumnPtr> m_freeChunkColums;

    WorkQueue* m_workqueue;

    Xorshift32 m_randomTickGen;

    int m_weather;

    void NotifyNeighbors(ChunkColumnPtr column, int chunkX, int chunkZ, int localX, int localZ, int y);
};

// Helper functions remain as free functions
int WorldToChunkCoord(int x);
int WorldToLocalCoord(int x);
