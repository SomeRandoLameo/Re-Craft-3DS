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

ChunkColumn::ChunkColumn() {
    memset(this, 0, sizeof(ChunkColumn));

    this->x = INT_MAX;
    this->z = INT_MAX;
    for (int i = 0; i < CHUNKS_PER_COLUMN; i++) {
        auto chunk = this->GetChunk(i);
        chunk->y = i;
        chunk->seeThrough = UINT16_MAX;
        chunk->empty = true;
    }
    this->uuid = Xorshift32_Next(&uuidGenerator);
}

ChunkColumn::ChunkColumn(int x, int z) {
    memset(this, 0, sizeof(ChunkColumn));

    this->x = x;
    this->z = z;
    for (int i = 0; i < CHUNKS_PER_COLUMN; i++) {
        auto chunk = this->GetChunk(i);
        chunk->y = i;
        chunk->seeThrough = UINT16_MAX;
        chunk->empty = true;
    }
    this->uuid = Xorshift32_Next(&uuidGenerator);
}

void ChunkColumn::RequestGraphicsUpdate(int cluster) {
    GetChunk(cluster)->forceVBOUpdate = true;
    forceVBOUpdate = true;
}

void ChunkColumn::GenerateHeightmap() {
	if (heightmapRevision != revision) {
		for (int x = 0; x < CHUNK_SIZE; x++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                for (int i = CHUNKS_PER_COLUMN - 1; i >= 0; --i) {
                    auto chunk = GetChunk(i);
                    if (chunk->IsEmpty()) continue;
                    for (int j = CHUNK_SIZE - 1; j >= 0; --j) {
                        if (chunk->blocks[x][j][z] != Block_Air) {
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

uint8_t ChunkColumn::GetHeightMap(int x, int z) {
    GenerateHeightmap();
    return heightmap[x][z];
}

uint8_t ChunkColumn::GetMetadata(mc::Vector3i position) {
    return GetChunk(position.y / CHUNK_SIZE)->metadataLight[position.x][position.y - (position.y / CHUNK_SIZE * CHUNK_SIZE)][position.z] & 0xf;
}

void ChunkColumn::SetMetadata(mc::Vector3i position, uint8_t metadata) {
    metadata &= 0xf;
    ChunkPtr chunk = GetChunk(position.y / CHUNK_SIZE);
    uint8_t* addr = &chunk->metadataLight[position.x][position.y - (position.y / CHUNK_SIZE * CHUNK_SIZE)][position.z];
    *addr = (*addr & 0xf0) | metadata;
    ++chunk->revision;
    ++revision;
}

Block ChunkColumn::GetBlock(mc::Vector3i position) {
    return GetChunk(position.y / CHUNK_SIZE)->blocks[position.x][position.y - (position.y / CHUNK_SIZE * CHUNK_SIZE)][position.z];
}


// resets the meta data
void ChunkColumn::SetBlock(mc::Vector3i position, Block block) {
    ChunkPtr chunk = GetChunk(position.y / CHUNK_SIZE);
    chunk->blocks[position.x][position.y - (position.y / CHUNK_SIZE * CHUNK_SIZE)][position.z] = block;
    SetMetadata(position, 0);
}

void ChunkColumn::SetBlockAndMeta(mc::Vector3i position, Block block, uint8_t metadata) {
    ChunkPtr chunk = GetChunk(position.y / CHUNK_SIZE);
    chunk->blocks[position.x][position.y - (position.y / CHUNK_SIZE * CHUNK_SIZE)][position.z] = block;
    metadata &= 0xf;
    uint8_t* addr = &chunk->metadataLight[position.x][position.y - (position.y / CHUNK_SIZE * CHUNK_SIZE)][position.z];
    *addr = (*addr & 0xf0) | metadata;

    ++chunk->revision;
    ++revision;
}

ChunkPtr ChunkColumn::GetChunk(int y) {
    return &m_chunks[y];
}

bool Chunk::IsEmpty() {
	if (emptyRevision == revision) return empty;
	empty = false;
	emptyRevision = revision;
	for (int i = 0; i < sizeof(blocks) / sizeof(uint32_t); i++) {
		if (((uint32_t*)blocks)[i] != 0) return false;
	}
	empty = true;
	return true;
}