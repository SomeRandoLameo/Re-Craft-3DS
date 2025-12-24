#pragma once


#include <cstdint>

#include "../world/Direction.h"

#include "mclib/block/Block.h"
#include "mclib/inventory/Slot.h"

void Block_Init();
void Block_Deinit();

void* Block_GetTextureMap();

void Block_GetTexture(mc::block::BlockPtr block, Direction direction, int16_t* out_uv);

void Block_GetColor(mc::block::BlockPtr block, uint8_t out_rgb[]);