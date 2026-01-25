#pragma once

#include "../blocks/CT_Block.hpp"

#include "../misc/Xorshift.hpp"
#include "../rendering/VBOCache.hpp"
#include "mclib/block/BlockEntity.h"

#include <array>
#include <stdbool.h>

#include <string.h>


class Chunk {
public:
    constexpr static int Size = 16;

    int      y;
    Metadata metadataLight[Size][Size][Size]; // first half metadata, second half light

    u32 revision;

    u16 seeThrough;

    bool empty;
    u32  emptyRevision;

    VBOBlock vbo;
    VBOBlock transparentVBO;

    size_t vertices;
    size_t transparentVertices;

    u32  vboRevision;
    bool forceVBOUpdate;

    Block GetBlock(mc::Vector3i pos) const { return m_blocks[pos.x + pos.y * Size + pos.z * Size * Size]; }

    /// DO NOT USE THIS MANUALLY
    void SetBlock(mc::Vector3i pos, Block block) { m_blocks[pos.x + pos.y * Size + pos.z * Size * Size] = block; }

    // TODO: REMOVE
    Block GetBlock(int x, int y, int z) const { return m_blocks[x + y * Size + z * Size * Size]; }

    bool IsEmpty();

    // raw data access for saving/loading only. Do not use for block manipulation!
    // this might become replaced with popper save load functions in the future.
    const Block* GetBlockData() const { return m_blocks.data(); }

    Block* GetBlockData() { return m_blocks.data(); }

    static constexpr size_t BlockCount = Size * Size * Size;

    static constexpr size_t GetBlockDataSize() { return BlockCount * sizeof(Block); }

private:
    std::array<Block, Size * Size * Size> m_blocks;
};

typedef Chunk* ChunkPtr;

typedef enum {
    ChunkGen_Empty, //
    ChunkGen_Terrain,
    ChunkGen_Finished // Terrain | Decoration
} ChunkGenProgress;

extern Xorshift32 uuidGenerator;
extern const u8   _seethroughTable[6][6];

u16  ChunkSeeThrough(Direction in, Direction out);
bool ChunkCanBeSeenThrough(u16 visiblity, Direction in, Direction out);


class ChunkColumn {
public:
    constexpr static int ChunksPerColumn = 16;

    u32 tasksRunning;
    u32 graphicalTasksRunning;
    u32 uuid;

    ChunkGenProgress genProgress;

    int x;
    int z;

    u8  heightmap[Chunk::Size][Chunk::Size];
    u32 heightmapRevision;

    size_t revision;

    u32  displayRevision;
    bool forceVBOUpdate;

    int references;

    ChunkColumn();
    ChunkColumn(int x, int z);
    ~ChunkColumn() = default;
    void RequestGraphicsUpdate(int cluster);

    void GenerateHeightmap();

    ChunkPtr GetChunk(int y);

    u8 GetHeightMap(int x, int z);

    Metadata GetMetadata(mc::Vector3i position);

    void SetMetadata(mc::Vector3i position, Metadata metadata);

    Block GetBlock(mc::Vector3i position);

    void SetBlock(mc::Vector3i position, Block block);

    void SetBlockAndMeta(mc::Vector3i position, Block block, Metadata metadata);

    void AddBlockEntity(mc::block::BlockEntityPtr blockEntity) {
        m_BlockEntities.insert(std::make_pair(blockEntity->GetPosition(), blockEntity));
    }

    void RemoveBlockEntity(mc::Vector3i pos) { m_BlockEntities.erase(pos); }

    mc::block::BlockEntityPtr              GetBlockEntity(mc::Vector3i worldPos);
    std::vector<mc::block::BlockEntityPtr> GetBlockEntities();

private:
    std::array<Chunk, ChunksPerColumn>                m_chunks;
    std::map<mc::Vector3i, mc::block::BlockEntityPtr> m_BlockEntities;
};

typedef ChunkColumn* ChunkColumnPtr;
