// CT_World.cpp - Simple on-demand allocation

#include "world/CT_World.h"
#include "blocks/BlockEvents.h"
#include <cstring>
#include <climits>
#include <cstdint>

int WorldToChunkCoord(int x) { return (x + (int)(x < 0)) / CHUNK_SIZE - (int)(x < 0); }
int WorldToLocalCoord(int x) { return x - WorldToChunkCoord(x) * CHUNK_SIZE; }

World::World(WorkQueue *workqueue) {
    strcpy(name, "TestWelt");
    m_workqueue = workqueue;
    genSettings.seed = 28112000;
    genSettings.type = WorldGen_SuperFlat;
    Reset();
}

World::~World() {
    for (int i = 0; i < CHUNKCACHE_SIZE; i++) {
        for (int j = 0; j < CHUNKCACHE_SIZE; j++) {
            if (chunkCache[i][j] != nullptr) {
                delete chunkCache[i][j];
                chunkCache[i][j] = nullptr;
            }
        }
    }
}

void World::Reset() {
    cacheTranslationX = 0;
    cacheTranslationZ = 0;

    memset(chunkCache, 0, sizeof(chunkCache));

    m_randomTickGen = Xorshift32_New();
}

ChunkColumn* World::LoadChunk(int x, int z) {
    ChunkColumn* chunk = new ChunkColumn(x, z);
    WorkQueue_AddItem(m_workqueue, (WorkerItem){WorkerItemType_Load, chunk});
    chunk->references++;
    return chunk;
}

void World::UnloadChunk(ChunkColumn* chunk) {
    if (chunk == nullptr) return;

    chunk->references--;

    WorkQueue_AddItem(m_workqueue, (WorkerItem){WorkerItemType_Save, chunk});

    delete chunk;
}

ChunkColumn* World::GetChunkColumn(int x, int z) {
    int halfS = CHUNKCACHE_SIZE / 2;
    int lowX = cacheTranslationX - halfS;
    int lowZ = cacheTranslationZ - halfS;
    int highX = cacheTranslationX + halfS;
    int highZ = cacheTranslationZ + halfS;

    if (x >= lowX && z >= lowZ && x <= highX && z <= highZ) {
        return chunkCache[x - lowX][z - lowZ];
    }
    return nullptr;
}

mc::block::BlockPtr World::GetBlock(mc::Vector3i position) {
    if (position.y < 0 || position.y >= CHUNK_HEIGHT) {
        return mc::block::BlockRegistry::GetInstance()->GetBlock("minecraft:air");
    }

    ChunkColumn* chunk = GetChunkColumn(WorldToChunkCoord(position.x), WorldToChunkCoord(position.z));
    if (chunk) {
        return chunk->GetBlock(mc::Vector3i(WorldToLocalCoord(position.x), position.y, WorldToLocalCoord(position.z)));
    }

    return mc::block::BlockRegistry::GetInstance()->GetBlock("minecraft:air");
}

static void NotifyNeighbor(int axis, int comp, World* world, int cX, int cZ, int y, int xDiff, int zDiff) {
    if (axis == comp) {
        ChunkColumn* neighborChunk = world->GetChunkColumn(cX + xDiff, cZ + zDiff);
        if (neighborChunk) {
            neighborChunk->RequestGraphicsUpdate(y / CHUNK_SIZE);
        }
    }
}

static void NotifyAllNeighbors(ChunkColumn* column, World* world, int cX, int cZ, int lX, int lZ, int y) {
    NotifyNeighbor(lX, 0, world, cX, cZ, y, -1, 0);
    NotifyNeighbor(lX, 15, world, cX, cZ, y, 1, 0);
    NotifyNeighbor(lZ, 0, world, cX, cZ, y, 0, -1);
    NotifyNeighbor(lZ, 15, world, cX, cZ, y, 0, 1);

    if (WorldToLocalCoord(y) == 0 && y / CHUNK_SIZE - 1 >= 0) {
        column->RequestGraphicsUpdate(y / CHUNK_SIZE - 1);
    }
    if (WorldToLocalCoord(y) == 15 && y / CHUNK_SIZE + 1 < CLUSTER_PER_CHUNK) {
        column->RequestGraphicsUpdate(y / CHUNK_SIZE + 1);
    }
}

void World::SetBlock(mc::Vector3i position, mc::block::BlockPtr block) {
    if (position.y < 0 || position.y >= CHUNK_HEIGHT) return;

    int cX = WorldToChunkCoord(position.x);
    int cZ = WorldToChunkCoord(position.z);
    ChunkColumn* chunk = GetChunkColumn(cX, cZ);

    if (chunk) {
        int lX = WorldToLocalCoord(position.x);
        int lZ = WorldToLocalCoord(position.z);
        chunk->SetBlock(mc::Vector3i(lX, position.y, lZ), block);
        NotifyAllNeighbors(chunk, this, cX, cZ, lX, lZ, position.y);
    }
}

