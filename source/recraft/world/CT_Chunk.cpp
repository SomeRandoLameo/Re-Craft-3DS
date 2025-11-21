#include "world/CT_Chunk.h"

Xorshift32 uuidGenerator = (Xorshift32)314159265;

const uint8_t _seethroughTable[6][6] = {
    // W E B T N S
    {255, 0, 1, 3, 6, 10},     // West
    {0, 255, 2, 4, 7, 11},     // East
    {1, 2, 255, 5, 8, 12},     // Bottom
    {3, 4, 5, 255, 9, 13},     // Top
    {6, 7, 8, 9, 255, 14},     // North
    {10, 11, 12, 13, 14, 255}  // South
};

uint16_t ChunkSeeThrough(Direction in, Direction out) {
    return 1 << (uint16_t)(_seethroughTable[in][out]);
}

bool ChunkCanBeSeenThrough(uint16_t visiblity, Direction in, Direction out) {
    return visiblity & (1 << (uint16_t)(_seethroughTable[in][out]));
}

void Chunk_Init(Chunk* chunk, int x, int z) {
    memset(chunk, 0, sizeof(Chunk));

    chunk->x = x;
    chunk->z = z;
    for (int i = 0; i < CLUSTER_PER_CHUNK; i++) {
        chunk->clusters[i].y = i;
        chunk->clusters[i].seeThrough = UINT16_MAX;
        chunk->clusters[i].empty = true;
    }
    chunk->uuid = Xorshift32_Next(&uuidGenerator);
}

void Chunk_RequestGraphicsUpdate(Chunk* chunk, int cluster) {
    chunk->clusters[cluster].forceVBOUpdate = true;
    chunk->forceVBOUpdate = true;
}

void Chunk_GenerateHeightmap(Chunk* chunk) {
	if (chunk->heightmapRevision != chunk->revision){
		for (int x = 0; x < CHUNK_SIZE; x++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                for (int i = CLUSTER_PER_CHUNK - 1; i >= 0; --i) {
                    if (Cluster_IsEmpty(&chunk->clusters[i])) continue;
                    for (int j = CHUNK_SIZE - 1; j >= 0; --j) {
                        if (chunk->clusters[i].blocks[x][j][z] != Block_Air) {
                            chunk->heightmap[x][z] = i * CHUNK_SIZE + j + 1;
                            i = -1;
                            break;
                        }
                    }
                }
            }
        }
    }
	chunk->heightmapRevision = chunk->revision;
}

uint8_t Chunk_GetHeightMap(Chunk* chunk, int x, int z) {
    Chunk_GenerateHeightmap(chunk);
    return chunk->heightmap[x][z];
}

uint8_t Chunk_GetMetadata(Chunk* chunk, mc::Vector3i position) {
    return chunk->clusters[position.y / CHUNK_SIZE].metadataLight[position.x][position.y - (position.y / CHUNK_SIZE * CHUNK_SIZE)][position.z] & 0xf;
}

void Chunk_SetMetadata(Chunk* chunk, mc::Vector3i position, uint8_t metadata) {
    metadata &= 0xf;
    Cluster* cluster = &chunk->clusters[position.y / CHUNK_SIZE];
    uint8_t* addr = &cluster->metadataLight[position.x][position.y - (position.y / CHUNK_SIZE * CHUNK_SIZE)][position.z];
    *addr = (*addr & 0xf0) | metadata;
    ++cluster->revision;
    ++chunk->revision;
}

Block Chunk_GetBlock(Chunk* chunk, mc::Vector3i position) {
    return chunk->clusters[position.y / CHUNK_SIZE].blocks[position.x][position.y - (position.y / CHUNK_SIZE * CHUNK_SIZE)][position.z];
}


// resets the meta data
void Chunk_SetBlock(Chunk* chunk, mc::Vector3i position, Block block) {
    Cluster* cluster = &chunk->clusters[position.y / CHUNK_SIZE];
    cluster->blocks[position.x][position.y - (position.y / CHUNK_SIZE * CHUNK_SIZE)][position.z] = block;
    Chunk_SetMetadata(chunk, position, 0);
}

void Chunk_SetBlockAndMeta(Chunk* chunk, mc::Vector3i position, Block block, uint8_t metadata) {
    Cluster* cluster = &chunk->clusters[position.y / CHUNK_SIZE];
    cluster->blocks[position.x][position.y - (position.y / CHUNK_SIZE * CHUNK_SIZE)][position.z] = block;
    metadata &= 0xf;
    uint8_t* addr = &cluster->metadataLight[position.x][position.y - (position.y / CHUNK_SIZE * CHUNK_SIZE)][position.z];
    *addr = (*addr & 0xf0) | metadata;

    ++cluster->revision;
    ++chunk->revision;
}

bool Cluster_IsEmpty(Cluster* cluster) {
	if (cluster->emptyRevision == cluster->revision) return cluster->empty;
	cluster->empty = false;
	cluster->emptyRevision = cluster->revision;
	for (int i = 0; i < sizeof(cluster->blocks) / sizeof(uint32_t); i++) {
		if (((uint32_t*)cluster->blocks)[i] != 0) return false;
	}
	cluster->empty = true;
	return true;
}