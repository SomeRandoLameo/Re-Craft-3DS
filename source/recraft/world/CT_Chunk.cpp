#include <climits>
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

Chunk::Chunk() {
    memset(this, 0, sizeof(Chunk));

    this->x = INT_MAX;
    this->z = INT_MAX;
    for (int i = 0; i < CLUSTER_PER_CHUNK; i++) {
        this->clusters[i].y = i;
        this->clusters[i].seeThrough = UINT16_MAX;
        this->clusters[i].empty = true;
    }
    this->uuid = Xorshift32_Next(&uuidGenerator);
}

Chunk::Chunk(int x, int z) {
    memset(this, 0, sizeof(Chunk));

    this->x = x;
    this->z = z;
    for (int i = 0; i < CLUSTER_PER_CHUNK; i++) {
        this->clusters[i].y = i;
        this->clusters[i].seeThrough = UINT16_MAX;
        this->clusters[i].empty = true;
    }
    this->uuid = Xorshift32_Next(&uuidGenerator);
}

void Chunk::RequestGraphicsUpdate(int cluster) {
    clusters[cluster].forceVBOUpdate = true;
    forceVBOUpdate = true;
}

void Chunk::GenerateHeightmap() {
	if (heightmapRevision != revision) {
		for (int x = 0; x < CHUNK_SIZE; x++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                for (int i = CLUSTER_PER_CHUNK - 1; i >= 0; --i) {
                    if (Cluster_IsEmpty(&clusters[i])) continue;
                    for (int j = CHUNK_SIZE - 1; j >= 0; --j) {
                        if (clusters[i].blocks[x][j][z] != Block_Air) {
                            heightmap[x][z] = i * CHUNK_SIZE + j + 1;
                            i = -1;
                            break;
                        }
                    }
                }
            }
        }
    }
	heightmapRevision = revision;
}

uint8_t Chunk::GetHeightMap(int x, int z) {
    GenerateHeightmap();
    return heightmap[x][z];
}

uint8_t Chunk::GetMetadata(mc::Vector3i position) {
    return clusters[position.y / CHUNK_SIZE].metadataLight[position.x][position.y - (position.y / CHUNK_SIZE * CHUNK_SIZE)][position.z] & 0xf;
}

void Chunk::SetMetadata(mc::Vector3i position, uint8_t metadata) {
    metadata &= 0xf;
    Cluster* cluster = &clusters[position.y / CHUNK_SIZE];
    uint8_t* addr = &cluster->metadataLight[position.x][position.y - (position.y / CHUNK_SIZE * CHUNK_SIZE)][position.z];
    *addr = (*addr & 0xf0) | metadata;
    ++cluster->revision;
    ++revision;
}

Block Chunk::GetBlock(mc::Vector3i position) {
    return clusters[position.y / CHUNK_SIZE].blocks[position.x][position.y - (position.y / CHUNK_SIZE * CHUNK_SIZE)][position.z];
}


// resets the meta data
void Chunk::SetBlock(mc::Vector3i position, Block block) {
    Cluster* cluster = &clusters[position.y / CHUNK_SIZE];
    cluster->blocks[position.x][position.y - (position.y / CHUNK_SIZE * CHUNK_SIZE)][position.z] = block;
    SetMetadata(position, 0);
}

void Chunk::SetBlockAndMeta(mc::Vector3i position, Block block, uint8_t metadata) {
    Cluster* cluster = &clusters[position.y / CHUNK_SIZE];
    cluster->blocks[position.x][position.y - (position.y / CHUNK_SIZE * CHUNK_SIZE)][position.z] = block;
    metadata &= 0xf;
    uint8_t* addr = &cluster->metadataLight[position.x][position.y - (position.y / CHUNK_SIZE * CHUNK_SIZE)][position.z];
    *addr = (*addr & 0xf0) | metadata;

    ++cluster->revision;
    ++revision;
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