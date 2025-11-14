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
