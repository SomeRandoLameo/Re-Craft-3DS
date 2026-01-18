#pragma once

#include "../world/WorkQueue.h"
#include "../world/CT_World.h"
#include "../entity/Player.h"
#include "../gui/DebugUI.h"

#include "VertexFmt.h"


static std::vector<VBOUpdate> vboUpdates;

static const int MAX_FACES_PER_CLUSTER = (Chunk::Size * Chunk::Size * Chunk::Size / 2 * 6);

typedef struct {
    int8_t x, y, z;
    Direction direction;
    Block block;
    int8_t ao;
    uint8_t metadata;
    bool transparent;
} Face;

static World* world;
static Player* player;

typedef struct { int8_t x, y, z; } QueueElement;

static std::vector<QueueElement> floodfill_queue;

static LightLock updateLock;

static Face faceBuffer[MAX_FACES_PER_CLUSTER];
static int currentFace;
static int transparentFaces;
static uint8_t floodfill_visited[Chunk::Size][Chunk::Size][Chunk::Size];

void PolyGen_Init(World* world_, Player* player_);
void PolyGen_Deinit();

void PolyGen_Harvest(DebugUI* debugUi);
void PolyGen_GeneratePolygons(WorkQueue* queue, WorkerItem item, void* context);

Block fastBlockFetch(World* world, ChunkColumnPtr column, ChunkPtr chunk, mc::Vector3i blockPos);

uint8_t fastMetadataFetch(World* world, ChunkColumnPtr column, ChunkPtr chunk, mc::Vector3i blockPos);

void addFace(mc::Vector3i facePos, Direction dir, Block block, uint8_t metadata, int ao, bool transparent);

uint16_t floodFill(Chunk* cluster, mc::Vector3i pos, Direction entrySide0, Direction entrySide1, Direction entrySide2);

/*
#pragma once

#include "../world/WorkQueue.h"
#include "../world/CT_World.h"
#include "../entity/Player.h"
#include "../gui/DebugUI.h"

#include "VertexFmt.h"

void PolyGen_Init(World* world_, Player* player_);
void PolyGen_Deinit();

void PolyGen_Harvest(DebugUI* debugUi);
void PolyGen_GeneratePolygons(WorkQueue* queue, WorkerItem item, void* context);
*/