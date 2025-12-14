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

    m_freeChunks.clear();

    Reset();
}

void World::Reset() {
	cacheTranslationX = 0;
	cacheTranslationZ = 0;

	m_freeChunks.clear();

	for (size_t i = 0; i < CHUNKPOOL_SIZE; i++) {
		m_chunkPool[i].x = INT_MAX;
		m_chunkPool[i].z = INT_MAX;
		m_freeChunks.push_back(&m_chunkPool[i]);
	}

	m_randomTickGen = Xorshift32_New();
}

Chunk* World::LoadChunk(int x, int z) {
	for (int i = 0; i < m_freeChunks.size(); i++) {
		if (m_freeChunks[i]->x == x && m_freeChunks[i]->z == z) {
			Chunk* chunk = m_freeChunks[i];
            m_freeChunks.erase(m_freeChunks.begin() + i);

			chunk->references++;
			return chunk;
		}
	}

	for (int i = 0; i < m_freeChunks.size(); i++) {
		if (!m_freeChunks[i]->tasksRunning) {
			Chunk* chunk = m_freeChunks[i];
            m_freeChunks.erase(m_freeChunks.begin() + i);

            chunk->~Chunk();
            new (chunk) Chunk(x, z);
			WorkQueue_AddItem(m_workqueue, (WorkerItem){WorkerItemType_Load, chunk});

			chunk->references++;
			return chunk;
		}
	}

	return NULL;
}

void World::UnloadChunk(Chunk* chunk) {
    // TODO: unloaded chunks should be removed from the cache, there seems to be a bug with saving or a buffer
	WorkQueue_AddItem(m_workqueue, (WorkerItem){WorkerItemType_Save, chunk});
	m_freeChunks.push_back(chunk);
	chunk->references--;
}

Chunk* World::GetChunk(int x, int z) {
	int halfS = CHUNKCACHE_SIZE / 2;
	int lowX = cacheTranslationX - halfS;
	int lowZ = cacheTranslationZ - halfS;
	int highX = cacheTranslationX + halfS;
	int highZ = cacheTranslationZ + halfS;
	if (x >= lowX && z >= lowZ && x <= highX && z <= highZ) return chunkCache[x - lowX][z - lowZ];
	return NULL;
}

Block World::GetBlock( mc::Vector3i position) {
	if (position.y < 0 || position.y >= CHUNK_HEIGHT) return Block_Air;
	Chunk* chunk = GetChunk(WorldToChunkCoord(position.x), WorldToChunkCoord(position.z));
	if (chunk) return chunk->GetBlock(mc::Vector3i(WorldToLocalCoord(position.x), position.y, WorldToLocalCoord(position.z)));
	return Block_Air;
}

static void NotifyNeighbor(int axis, int comp, World* world, int cX, int cZ, int y, int xDiff, int zDiff) {
    if (axis == comp) {
        Chunk* neighborChunk = world->GetChunk(cX + xDiff, cZ + zDiff);
        if (neighborChunk) {
            neighborChunk->RequestGraphicsUpdate(y / CHUNK_SIZE);
        }
    }
}

static void NotifyAllNeighbors(Chunk* chunk, World* world, int cX, int cZ, int lX, int lZ, int y) {
    NotifyNeighbor(lX, 0, world, cX, cZ, y, -1, 0);
    NotifyNeighbor(lX, 15, world, cX, cZ, y, 1, 0);
    NotifyNeighbor(lZ, 0, world, cX, cZ, y, 0, -1);
    NotifyNeighbor(lZ, 15, world, cX, cZ, y, 0, 1);

    if (WorldToLocalCoord(y) == 0 && y / CHUNK_SIZE - 1 >= 0) {
        chunk->RequestGraphicsUpdate(y / CHUNK_SIZE - 1);
    }
    if (WorldToLocalCoord(y) == 15 && y / CHUNK_SIZE + 1 < CLUSTER_PER_CHUNK) {
        chunk->RequestGraphicsUpdate(y / CHUNK_SIZE + 1);
    }
}

void World::SetBlock(mc::Vector3i position, Block block) {
	if (position.y < 0 || position.y >= CHUNK_HEIGHT) return;
	int cX = WorldToChunkCoord(position.x);
	int cZ = WorldToChunkCoord(position.z);
	Chunk* chunk = GetChunk(cX, cZ);
	if (chunk) {
		int lX = WorldToLocalCoord(position.x);
		int lZ = WorldToLocalCoord(position.z);
        chunk->SetBlock(mc::Vector3i(lX, position.y, lZ), block);

        NotifyAllNeighbors(chunk,this, cX,cZ,lX,lZ,position.y);
	}
}

void World::SetBlockAndMeta(mc::Vector3i position, Block block, uint8_t metadata) {
	if (position.y < 0 || position.y >= CHUNK_HEIGHT) return;
	int cX = WorldToChunkCoord(position.x);
	int cZ = WorldToChunkCoord(position.z);
	Chunk* chunk = GetChunk(cX, cZ);
	if (chunk) {
		int lX = WorldToLocalCoord(position.x);
		int lZ = WorldToLocalCoord(position.z);
        chunk->SetBlockAndMeta(mc::Vector3i(lX, position.y, lZ), block, metadata);

        NotifyAllNeighbors(chunk,this,cX,cZ,lX,lZ,position.y);
	}
}

