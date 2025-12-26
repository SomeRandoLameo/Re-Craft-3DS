#pragma once

#include "CT_Chunk.h"
#include "WorkQueue.h"
#include "../misc/NumberUtils.h"
#include "../misc/Xorshift.h"

#define CHUNKCACHE_SIZE (9)

typedef enum { WorldGen_Smea, WorldGen_SuperFlat, WorldGenTypes_Count } WorldGenType;
typedef enum { Gamemode_Survival, Gamemode_Creative, Gamemode_Adventure, Gamemode_Spectator, Gamemode_Count } gamemode;

typedef struct {
    uint64_t seed;
    WorldGenType type;
    union {
        struct {
            // No settings...
        } superflat;
    } settings;
} GeneratorSettings;

#define WORLD_NAME_SIZE 12

class World {
public:
    World(WorkQueue* workqueue);
    ~World();

    void Reset();
    void Tick();

    ChunkColumn* LoadChunk(int x, int z);
    void UnloadChunk(ChunkColumn* chunk);

    ChunkColumn* GetChunkColumn(int x, int z);

    mc::block::BlockPtr GetBlock(mc::Vector3i position);
    void SetBlock(mc::Vector3i position, mc::block::BlockPtr block);
    uint8_t GetMetadata(mc::Vector3i position);
    void SetMetadata(mc::Vector3i position, uint8_t metadata);

    void SetBlockAndMeta(mc::Vector3i position, mc::block::BlockPtr block, uint8_t metadata);

    void UpdateChunkCache(int originX, int originZ);

    int GetHeight(int x, int z);

    int GetHighestBlock() const { return m_HighestBlock; }
    const char* GetName() const { return name; }
    const GeneratorSettings& GetGenSettings() const { return genSettings; }
    int GetWeather() const { return m_weather; }
    void SetWeather(int newWeather) { m_weather = newWeather; }

    int cacheTranslationX, cacheTranslationZ;
    GeneratorSettings genSettings;

    ChunkColumn* chunkCache[CHUNKCACHE_SIZE][CHUNKCACHE_SIZE];
    char name[WORLD_NAME_SIZE];

private:
    int m_HighestBlock;
    WorkQueue* m_workqueue;
    Xorshift32 m_randomTickGen;
    int m_weather;
};

int WorldToChunkCoord(int x);
int WorldToLocalCoord(int x);