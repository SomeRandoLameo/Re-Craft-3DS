#pragma once

#include "blocks/CT_Block.hpp"
#include "world/CT_Chunk.hpp"
#include "world/Direction.hpp"
#include "world/WorkQueue.hpp"

static std::vector<VBOUpdate> vboUpdates;

static const int MAX_FACES_PER_CLUSTER = (Chunk::Size * Chunk::Size * Chunk::Size / 2 * 6);

class DebugUI;
class Player;
class World;

struct Face {
    s8        x, y, z;
    Direction direction;
    Block     block;
    s8        ao;
    u8        metadata;
    bool      transparent;
};

static World*  world;
static Player* player;

struct QueueElement {
    s8 x, y, z;
};

static std::vector<QueueElement> floodfill_queue;

static LightLock updateLock;

static Face faceBuffer[MAX_FACES_PER_CLUSTER];
static int  currentFace;
static int  transparentFaces;
static u8   floodfill_visited[Chunk::Size][Chunk::Size][Chunk::Size];

void PolyGen_Init(World* world_, Player* player_);
void PolyGen_Deinit();

void PolyGen_Harvest(DebugUI* debugUi);
void PolyGen_GeneratePolygons(WorkQueue* queue, WorkerItem item, void* context);

Block fastBlockFetch(World* world, ChunkColumnPtr column, ChunkPtr chunk, int x, int y, int z);

u8 fastMetadataFetch(World* world, ChunkColumnPtr column, ChunkPtr chunk, int x, int y, int z);

void addFace(int x, int y, int z, Direction dir, Block block, u8 metadata, int ao, bool transparent);

u16 floodFill(World* world, ChunkColumnPtr chunk, Chunk* cluster, int x, int y, int z, Direction entrySide0,
              Direction entrySide1, Direction entrySide2);
