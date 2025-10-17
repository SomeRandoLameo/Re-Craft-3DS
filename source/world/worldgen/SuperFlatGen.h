#pragma once

#include "../WorkQueue.h"
#include "../CT_World.h"

typedef struct { World* world; } SuperFlatGen;

void SuperFlatGen_Init(SuperFlatGen* gen, World* world);

void SuperFlatGen_Generate(WorkQueue* queue, WorkerItem item, void* context);
