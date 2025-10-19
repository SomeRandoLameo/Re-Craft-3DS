#pragma once

#include "../entity/Player.h"
#include "../world/CT_World.h"
#include "../gui/DebugUI.h"

void CommandLine_Activate(World* world, Player* player, DebugUI* debugUi);
void CommandLine_Execute(World* world, Player* player, DebugUI* debugUi, const char* text);