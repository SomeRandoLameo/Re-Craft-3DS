#include "world/CT_World.h"

#include "blocks/BlockEvents.h"

#include <cstring>
#include <climits>
#include <cstdint>

int WorldToChunkCoord(int x) { return (x + (int)(x < 0)) / Chunk::Size - (int)(x < 0); }
int WorldToLocalCoord(int x) { return x - WorldToChunkCoord(x) * Chunk::Size; }

World::World(WorkQueue *workqueue) {
    strcpy(name, "TestWelt");

    m_workqueue = workqueue;

    genSettings.seed = 28112000;
    genSettings.type = WorldGenType::Empty; // as it should be

    m_freeChunkColums.clear();

    Reset();
}

void World::Reset() {
	cacheTranslationX = 0;
	cacheTranslationZ = 0;

    m_freeChunkColums.clear();

	for (size_t i = 0; i < ChunkPoolSize; i++) {
        m_chunkChunkPool[i].x = INT_MAX;
        m_chunkChunkPool[i].z = INT_MAX;
        m_freeChunkColums.push_back(&m_chunkChunkPool[i]);
	}

	m_randomTickGen = Xorshift32_New();
}

/**
 * Loads a chunk column at the specified world coordinates.
 *
 * This function implements a chunk pooling system to efficiently manage memory by reusing
 * existing chunk column objects rather than allocating new ones. It first attempts to find
 * an existing chunk at the requested coordinates, then tries to recycle an unused chunk,
 * and finally returns NULL if no chunks are available.
 *
 * @param x The x-coordinate of the chunk column in world space
 * @param z The z-coordinate of the chunk column in world space
 *
 * @return ChunkColumnPtr Pointer to the loaded/recycled chunk column, or NULL if no chunks are available
 *
 * @note Returned chunk will have its reference count incremented
 * @note This function queues asynchronous load operations via the work queue
 *
 * Behavior:
 * 1. First Pass - Reuse Existing Chunk:
 *    - Searches the free chunk pool for a chunk already at position (x, z)
 *    - If found, removes it from the pool, refreshes its graphics, and returns it
 *
 * 2. Second Pass - Recycle Unused Chunk:
 *    - If no matching chunk exists, looks for any chunk not currently running tasks
 *    - Destroys the old chunk data and reconstructs it at the new position (x, z)
 *    - Queues the chunk for background loading via the work queue
 *
 * 3. Failure:
 *    - Returns NULL if all chunks are either in use or running background tasks
 */
ChunkColumnPtr World::LoadChunk(int x, int z) {
    // First pass: Check if a chunk at this position already exists in the free pool
    for (int i = 0; i < m_freeChunkColums.size(); i++) {
        if (m_freeChunkColums[i]->x == x && m_freeChunkColums[i]->z == z) {
            ChunkColumnPtr column = m_freeChunkColums[i];
            // Remove from free pool
            m_freeChunkColums.erase(m_freeChunkColums.begin() + i);

            // Request graphics updates for all chunks in this column
            for (int j = 0; j < ChunkColumn::ChunksPerColumn; j++) {
                column->RequestGraphicsUpdate(j);
            }

            // Mark as in-use by incrementing reference count
            column->references++;
            return column;
        }
    }

    // Second pass: If no chunk found at the requested position, recycle an unused chunk
    for (int i = 0; i < m_freeChunkColums.size(); i++) {
        if (!m_freeChunkColums[i]->tasksRunning) {
            ChunkColumnPtr column = m_freeChunkColums[i];
            // Remove from free pool
            m_freeChunkColums.erase(m_freeChunkColums.begin() + i);

            // Destroy old chunk data and reconstruct at new position
            column->~ChunkColumn();
            new (column) ChunkColumn(x, z);

            // Queue chunk for asynchronous loading
            WorkQueue_AddItem(m_workqueue, (WorkerItem){WorkerItemType::Load, column});

            // Mark as in-use by incrementing reference count
            column->references++;
            return column;
        }
    }

    // No chunks available (all in use or running tasks)
    return NULL;
}

