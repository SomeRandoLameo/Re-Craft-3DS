#pragma once

#include "block/Block.hpp"

#include "client/renderer/VBOCache.hpp"
#include "mclib/block/BlockEntity.h"
#include "misc/Xorshift.hpp"

#include <array>

#include <stdint.h>
#include <string.h>

class Chunk {
public:
    constexpr static int Size = 16;
    constexpr static size_t BlockCount = Size * Size * Size;

    int y;
    Metadata metadataLight[Size][Size][Size];

    uint32_t revision;

    uint16_t seeThrough;

    bool empty;
    uint32_t emptyRevision;

    VBOBlock vbo;
    VBOBlock transparentVBO;

    size_t vertices;
    size_t transparentVertices;

    uint32_t vboRevision;
    bool forceVBOUpdate;

    BlockID GetBlockID(mc::Vector3i pos) const {
        return BlockRegistry::GetBlockID(m_blocks[BlockIndex(pos)]);
    }

    /// DO NOT USE THIS MANUALLY
    void SetBlockID(mc::Vector3i pos, BlockID block) {
        m_blocks[BlockIndex(pos)] = static_cast<u8>(BlockRegistry::GetId(block));
    }

    BlockPtr GetBlock(mc::Vector3i pos) const {
        return const_cast<BlockPtr>(BlockRegistry::GetBlock((BlockID)m_blocks[BlockIndex(pos)]));
    }

    /// DO NOT USE THIS MANUALLY
    void SetBlock(mc::Vector3i pos, BlockPtr block) {
        m_blocks[BlockIndex(pos)] = static_cast<u8>(BlockRegistry::GetId(block->GetID()));
    }

    // TODO: REMOVE
    BlockID GetBlockID(int x, int y, int z) const { return GetBlockID(mc::Vector3i(x, y, z)); }

    BlockPtr GetBlock(int x, int y, int z) const { return GetBlock(mc::Vector3i(x, y, z)); }

    bool IsEmpty();

    // raw data access for saving/loading only. Do not use for block manipulation!
    const u8* GetBlockData() const { return m_blocks; }

    u8* GetBlockData() { return m_blocks; }

    static constexpr size_t GetBlockDataSize() { return BlockCount; }

private:
    u8 m_blocks[BlockCount];

    static int BlockIndex(mc::Vector3i pos) {
        return pos.x + pos.y * Size + pos.z * Size * Size;
    }
};

typedef Chunk* ChunkPtr;

typedef enum {
    ChunkGen_Empty, //
    ChunkGen_Terrain,
    ChunkGen_Finished // Terrain | Decoration
} ChunkGenProgress;

extern Xorshift32 uuidGenerator;
extern const uint8_t _seethroughTable[6][6];

uint16_t ChunkSeeThrough(Direction in, Direction out);
bool ChunkCanBeSeenThrough(uint16_t visiblity, Direction in, Direction out);


class ChunkColumn {
public:
    constexpr static int ChunksPerColumn = 16;

    uint32_t tasksRunning;
    uint32_t graphicalTasksRunning;
    uint32_t uuid;

    ChunkGenProgress genProgress;

    int x;
    int z;

    uint8_t heightmap[Chunk::Size][Chunk::Size];
    uint32_t heightmapRevision;

    size_t revision;

    uint32_t displayRevision;
    bool forceVBOUpdate;

    int references;

    ChunkColumn();
    ChunkColumn(int x, int z);
    ~ChunkColumn() = default;
    void RequestGraphicsUpdate(int cluster);

    void GenerateHeightmap();

    ChunkPtr GetChunk(int y);

    uint8_t GetHeightMap(int x, int z);

    Metadata GetMetadata(mc::Vector3i position);

    void SetMetadata(mc::Vector3i position, Metadata metadata);

    BlockID GetBlockID(mc::Vector3i position);

    BlockPtr GetBlock(mc::Vector3i position);

    void SetBlockID(mc::Vector3i position, BlockID block);

    void SetBlock(mc::Vector3i position, BlockPtr block);

    void SetBlockAndMeta(mc::Vector3i position, BlockID block, Metadata metadata);

    void AddBlockEntity(mc::block::BlockEntityPtr blockEntity) {
        m_BlockEntities.insert(std::make_pair(blockEntity->GetPosition(), blockEntity));
    }

    void RemoveBlockEntity(mc::Vector3i pos) { m_BlockEntities.erase(pos); }

    mc::block::BlockEntityPtr GetBlockEntity(mc::Vector3i worldPos);
    std::vector<mc::block::BlockEntityPtr> GetBlockEntities();

private:
    std::array<Chunk, ChunksPerColumn> m_chunks;
    std::map<mc::Vector3i, mc::block::BlockEntityPtr> m_BlockEntities;
};

typedef ChunkColumn* ChunkColumnPtr;
