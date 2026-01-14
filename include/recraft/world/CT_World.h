#pragma once

#include "CT_Chunk.h"
#include "WorkQueue.h"

#include "../misc/NumberUtils.h"
#include "../misc/Xorshift.h"



#define UNDEADCHUNKS_COUNT (2 * World::ChunkCacheSize + World::ChunkCacheSize * World::ChunkCacheSize)


typedef enum {
    Smea,
    SuperFlat,
    Empty,
    WorldGenTypes_Count
} WorldGenType;

typedef enum {
    Survival,
    Creative,
    adventure,
    Spectator,
    Gamemode_Count
} Gamemode;

typedef struct {
	uint64_t seed;
	WorldGenType type;
	//gamemode type;
	union {
		struct {
			// Keine Einstellungen...
		} superflat;
	} settings;
} GeneratorSettings;

class World {
public:

    constexpr static int Height = 256;
    constexpr static int ChunkCacheSize = 9;
    constexpr static int ChunkPoolSize = World::ChunkCacheSize * World::ChunkCacheSize + UNDEADCHUNKS_COUNT;
    constexpr static int NameSize = 12;


    World(WorkQueue* workqueue);
    ~World() = default;

    void Reset();
    void Tick();

    ChunkColumnPtr LoadChunk(int x, int z);
    void UnloadChunk(ChunkColumnPtr chunk);

    ChunkColumnPtr GetChunkColumn(int x, int z);

    Block GetBlock(mc::Vector3i position);
    void SetBlock(mc::Vector3i position, Block block);
    uint8_t GetMetadata(mc::Vector3i position);
    void SetMetadata(mc::Vector3i position, uint8_t metadata);

    void SetBlockAndMeta(mc::Vector3i position, Block block, uint8_t metadata);

    void UpdateChunkCache(int originX, int originZ);

    int GetHeight(int x, int z);

    // Public member access (if needed)
    int GetHighestBlock() const { return m_HighestBlock; }
    const char* GetName() const { return name; }
    const GeneratorSettings& GetGenSettings() const { return genSettings; }
    int GetWeather() const { return m_weather; }
    void SetWeather(int newWeather) { m_weather = newWeather; }

    bool online = false;

    int cacheTranslationX, cacheTranslationZ;
    GeneratorSettings genSettings;

    ChunkColumnPtr columnCache[ChunkCacheSize][ChunkCacheSize];
    char name[NameSize];



    VBOCache vboCache;
private:
    int m_HighestBlock;

    ChunkColumn m_chunkChunkPool[ChunkPoolSize];
    std::vector<ChunkColumnPtr> m_freeChunkColums;

    WorkQueue* m_workqueue;

    Xorshift32 m_randomTickGen;

    int m_weather;
};

// Helper functions remain as free functions
int WorldToChunkCoord(int x);
int WorldToLocalCoord(int x);