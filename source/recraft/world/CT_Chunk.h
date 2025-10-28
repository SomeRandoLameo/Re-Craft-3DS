#pragma once

#include "../blocks/CT_Block.h"

#include "../misc/Xorshift.h"
#include "../rendering/VBOCache.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define CHUNK_SIZE (16)
#define CHUNK_HEIGHT (128)
#define CLUSTER_PER_CHUNK (CHUNK_HEIGHT / CHUNK_SIZE)

typedef struct {
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
} Cluster;

typedef enum {
	ChunkGen_Empty,  //
	ChunkGen_Terrain,
	ChunkGen_Finished  // Terrain | Decoration
} ChunkGenProgress;

typedef struct {
	// Die Gesamtanzahl! >= graphicalTasksRunning
	uint32_t tasksRunning;
	uint32_t graphicalTasksRunning;

	uint32_t uuid;

	ChunkGenProgress genProgress;

	int x, z;
	Cluster clusters[CLUSTER_PER_CHUNK];

	uint8_t heightmap[CHUNK_SIZE][CHUNK_SIZE];
	uint32_t heightmapRevision;

	size_t revision;

	uint32_t displayRevision;
	bool forceVBOUpdate;

	int references;
} Chunk;

extern Xorshift32 uuidGenerator;
extern const uint8_t _seethroughTable[6][6];
uint16_t ChunkSeeThrough(Direction in, Direction out);

bool ChunkCanBeSeenThrough(uint16_t visiblity, Direction in, Direction out);

void Chunk_Init(Chunk* chunk, int x, int z);

void Chunk_RequestGraphicsUpdate(Chunk* chunk, int cluster);

void Chunk_GenerateHeightmap(Chunk* chunk);

uint8_t Chunk_GetHeightMap(Chunk* chunk, int x, int z);

uint8_t Chunk_GetMetadata(Chunk* chunk, int x, int y, int z);

void Chunk_SetMetadata(Chunk* chunk, int x, int y, int z, uint8_t metadata);

Block Chunk_GetBlock(Chunk* chunk, int x, int y, int z);

// resets the meta data
void Chunk_SetBlock(Chunk* chunk, int x, int y, int z, Block block);

void Chunk_SetBlockAndMeta(Chunk* chunk, int x, int y, int z, Block block, uint8_t metadata);

bool Cluster_IsEmpty(Cluster* cluster);