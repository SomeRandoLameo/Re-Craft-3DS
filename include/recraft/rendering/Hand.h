#pragma once

#include <citro3d.h>

#include "../entity/Player.h"
#include "../blocks/CT_Block.h"

void Hand_Init();
void Hand_Deinit();

void Hand_Draw(int projUniform, C3D_Mtx* projection, mc::inventory::Slot stack, Player* player);