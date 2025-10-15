#pragma once

#include "../entity/Player.h"
#include "../world/CT_World.h"

void CommandLine_Activate(World* world, Player* player);
void CommandLine_Execute(World* world, Player* player, const char* text);