uint8_t World::GetMetadata(mc::Vector3i position) {
	if (position.y < 0 || position.y >= CHUNK_HEIGHT) return 0;
	Chunk* chunk = GetChunk(WorldToChunkCoord(position.x), WorldToChunkCoord(position.z));
	if (chunk) return chunk->GetMetadata(mc::Vector3i(WorldToLocalCoord(position.x), position.y, WorldToLocalCoord(position.z)));
	return 0;
}

void World::SetMetadata(mc::Vector3i position, uint8_t metadata) {
	if (position.y < 0 || position.y >= CHUNK_HEIGHT) return;
	int cX = WorldToChunkCoord(position.x);
	int cZ = WorldToChunkCoord(position.z);
	Chunk* chunk = GetChunk(cX, cZ);
	if (chunk) {
		int lX = WorldToLocalCoord(position.x);
		int lZ = WorldToLocalCoord(position.z);
        chunk->SetMetadata(mc::Vector3i(lX, position.y, lZ), metadata);

        NotifyAllNeighbors(chunk,this,cX,cZ,lX,lZ,position.y);
	}
}

int World::GetHeight(int x, int z) {
	int cX = WorldToChunkCoord(x);
	int cZ = WorldToChunkCoord(z);
	Chunk* chunk = GetChunk(cX, cZ);
	if (chunk) {
		int lX = WorldToLocalCoord(x);
		int lZ = WorldToLocalCoord(z);

		return chunk->GetHeightMap(lX, lZ);
	}
	return 0;
}

void World::UpdateChunkCache(int orginX, int orginZ) {
	if (orginX != cacheTranslationX || orginZ != cacheTranslationZ) {
		Chunk* tmpBuffer[CHUNKCACHE_SIZE][CHUNKCACHE_SIZE];
		memcpy(tmpBuffer, chunkCache, sizeof(tmpBuffer));

		int oldBufferStartX = cacheTranslationX - CHUNKCACHE_SIZE / 2;
		int oldBufferStartZ = cacheTranslationZ - CHUNKCACHE_SIZE / 2;

		int diffX = orginX - cacheTranslationX;
		int diffZ = orginZ - cacheTranslationZ;

		for (int i = 0; i < CHUNKCACHE_SIZE; i++) {
			for (int j = 0; j < CHUNKCACHE_SIZE; j++) {
				int wx = orginX + (i - CHUNKCACHE_SIZE / 2);
				int wz = orginZ + (j - CHUNKCACHE_SIZE / 2);
				if (wx >= oldBufferStartX && wx < oldBufferStartX + CHUNKCACHE_SIZE && wz >= oldBufferStartZ &&
				    wz < oldBufferStartZ + CHUNKCACHE_SIZE) {
					chunkCache[i][j] = tmpBuffer[i + diffX][j + diffZ];
					tmpBuffer[i + diffX][j + diffZ] = NULL;
				} else {
					chunkCache[i][j] = LoadChunk(wx, wz);
				}
			}
		}

		for (int i = 0; i < CHUNKCACHE_SIZE; i++) {
			for (int j = 0; j < CHUNKCACHE_SIZE; j++) {
				if (tmpBuffer[i][j] != NULL) {
					UnloadChunk(tmpBuffer[i][j]);
				}
			}
		}

		cacheTranslationX = orginX;
		cacheTranslationZ = orginZ;
	}
}

void World::Tick() {
	for (int x = 0; x < CHUNKCACHE_SIZE; x++) {
        for (int z = 0; z < CHUNKCACHE_SIZE; z++) {
            Chunk *chunk = chunkCache[x][z];

            if (chunk->genProgress == ChunkGen_Empty && !chunk->tasksRunning) {
                WorkQueue_AddItem(m_workqueue, (WorkerItem) {WorkerItemType_BaseGen, chunk});
            }


            if (x > 0 && z > 0 && x < CHUNKCACHE_SIZE - 1 && z < CHUNKCACHE_SIZE - 1 &&
                chunk->genProgress == ChunkGen_Terrain && !chunk->tasksRunning) {
                bool clear = true;
                for (int xOff = -1; xOff < 2 && clear; xOff++) {
                    for (int zOff = -1; zOff < 2 && clear; zOff++) {
                        Chunk *borderChunk = chunkCache[x + xOff][z + zOff];
                        if (borderChunk->genProgress == ChunkGen_Empty || !borderChunk->tasksRunning) clear = false;
                    }
                }

                if (clear) {
                    WorkQueue_AddItem(m_workqueue, (WorkerItem) {WorkerItemType_Decorate, chunk});
                }

                int xVals[RANDOMTICKS_PER_CHUNK];
                int yVals[RANDOMTICKS_PER_CHUNK];
                int zVals[RANDOMTICKS_PER_CHUNK];
                for (int i = 0; i < RANDOMTICKS_PER_CHUNK; i++) {
                    xVals[i] = WorldToLocalCoord(Xorshift32_Next(&m_randomTickGen));
                    yVals[i] = WorldToLocalCoord(Xorshift32_Next(&m_randomTickGen));
                    zVals[i] = WorldToLocalCoord(Xorshift32_Next(&m_randomTickGen));
                }
            }
        }
    }
}