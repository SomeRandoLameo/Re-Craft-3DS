#include "world/ChunkWorker.h"
#include "world/savegame/SaveManager.h"
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
    return nullptr;
}


void World::UnloadChunk(ChunkColumnPtr column) {

    if(!online) WorkQueue_AddItem(m_workqueue, (WorkerItem){WorkerItemType::Save, column});

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

ChunkColumnPtr World::GetChunkColumn(int chunkX, int chunkZ) {
    int halfCacheSize = ChunkCacheSize / 2;
    int cacheMinX = cacheTranslationX - halfCacheSize;
    int cacheMinZ = cacheTranslationZ - halfCacheSize;

    if (chunkX < cacheMinX || chunkZ < cacheMinZ) {
        return nullptr;
    }

    int cacheIndexX = chunkX - cacheMinX;
    int cacheIndexZ = chunkZ - cacheMinZ;

    if (cacheIndexX >= ChunkCacheSize || cacheIndexZ >= ChunkCacheSize) {
        return nullptr;
    }

    return columnCache[cacheIndexX][cacheIndexZ];
}


mc::block::BlockEntityPtr World::GetBlockEntity(mc::Vector3i pos) {
    ChunkColumnPtr col = GetChunkColumn(pos.x, pos.z);

    if (!col) return nullptr;

    return col->GetBlockEntity(pos);
}


Block World::GetBlock(mc::Vector3i position) {
    if (position.y < 0 || position.y >= World::Height) {
        return Block::Air;
    }

    ChunkColumnPtr chunk = GetChunkColumn(
        WorldToChunkCoord(position.x),
        WorldToChunkCoord(position.z)
    );

    if (!chunk) {
        return Block::Air;
    }

    position.x = WorldToLocalCoord(position.x);
    position.z = WorldToLocalCoord(position.z);
    return chunk->GetBlock(position);
}

/**
 * Notifies neighboring chunks to update their graphics when a block changes.
 *
 * When a block is placed or modified on a chunk boundary, adjacent chunks need to
 * regenerate their meshes to properly render the change. This function checks if
 * the block is on any chunk edge and notifies the appropriate neighbors.
 *
 * @param column The chunk column containing the modified block
 * @param chunkX X-coordinate of the chunk in world space
 * @param chunkZ Z-coordinate of the chunk in world space
 * @param localX X-coordinate within the chunk (0-15)
 * @param localZ Z-coordinate within the chunk (0-15)
 * @param y World-space Y-coordinate of the modified block
 */
void World::NotifyNeighbors(ChunkColumnPtr column, int chunkX, int chunkZ, int localX, int localZ, int y) {
    int chunkY = y / Chunk::Size;

    if (localX == 0) {
        ChunkColumnPtr chunk = GetChunkColumn(chunkX - 1, chunkZ);
        if (chunk) chunk->RequestGraphicsUpdate(chunkY);
    }

    if (localX == 15) {
        ChunkColumnPtr chunk = GetChunkColumn(chunkX + 1, chunkZ);
        if (chunk) chunk->RequestGraphicsUpdate(chunkY);
    }

    if (localZ == 0) {
        ChunkColumnPtr chunk = GetChunkColumn(chunkX, chunkZ - 1);
        if (chunk) chunk->RequestGraphicsUpdate(chunkY);
    }

    if (localZ == 15) {
        ChunkColumnPtr chunk = GetChunkColumn(chunkX, chunkZ + 1);
        if (chunk) chunk->RequestGraphicsUpdate(chunkY);
    }

    int localY = WorldToLocalCoord(y);
    if (localY == 0 && chunkY > 0) {
        column->RequestGraphicsUpdate(chunkY - 1);
    }
    if (localY == 15 && chunkY + 1 < ChunkColumn::ChunksPerColumn) {
        column->RequestGraphicsUpdate(chunkY + 1);
    }
}

void World::SetBlock(mc::Vector3i position, Block block) {
	if (position.y < 0 || position.y >= World::Height) return;
	int chunkX = WorldToChunkCoord(position.x);
	int chunkZ = WorldToChunkCoord(position.z);

	ChunkColumnPtr column = GetChunkColumn(chunkX, chunkZ);
	if (column) {
		int localX = WorldToLocalCoord(position.x);
		int localZ = WorldToLocalCoord(position.z);
        column->SetBlock(mc::Vector3i(localX, position.y, localZ), block);

        NotifyNeighbors(column, chunkX, chunkZ, localX, localZ, position.y);
	}
}

void World::SetBlockAndMeta(mc::Vector3i position, Block block, uint8_t metadata) {
	if (position.y < 0 || position.y >= World::Height) return;
	int chunkX = WorldToChunkCoord(position.x);
	int chunkZ = WorldToChunkCoord(position.z);

	ChunkColumnPtr chunk = GetChunkColumn(chunkX, chunkZ);
	if (chunk) {
		int localX = WorldToLocalCoord(position.x);
		int localZ = WorldToLocalCoord(position.z);
        chunk->SetBlockAndMeta(mc::Vector3i(localX, position.y, localZ), block, metadata);

        NotifyNeighbors(chunk, chunkX, chunkZ, localX, localZ, position.y);
	}
}

uint8_t World::GetMetadata(mc::Vector3i position) {
    if (position.y < 0 || position.y >= World::Height) {
        return 0;
    }

    int chunkX = WorldToChunkCoord(position.x);
    int chunkZ = WorldToChunkCoord(position.z);
    ChunkColumnPtr chunk = GetChunkColumn(chunkX, chunkZ);

    if (!chunk) {
        return 0;
    }

    int localX = WorldToLocalCoord(position.x);
    int localZ = WorldToLocalCoord(position.z);
    return chunk->GetMetadata(mc::Vector3i(localX, position.y, localZ));
}

void World::SetMetadata(mc::Vector3i position, uint8_t metadata) {
	if (position.y < 0 || position.y >= World::Height) return;
	int chunkX = WorldToChunkCoord(position.x);
	int chunkZ = WorldToChunkCoord(position.z);

	ChunkColumnPtr chunk = GetChunkColumn(chunkX, chunkZ);
	if (chunk) {
		int localX = WorldToLocalCoord(position.x);
		int localZ = WorldToLocalCoord(position.z);
        chunk->SetMetadata(mc::Vector3i(localX, position.y, localZ), metadata);

        NotifyNeighbors(chunk, chunkX, chunkZ, localX, localZ, position.y);
	}
}

int World::GetHeight(int x, int z) {
	int chunkX = WorldToChunkCoord(x);
	int chunkZ = WorldToChunkCoord(z);

	ChunkColumnPtr chunk = GetChunkColumn(chunkX, chunkZ);
	if (chunk) {
		int localX = WorldToLocalCoord(x);
		int localZ = WorldToLocalCoord(z);

		return chunk->GetHeightMap(localX, localZ);
	}

	return 0;
}

void World::UpdateChunkCache(int orginX, int orginZ) {
	if (orginX != cacheTranslationX || orginZ != cacheTranslationZ) {
		ChunkColumnPtr newCache[World::ChunkCacheSize][World::ChunkCacheSize];
		memcpy(newCache, columnCache, sizeof(newCache));

		int oldBufferStartX = cacheTranslationX - World::ChunkCacheSize / 2;
		int oldBufferStartZ = cacheTranslationZ - World::ChunkCacheSize / 2;

		int diffX = orginX - cacheTranslationX;
		int diffZ = orginZ - cacheTranslationZ;

		for (int x = 0; x < World::ChunkCacheSize; x++) {
			for (int z = 0; z < World::ChunkCacheSize; z++) {
				int worldX = orginX + (x - World::ChunkCacheSize / 2);
				int worldZ = orginZ + (z - World::ChunkCacheSize / 2);
				if (worldX >= oldBufferStartX && worldX < oldBufferStartX + World::ChunkCacheSize && worldZ >= oldBufferStartZ &&
                    worldZ < oldBufferStartZ + World::ChunkCacheSize) {
                    columnCache[x][z] = newCache[x + diffX][z + diffZ];
                    newCache[x + diffX][z + diffZ] = nullptr;
				} else {
                    columnCache[x][z] = LoadChunk(worldX, worldZ);
				}
			}
		}

		for (int x = 0; x < World::ChunkCacheSize; x++) {
			for (int z = 0; z < World::ChunkCacheSize; z++) {
				if (newCache[x][z] != nullptr) {
					UnloadChunk(newCache[x][z]);
				}
			}
		}

		cacheTranslationX = orginX;
		cacheTranslationZ = orginZ;
	}
}

void World::Release(ChunkWorker* chunkWorker, SaveManager* savemgr) {
	for (int i = 0; i < World::ChunkCacheSize; i++) {
		for (int j = 0; j < World::ChunkCacheSize; j++) {
			if (columnCache[i][j]) {
				UnloadChunk(columnCache[i][j]);
				columnCache[i][j] = nullptr;
			}
		}
	}

	chunkWorker->Finish();
	Reset();

	if (savemgr) savemgr->Unload();
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
    // This logic is ideally only executed on singleplayer worlds, since multiplayer servers handle chunk generation themselves and we just read the data.
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
                for (int xOffset = -1; xOffset < 2 && clear; xOffset++) {
                    for (int zOffset = -1; zOffset < 2 && clear; zOffset++) {
                        ChunkColumn *borderChunk = columnCache[x + xOffset][z + zOffset];
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

                BlockEvent_RandomTick(this,chunk,xVals,yVals,zVals);
            }
        }
    }
}