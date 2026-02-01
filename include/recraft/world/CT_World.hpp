#pragma once

#include "CT_Chunk.hpp"
#include "WorkQueue.hpp"

#include "misc/NumberUtils.hpp"
#include "misc/Xorshift.hpp"
#include "phys/AABB.hpp"


#define UNDEADCHUNKS_COUNT (2 * World::ChunkCacheSize + World::ChunkCacheSize * World::ChunkCacheSize)


enum WorldGenType{
    Smea,
    SuperFlat,
    Empty,
    WorldGenTypes_Count
};

enum Gamemode {
    Survival,
    Creative,
    adventure,
    Spectator,
    Gamemode_Count
};

struct GeneratorSettings{
	u64 seed;
	WorldGenType type;
	//gamemode type;
	union {
		struct {
			// Keine Einstellungen...
		} superflat;
	} settings;
};

class ChunkWorker;
class SaveManager;

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

    mc::block::BlockEntityPtr GetBlockEntity(mc::Vector3i pos);

    BlockID GetBlockID(mc::Vector3i position);
    void SetBlockID(mc::Vector3i position, BlockID block);
    u8 GetMetadata(mc::Vector3i position);
    void SetMetadata(mc::Vector3i position, u8 metadata);

    BlockPtr GetBlock(mc::Vector3i position);
    void SetBlock(mc::Vector3i position, BlockPtr block);

    std::vector<AABB> GetCubes(const AABB& aabb);

    void SetBlockAndMeta(mc::Vector3i position, BlockID block, u8 metadata);

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

    void Release(ChunkWorker* chunkWorker, SaveManager* savemgr);
private:
    int m_HighestBlock;

    ChunkColumn m_chunkChunkPool[ChunkPoolSize];
    std::vector<ChunkColumnPtr> m_freeChunkColums;

    WorkQueue* m_workqueue;

    Xorshift32 m_randomTickGen;

    int m_weather;

    void NotifyNeighbors(ChunkColumnPtr column, int chunkX, int chunkZ, int localX, int localZ, int y);
};

// Helper functions remain as free functions
int WorldToChunkCoord(int x);
int WorldToLocalCoord(int x);
