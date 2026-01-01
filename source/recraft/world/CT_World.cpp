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

    m_freeChunkColums.clear();

    Reset();
}

void World::Reset() {
	cacheTranslationX = 0;
	cacheTranslationZ = 0;

    m_freeChunkColums.clear();

	for (size_t i = 0; i < CHUNKPOOL_SIZE; i++) {
        m_chunkChunkPool[i].x = INT_MAX;
        m_chunkChunkPool[i].z = INT_MAX;
        m_freeChunkColums.push_back(&m_chunkChunkPool[i]);
	}

	m_randomTickGen = Xorshift32_New();
}

ChunkColumnPtr World::LoadChunk(int x, int z) {
	for (int i = 0; i < m_freeChunkColums.size(); i++) {
		if (m_freeChunkColums[i]->x == x && m_freeChunkColums[i]->z == z) {
            ChunkColumnPtr column = m_freeChunkColums[i];
            m_freeChunkColums.erase(m_freeChunkColums.begin() + i);
            for (int j = 0; j < CHUNKS_PER_COLUMN; j++) {
                column->RequestGraphicsUpdate(j);
            }
            column->references++;
			return column;
		}
	}

	for (int i = 0; i < m_freeChunkColums.size(); i++) {
		if (!m_freeChunkColums[i]->tasksRunning) {
            ChunkColumnPtr column = m_freeChunkColums[i];
            m_freeChunkColums.erase(m_freeChunkColums.begin() + i);

            column->~ChunkColumn();
            new (column) ChunkColumn(x, z);
			WorkQueue_AddItem(m_workqueue, (WorkerItem){WorkerItemType_Load, column});

            column->references++;
			return column;
		}
	}

	return NULL;
}

void World::UnloadChunk(ChunkColumnPtr column) {
    // TODO: unloaded chunks should be removed from the cache, there seems to be a bug with saving or a buffer
	WorkQueue_AddItem(m_workqueue, (WorkerItem){WorkerItemType_Save, column});
    m_freeChunkColums.push_back(column);

    for (int i = 0; i < CHUNKS_PER_COLUMN; ++i) {
        auto chunk = column->GetChunk(i);
        if(chunk->vertices){
            vboCache.Free(chunk->vbo);
            chunk->vbo.memory = nullptr;
            chunk->vbo.size = 0;
        }
        if(chunk->transparentVertices){
            vboCache.Free(chunk->transparentVBO);
            chunk->transparentVBO.memory = nullptr;
            chunk->transparentVBO.size = 0;
        }
    }
    column->references--;
}

ChunkColumnPtr World::GetChunkColumn(int x, int z) {
	int halfS = CHUNKCACHE_SIZE / 2;
	int lowX = cacheTranslationX - halfS;
	int lowZ = cacheTranslationZ - halfS;
	int highX = cacheTranslationX + halfS;
	int highZ = cacheTranslationZ + halfS;
	if (x >= lowX && z >= lowZ && x <= highX && z <= highZ) return columnCache[x - lowX][z - lowZ];
	return nullptr;
}

Block World::GetBlock( mc::Vector3i position) {
	if (position.y < 0 || position.y >= CHUNK_HEIGHT) return Block_Air;
	ChunkColumnPtr chunk = GetChunkColumn(WorldToChunkCoord(position.x), WorldToChunkCoord(position.z));
	if (chunk) return chunk->GetBlock(mc::Vector3i(WorldToLocalCoord(position.x), position.y, WorldToLocalCoord(position.z)));
	return Block_Air;
}

static void NotifyNeighbor(int axis, int comp, World* world, int cX, int cZ, int y, int xDiff, int zDiff) {
    if (axis == comp) {
        ChunkColumnPtr neighborChunk = world->GetChunkColumn(cX + xDiff, cZ + zDiff);
        if (neighborChunk) {
            neighborChunk->RequestGraphicsUpdate(y / CHUNK_SIZE);
        }
    }
}

