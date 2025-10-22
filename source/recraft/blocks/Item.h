#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "../world/Direction.h"

typedef uint8_t Item;

enum {
    Item_Totem,
	Items_Count
};

void Item_Init();
void Item_Deinit();

void* Item_GetTextureMap();

void Item_GetTexture(Item items, Direction direction, uint8_t metadata, int16_t* out_uv);

extern const char* ItemNames[Items_Count];
