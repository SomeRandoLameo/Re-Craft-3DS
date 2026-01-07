#pragma once

#include "../blocks/CT_Block.h"

#include "../misc/Xorshift.h"
#include "../rendering/VBOCache.h"

#include <array>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>




class Chunk {
public:
    enum { Size = 16 };

	int y;
	uint8_t metadataLight[Size][Size][Size];  // first half metadata, second half light

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

    Block GetBlock(int x, int y, int z) const {
        return m_blocks[x + y * Size + z * Size * Size];
    }

    void SetBlock(int x, int y, int z, Block block) {
        m_blocks[x + y * Size + z * Size * Size] = block;
    }

    bool IsEmpty();

    // raw data access for saving/loading only. Do not use for block manipulation!
    // this might become replaced with popper save load functions in the future.
    const Block* GetBlockData() const {
        return m_blocks.data();
    }

    Block* GetBlockData() {
        return m_blocks.data();
    }

    static constexpr size_t BlockCount = Size * Size * Size;

    static constexpr size_t GetBlockDataSize() {
        return BlockCount * sizeof(Block);
    }
private:
    std::array<Block, Size * Size * Size> m_blocks;
};

typedef Chunk* ChunkPtr;

typedef enum {
	ChunkGen_Empty,  //
	ChunkGen_Terrain,
	ChunkGen_Finished  // Terrain | Decoration
} ChunkGenProgress;

extern Xorshift32 uuidGenerator;
extern const uint8_t _seethroughTable[6][6];

uint16_t ChunkSeeThrough(Direction in, Direction out);
bool ChunkCanBeSeenThrough(uint16_t visiblity, Direction in, Direction out);


class ChunkColumn {
public:
    enum { ChunksPerColumn = 16 };

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

    uint8_t GetMetadata(mc::Vector3i position);

    void SetMetadata(mc::Vector3i position, uint8_t metadata);

    Block GetBlock(mc::Vector3i position);

    void SetBlock(mc::Vector3i position, Block block);

    void SetBlockAndMeta(mc::Vector3i position, Block block, uint8_t metadata);
private:
    std::array<Chunk, ChunksPerColumn> m_chunks;
};

typedef ChunkColumn* ChunkColumnPtr;
