#include "world/CT_Chunk.hpp"
#include <climits>

Xorshift32 uuidGenerator = (Xorshift32)314159265;

const u8 _seethroughTable[6][6] = {
    // W E B T N S
    {255, 0, 1, 3, 6, 10}, // West
    {0, 255, 2, 4, 7, 11}, // East
    {1, 2, 255, 5, 8, 12}, // Bottom
    {3, 4, 5, 255, 9, 13}, // Top
    {6, 7, 8, 9, 255, 14}, // North
    {10, 11, 12, 13, 14, 255} // South
};

u16 ChunkSeeThrough(Direction in, Direction out) {
    return 1 << (u16)(_seethroughTable[in][out]);
}

bool ChunkCanBeSeenThrough(u16 visiblity, Direction in, Direction out) {
    return visiblity & (1 << (u16)(_seethroughTable[in][out]));
}

ChunkColumn::ChunkColumn() {
    memset(this, 0, sizeof(ChunkColumn));

    this->x = INT_MAX;
    this->z = INT_MAX;
    for (int i = 0; i < ChunksPerColumn; i++) {
        auto chunk        = this->GetChunk(i);
        chunk->y          = i;
        chunk->seeThrough = UINT16_MAX;
        chunk->empty      = true;
    }
    this->uuid = Xorshift32_Next(&uuidGenerator);
}

ChunkColumn::ChunkColumn(int x, int z) {
    memset(this, 0, sizeof(ChunkColumn));

    this->x = x;
    this->z = z;
    for (int i = 0; i < ChunksPerColumn; i++) {
        auto chunk        = this->GetChunk(i);
        chunk->y          = i;
        chunk->seeThrough = UINT16_MAX;
        chunk->empty      = true;
    }
    this->uuid = Xorshift32_Next(&uuidGenerator);
}

void ChunkColumn::RequestGraphicsUpdate(int cluster) {
    GetChunk(cluster)->forceVBOUpdate = true;
    forceVBOUpdate                    = true;
}

void ChunkColumn::GenerateHeightmap() {
    if (heightmapRevision != revision) {
        for (int x = 0; x < Chunk::Size; x++) {
            for (int z = 0; z < Chunk::Size; z++) {
                for (int i = ChunksPerColumn - 1; i >= 0; --i) {
                    auto chunk = GetChunk(i);
                    if (chunk->IsEmpty()) {
                        continue;
                    }
                    for (int j = Chunk::Size - 1; j >= 0; --j) {
                        if (chunk->GetBlock(x, j, z) != Block::Air) {
                            heightmap[x][z] = i * Chunk::Size + j + 1;
                            i               = -1;
                            break;
                        }
                    }
                }
            }
        }
    }
    heightmapRevision = revision;
}

u8 ChunkColumn::GetHeightMap(int x, int z) {
    GenerateHeightmap();
    return heightmap[x][z];
}

Metadata ChunkColumn::GetMetadata(mc::Vector3i position) {
    return GetChunk(position.y / Chunk::Size)
               ->metadataLight[position.x][position.y - (position.y / Chunk::Size * Chunk::Size)][position.z] &
           0xf;
}

void ChunkColumn::SetMetadata(mc::Vector3i position, Metadata metadata) {
    metadata &= 0xf;
    ChunkPtr chunk = GetChunk(position.y / Chunk::Size);
    u8* addr = &chunk->metadataLight[position.x][position.y - (position.y / Chunk::Size * Chunk::Size)][position.z];
    *addr    = (*addr & 0xf0) | metadata;
    ++chunk->revision;
    ++revision;
}

Block ChunkColumn::GetBlock(mc::Vector3i position) {
    return GetChunk(position.y / Chunk::Size)
        ->GetBlock(position.x, position.y - (position.y / Chunk::Size * Chunk::Size), position.z);
}


// resets the meta data
/// DO NOT USE THIS MANUALLY, AS THIS WONT UPDATE RENDERING-RELATED DATA
void ChunkColumn::SetBlock(mc::Vector3i position, Block block) {
    ChunkPtr     chunk    = GetChunk(position.y / Chunk::Size);
    mc::Vector3i localPos = position;
    localPos.y            = position.y % Chunk::Size;
    chunk->SetBlock(localPos, block);
    SetMetadata(position, 0);
}

void ChunkColumn::SetBlockAndMeta(mc::Vector3i position, Block block, u8 metadata) {
    ChunkPtr     chunk    = GetChunk(position.y / Chunk::Size);
    mc::Vector3i localPos = position;
    localPos.y            = position.y % Chunk::Size;
    chunk->SetBlock(localPos, block);
    metadata &= 0xf;
    u8* addr = &chunk->metadataLight[localPos.x][localPos.y][localPos.z];
    *addr    = (*addr & 0xf0) | metadata;

    ++chunk->revision;
    ++revision;
}

ChunkPtr ChunkColumn::GetChunk(int y) {
    return &m_chunks[y];
}

bool Chunk::IsEmpty() {
    if (emptyRevision == revision) {
        return empty;
    }

    emptyRevision = revision;

    for (Block block : m_blocks) {
        if (block != Block::Air) {
            empty = false;
            return false;
        }
    }

    empty = true;
    return true;
}
