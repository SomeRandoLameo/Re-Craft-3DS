#pragma once

#include "../CT_World.h"

typedef struct { World* world; } SmeaGen;

void SmeaGen_Init(SmeaGen* gen, World* world);
void SmeaGen_Generate(WorkQueue* queue, WorkerItem item, void* this);