void World::SetBlockAndMeta(mc::Vector3i position, mc::block::BlockPtr block, uint8_t metadata) {
    if (position.y < 0 || position.y >= CHUNK_HEIGHT) return;

    int cX = WorldToChunkCoord(position.x);
    int cZ = WorldToChunkCoord(position.z);
    ChunkColumn* chunk = GetChunkColumn(cX, cZ);

    if (chunk) {
        int lX = WorldToLocalCoord(position.x);
        int lZ = WorldToLocalCoord(position.z);
        chunk->SetBlockAndMeta(mc::Vector3i(lX, position.y, lZ), block, metadata);
        NotifyAllNeighbors(chunk, this, cX, cZ, lX, lZ, position.y);
    }
}

uint8_t World::GetMetadata(mc::Vector3i position) {
    if (position.y < 0 || position.y >= CHUNK_HEIGHT) return 0;

    ChunkColumn* chunk = GetChunkColumn(WorldToChunkCoord(position.x), WorldToChunkCoord(position.z));
    if (chunk) {
        return chunk->GetMetadata(mc::Vector3i(WorldToLocalCoord(position.x), position.y, WorldToLocalCoord(position.z)));
    }
    return 0;
}

void World::SetMetadata(mc::Vector3i position, uint8_t metadata) {
    if (position.y < 0 || position.y >= CHUNK_HEIGHT) return;

    int cX = WorldToChunkCoord(position.x);
    int cZ = WorldToChunkCoord(position.z);
    ChunkColumn* chunk = GetChunkColumn(cX, cZ);

    if (chunk) {
        int lX = WorldToLocalCoord(position.x);
        int lZ = WorldToLocalCoord(position.z);
        chunk->SetMetadata(mc::Vector3i(lX, position.y, lZ), metadata);
        NotifyAllNeighbors(chunk, this, cX, cZ, lX, lZ, position.y);
    }
}

int World::GetHeight(int x, int z) {
    int cX = WorldToChunkCoord(x);
    int cZ = WorldToChunkCoord(z);
    ChunkColumn* chunk = GetChunkColumn(cX, cZ);

    if (chunk) {
        int lX = WorldToLocalCoord(x);
        int lZ = WorldToLocalCoord(z);
        return chunk->GetHeightMap(lX, lZ);
    }
    return 0;
}

void World::UpdateChunkCache(int originX, int originZ) {
    if (originX != cacheTranslationX || originZ != cacheTranslationZ) {
        ChunkColumn* tmpBuffer[CHUNKCACHE_SIZE][CHUNKCACHE_SIZE];
        memcpy(tmpBuffer, chunkCache, sizeof(tmpBuffer));

        int oldBufferStartX = cacheTranslationX - CHUNKCACHE_SIZE / 2;
        int oldBufferStartZ = cacheTranslationZ - CHUNKCACHE_SIZE / 2;

        int diffX = originX - cacheTranslationX;
        int diffZ = originZ - cacheTranslationZ;

        for (int i = 0; i < CHUNKCACHE_SIZE; i++) {
            for (int j = 0; j < CHUNKCACHE_SIZE; j++) {
                int wx = originX + (i - CHUNKCACHE_SIZE / 2);
                int wz = originZ + (j - CHUNKCACHE_SIZE / 2);

                if (wx >= oldBufferStartX && wx < oldBufferStartX + CHUNKCACHE_SIZE &&
                    wz >= oldBufferStartZ && wz < oldBufferStartZ + CHUNKCACHE_SIZE) {
                    // Reuse existing chunk from old position
                    chunkCache[i][j] = tmpBuffer[i + diffX][j + diffZ];
                    tmpBuffer[i + diffX][j + diffZ] = nullptr;
                } else {
                    // Allocate new chunk
                    chunkCache[i][j] = LoadChunk(wx, wz);
                }
            }
        }

        // Clean up old chunks that are no longer in cache
        for (int i = 0; i < CHUNKCACHE_SIZE; i++) {
            for (int j = 0; j < CHUNKCACHE_SIZE; j++) {
                if (tmpBuffer[i][j] != nullptr) {
                    UnloadChunk(tmpBuffer[i][j]);
                }
            }
        }

        cacheTranslationX = originX;
        cacheTranslationZ = originZ;
    }
}

void World::Tick() {
    for (int x = 0; x < CHUNKCACHE_SIZE; x++) {
        for (int z = 0; z < CHUNKCACHE_SIZE; z++) {
            ChunkColumn* chunk = chunkCache[x][z];
            if (!chunk) continue;

            if (chunk->genProgress == ChunkGen_Empty && !chunk->tasksRunning) {
                WorkQueue_AddItem(m_workqueue, (WorkerItem){WorkerItemType_BaseGen, chunk});
            }

            if (x > 0 && z > 0 && x < CHUNKCACHE_SIZE - 1 && z < CHUNKCACHE_SIZE - 1 &&
                chunk->genProgress == ChunkGen_Terrain && !chunk->tasksRunning) {
                bool clear = true;
                for (int xOff = -1; xOff < 2 && clear; xOff++) {
                    for (int zOff = -1; zOff < 2 && clear; zOff++) {
                        ChunkColumn* borderChunk = chunkCache[x + xOff][z + zOff];
                        if (!borderChunk || borderChunk->genProgress == ChunkGen_Empty ||
                            borderChunk->tasksRunning) clear = false;
                    }
                }

                if (clear) {
                    WorkQueue_AddItem(m_workqueue, (WorkerItem){WorkerItemType_Decorate, chunk});
                }
            }
        }
    }
}