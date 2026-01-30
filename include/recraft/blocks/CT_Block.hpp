/*
    TODO: DEPRECATION NOTICE: THIS FILE WILL BE REMOVED AND REPLACED
        - PLEASE NO BIG MODIFICATIONS TO THIS FILE, AS IT WILL BE REWRITTEN FROM SCRATCH
        - MINOR MODIFICATIONS FOR BUGFIXES OR "GENERAL" IMPROVEMENTS ARE ALLOWED
*/

#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "../world/Direction.hpp"

#include "mclib/block/Block.h"
#include "mclib/inventory/Slot.h"


enum class BlockID : u16 {
    Air,
    Stone,
    Dirt,
    Grass,
    Cobblestone,
    Sand,
    Log,
    Gravel,
    Leaves,
    Glass,
   	Stonebrick,
    Brick,
    Planks,
    Wool,
    Bedrock,
    Coarse,
    Door_Top,
   	Door_Bottom,
   	Snow_Grass,
   	Snow,
   	Obsidian,
   	Netherrack,
   	Sandstone,
   	Smooth_Stone,
   	Crafting_Table,
   	Grass_Path,
   	Water,
   	Lava,
	Iron_Ore,
	Coal_Ore,
	Diamond_Ore,
	Gold_Ore,
	Emerald_Ore,
	Gold_Block,
	Diamond_Block,
	Coal_Block,
	Iron_Block,
	Emerald_Block,
	Furnace,
	Count
};

typedef u8 Metadata;

void Block_Init();
void Block_Deinit();

void* Block_GetTextureMap();

void Block_GetTexture(mc::inventory::Slot block, Direction direction, int16_t* out_uv);

void Block_GetColor(BlockID block, Metadata metadata, Direction direction, Metadata out_rgb[]);

bool Block_Opaque(BlockID block, Metadata metadata);

bool Block_Solid(BlockID block);