#pragma once

#include "../blocks/CT_Block.h"

#include "../misc/Xorshift.h"
#include "../rendering/VBOCache.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define CHUNK_SIZE (16)
#define CHUNK_HEIGHT (256)
#define CLUSTER_PER_CHUNK (CHUNK_HEIGHT / CHUNK_SIZE)

class Chunk {
public:
	int y;
	Block blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
	uint8_t metadataLight[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];  // first half metadata, second half light

	uint32_t revision;

	uint16_t seeThrough;

	bool empty;
	uint32_t emptyRevision;

	VBO_Block vbo, transparentVBO;
	size_t vertices, transparentVertices;
	uint32_t vboRevision;
	bool forceVBOUpdate;

    bool IsEmpty();
};

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
    uint32_t tasksRunning;
    uint32_t graphicalTasksRunning;

    uint32_t uuid;

    ChunkGenProgress genProgress;

    int x, z;
    Chunk chunks[CLUSTER_PER_CHUNK];

    uint8_t heightmap[CHUNK_SIZE][CHUNK_SIZE];
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

    uint8_t GetHeightMap(int x, int z);

    uint8_t GetMetadata(mc::Vector3i position);

    void SetMetadata(mc::Vector3i position, uint8_t metadata);

    Block GetBlock(mc::Vector3i position);

    void SetBlock(mc::Vector3i position, Block block);

    void SetBlockAndMeta(mc::Vector3i position, Block block, uint8_t metadata);
};

