#pragma once

#include "world/CT_Chunk.hpp"

#define RANDOMTICKS_PER_CLUSTER (3)
#define RANDOMTICKS_PER_CHUNK (ChunkColumn::ChunksPerColumn * RANDOMTICKS_PER_CLUSTER)
class World;
void BlockEvent_RandomTick(World* world, ChunkColumnPtr chunk, int x[], int y[], int z[]);
