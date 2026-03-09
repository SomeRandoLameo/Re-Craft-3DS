#pragma once

#include "client/entity/Player.hpp"
#include "client/gui/DebugUI.hpp"
#include "world/World.hpp"

void CommandLine_Activate(World* world, Player* player, DebugUI* debugUi);
void CommandLine_Execute(World* world, Player* player, DebugUI* debugUi, const char* text);