//TODO: if online == true: dont save, else: save.
void World::UnloadChunk(ChunkColumnPtr column) {
	WorkQueue_AddItem(m_workqueue, (WorkerItem){WorkerItemType::Save, column});
    m_freeChunkColums.push_back(column);

    for (int i = 0; i < ChunkColumn::ChunksPerColumn; ++i) {
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

// Do we really need to cache chunks in general?
ChunkColumnPtr World::GetChunkColumn(int x, int z) {
	int halfS = World::ChunkCacheSize / 2;
	int lowX = cacheTranslationX - halfS;
	int lowZ = cacheTranslationZ - halfS;
	int highX = cacheTranslationX + halfS;
	int highZ = cacheTranslationZ + halfS;
	if (x >= lowX && z >= lowZ && x <= highX && z <= highZ) return columnCache[x - lowX][z - lowZ];
	return nullptr;
}

Block World::GetBlock( mc::Vector3i position) {
	if (position.y < 0 || position.y >= World::Height) return Block::Air;
	ChunkColumnPtr chunk = GetChunkColumn(WorldToChunkCoord(position.x), WorldToChunkCoord(position.z));
	if (chunk) return chunk->GetBlock(mc::Vector3i(WorldToLocalCoord(position.x), position.y, WorldToLocalCoord(position.z)));
	return Block::Air;
}

static void NotifyNeighbor(int axis, int comp, World* world, int cX, int cZ, int y, int xDiff, int zDiff) {
    if (axis == comp) {
        ChunkColumnPtr neighborChunk = world->GetChunkColumn(cX + xDiff, cZ + zDiff);
        if (neighborChunk) {
            neighborChunk->RequestGraphicsUpdate(y / Chunk::Size);
        }
    }
}

static void NotifyAllNeighbors(ChunkColumnPtr column, World* world, int cX, int cZ, int lX, int lZ, int y) {
    NotifyNeighbor(lX, 0, world, cX, cZ, y, -1, 0);
    NotifyNeighbor(lX, 15, world, cX, cZ, y, 1, 0);
    NotifyNeighbor(lZ, 0, world, cX, cZ, y, 0, -1);
    NotifyNeighbor(lZ, 15, world, cX, cZ, y, 0, 1);

    if (WorldToLocalCoord(y) == 0 && y / Chunk::Size - 1 >= 0) {
        column->RequestGraphicsUpdate(y / Chunk::Size - 1);
    }
    if (WorldToLocalCoord(y) == 15 && y / Chunk::Size + 1 < ChunkColumn::ChunksPerColumn) {
        column->RequestGraphicsUpdate(y / Chunk::Size + 1);
    }
}

void World::SetBlock(mc::Vector3i position, Block block) {
	if (position.y < 0 || position.y >= World::Height) return;
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
	if (position.y < 0 || position.y >= World::Height) return;
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
	if (position.y < 0 || position.y >= World::Height) return 0;
	ChunkColumnPtr chunk = GetChunkColumn(WorldToChunkCoord(position.x), WorldToChunkCoord(position.z));
	if (chunk) return chunk->GetMetadata(mc::Vector3i(WorldToLocalCoord(position.x), position.y, WorldToLocalCoord(position.z)));
	return 0;
}

void World::SetMetadata(mc::Vector3i position, uint8_t metadata) {
	if (position.y < 0 || position.y >= World::Height) return;
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
		ChunkColumnPtr tmpBuffer[World::ChunkCacheSize][World::ChunkCacheSize];
		memcpy(tmpBuffer, columnCache, sizeof(tmpBuffer));

		int oldBufferStartX = cacheTranslationX - World::ChunkCacheSize / 2;
		int oldBufferStartZ = cacheTranslationZ - World::ChunkCacheSize / 2;

		int diffX = orginX - cacheTranslationX;
		int diffZ = orginZ - cacheTranslationZ;

		for (int i = 0; i < World::ChunkCacheSize; i++) {
			for (int j = 0; j < World::ChunkCacheSize; j++) {
				int wx = orginX + (i - World::ChunkCacheSize / 2);
				int wz = orginZ + (j - World::ChunkCacheSize / 2);
				if (wx >= oldBufferStartX && wx < oldBufferStartX + World::ChunkCacheSize && wz >= oldBufferStartZ &&
				    wz < oldBufferStartZ + World::ChunkCacheSize) {
                    columnCache[i][j] = tmpBuffer[i + diffX][j + diffZ];
					tmpBuffer[i + diffX][j + diffZ] = NULL;
				} else {
                    columnCache[i][j] = LoadChunk(wx, wz);
				}
			}
		}

		for (int i = 0; i < World::ChunkCacheSize; i++) {
			for (int j = 0; j < World::ChunkCacheSize; j++) {
				if (tmpBuffer[i][j] != NULL) {
					UnloadChunk(tmpBuffer[i][j]);
				}
			}
		}

		cacheTranslationX = orginX;
		cacheTranslationZ = orginZ;
	}
}

/**
 * Performs per-tick world update logic for all chunks in the cache.
 *
 * This function manages the chunk generation pipeline and random block ticks for the entire
 * cached world area. It iterates through all chunks in the cache and triggers appropriate
 * generation stages or gameplay updates based on each chunk's current state.
 *
 * Generation Pipeline:
 * 1. BaseGen - Initial terrain generation for empty chunks
 * 2. Decorate - Decoration pass (trees, structures, etc.) once surrounding chunks are ready
 * 3. RandomTicks - Gameplay updates for fully generated chunks (plant growth, etc.)
 *
 * @note This function is called every tick
 * @note Work items are queued asynchronously via the work queue system
 * @note Chunks must not have running tasks before new work can be queued
 */
void World::Tick() {
    // Iterate through all chunks in the cache
    for (int x = 0; x < World::ChunkCacheSize; x++) {
        for (int z = 0; z < World::ChunkCacheSize; z++) {
            ChunkColumn *chunk = columnCache[x][z];

            // Stage 1: Queue base terrain generation for empty chunks
            if (chunk->genProgress == ChunkGen_Empty && !chunk->tasksRunning) {
                WorkQueue_AddItem(m_workqueue, (WorkerItem) {WorkerItemType::BaseGen, chunk});
            }

            // Stage 2: Queue decoration pass for terrain-generated chunks
            // Only process chunks not on the border (need neighbors for decoration)
            if (x > 0 && z > 0 && x < World::ChunkCacheSize - 1 && z < World::ChunkCacheSize - 1 &&
                chunk->genProgress == ChunkGen_Terrain && !chunk->tasksRunning) {

                // Check if all 3x3 neighboring chunks (including self) have base terrain generated
                bool clear = true;
                for (int xOff = -1; xOff < 2 && clear; xOff++) {
                    for (int zOff = -1; zOff < 2 && clear; zOff++) {
                        ChunkColumn *borderChunk = columnCache[x + xOff][z + zOff];
                        // Neighbor must have terrain generated and no running tasks
                        if (borderChunk->genProgress == ChunkGen_Empty || !borderChunk->tasksRunning)
                            clear = false;
                    }
                }

                // All neighbors ready - safe to decorate this chunk
                if (clear) {
                    WorkQueue_AddItem(m_workqueue, (WorkerItem) {WorkerItemType::Decorate, chunk});
                }

                // Stage 3: Generate random tick coordinates for gameplay updates
                // (Note: These coordinates are generated but not currently used in the code)
                int xVals[RANDOMTICKS_PER_CHUNK];
                int yVals[RANDOMTICKS_PER_CHUNK];
                int zVals[RANDOMTICKS_PER_CHUNK];
                for (int i = 0; i < RANDOMTICKS_PER_CHUNK; i++) {
                    // Generate random block coordinates within the chunk for random tick events
                    xVals[i] = WorldToLocalCoord(Xorshift32_Next(&m_randomTickGen));
                    yVals[i] = WorldToLocalCoord(Xorshift32_Next(&m_randomTickGen));
                    zVals[i] = WorldToLocalCoord(Xorshift32_Next(&m_randomTickGen));
                }
            }
        }
    }
}