static void NotifyAllNeighbors(ChunkColumnPtr column, World* world, int cX, int cZ, int lX, int lZ, int y) {
    NotifyNeighbor(lX, 0, world, cX, cZ, y, -1, 0);
    NotifyNeighbor(lX, 15, world, cX, cZ, y, 1, 0);
    NotifyNeighbor(lZ, 0, world, cX, cZ, y, 0, -1);
    NotifyNeighbor(lZ, 15, world, cX, cZ, y, 0, 1);

    if (WorldToLocalCoord(y) == 0 && y / CHUNK_SIZE - 1 >= 0) {
        column->RequestGraphicsUpdate(y / CHUNK_SIZE - 1);
    }
    if (WorldToLocalCoord(y) == 15 && y / CHUNK_SIZE + 1 < CHUNKS_PER_COLUMN) {
        column->RequestGraphicsUpdate(y / CHUNK_SIZE + 1);
    }
}

void World::SetBlock(mc::Vector3i position, Block block) {
	if (position.y < 0 || position.y >= CHUNK_HEIGHT) return;
	int cX = WorldToChunkCoord(position.x);
	int cZ = WorldToChunkCoord(position.z);
	ChunkColumnPtr column = GetChunkColumn(cX, cZ);
	if (column) {
		int lX = WorldToLocalCoord(position.x);
		int lZ = WorldToLocalCoord(position.z);
        column->SetBlock(mc::Vector3i(lX, position.y, lZ), block);

        NotifyAllNeighbors(column,this, cX,cZ,lX,lZ,position.y);
	}
}

void World::SetBlockAndMeta(mc::Vector3i position, Block block, uint8_t metadata) {
	if (position.y < 0 || position.y >= CHUNK_HEIGHT) return;
	int cX = WorldToChunkCoord(position.x);
	int cZ = WorldToChunkCoord(position.z);
	ChunkColumnPtr chunk = GetChunkColumn(cX, cZ);
	if (chunk) {
		int lX = WorldToLocalCoord(position.x);
		int lZ = WorldToLocalCoord(position.z);
        chunk->SetBlockAndMeta(mc::Vector3i(lX, position.y, lZ), block, metadata);

        NotifyAllNeighbors(chunk,this,cX,cZ,lX,lZ,position.y);
	}
}

uint8_t World::GetMetadata(mc::Vector3i position) {
	if (position.y < 0 || position.y >= CHUNK_HEIGHT) return 0;
	ChunkColumnPtr chunk = GetChunkColumn(WorldToChunkCoord(position.x), WorldToChunkCoord(position.z));
	if (chunk) return chunk->GetMetadata(mc::Vector3i(WorldToLocalCoord(position.x), position.y, WorldToLocalCoord(position.z)));
	return 0;
}

void World::SetMetadata(mc::Vector3i position, uint8_t metadata) {
	if (position.y < 0 || position.y >= CHUNK_HEIGHT) return;
	int cX = WorldToChunkCoord(position.x);
	int cZ = WorldToChunkCoord(position.z);
	ChunkColumnPtr chunk = GetChunkColumn(cX, cZ);
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
	ChunkColumnPtr chunk = GetChunkColumn(cX, cZ);
	if (chunk) {
		int lX = WorldToLocalCoord(x);
		int lZ = WorldToLocalCoord(z);

		return chunk->GetHeightMap(lX, lZ);
	}
	return 0;
}

void World::UpdateChunkCache(int orginX, int orginZ) {
	if (orginX != cacheTranslationX || orginZ != cacheTranslationZ) {
		ChunkColumnPtr tmpBuffer[CHUNKCACHE_SIZE][CHUNKCACHE_SIZE];
		memcpy(tmpBuffer, columnCache, sizeof(tmpBuffer));

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
                    columnCache[i][j] = tmpBuffer[i + diffX][j + diffZ];
					tmpBuffer[i + diffX][j + diffZ] = NULL;
				} else {
                    columnCache[i][j] = LoadChunk(wx, wz);
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
            ChunkColumn *chunk = columnCache[x][z];

            if (chunk->genProgress == ChunkGen_Empty && !chunk->tasksRunning) {
                WorkQueue_AddItem(m_workqueue, (WorkerItem) {WorkerItemType_BaseGen, chunk});
            }


            if (x > 0 && z > 0 && x < CHUNKCACHE_SIZE - 1 && z < CHUNKCACHE_SIZE - 1 &&
                chunk->genProgress == ChunkGen_Terrain && !chunk->tasksRunning) {
                bool clear = true;
                for (int xOff = -1; xOff < 2 && clear; xOff++) {
                    for (int zOff = -1; zOff < 2 && clear; zOff++) {
                        ChunkColumn *borderChunk = columnCache[x + xOff][z + zOff];
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