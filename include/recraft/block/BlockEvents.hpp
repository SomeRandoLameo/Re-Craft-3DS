#pragma once

#include "Block.hpp"
#include "world/World.hpp"

#define RANDOMTICKS_PER_CLUSTER (3)
#define RANDOMTICKS_PER_CHUNK (ChunkColumn::ChunksPerColumn * RANDOMTICKS_PER_CLUSTER)
void BlockEvent_RandomTick(World* world, ChunkColumnPtr chunk, int x[], int y[], int z[]);
