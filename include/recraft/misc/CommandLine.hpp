#pragma once

class World;
class Player;
class DebugUI;
void CommandLine_Activate(World* world, Player* player, DebugUI* debugUi);
void CommandLine_Execute(World* world, Player* player, DebugUI* debugUi, const char* text);
