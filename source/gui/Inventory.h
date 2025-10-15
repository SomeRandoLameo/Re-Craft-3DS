#pragma once

#include "../inventory/ItemStack.h"

#include "../misc/NumberUtils.h"

#define INVENTORY_QUICKSELECT_MAXSLOTS 9
#define INVENTORY_QUICKSELECT_HEIGHT (22 + 1)

#define INVENTORY_MAX_PER_SITE (32)

inline int Inventory_QuickSelectCalcSlots(int screenwidth) { return MIN(INVENTORY_QUICKSELECT_MAXSLOTS, (screenwidth - 21 * 2) / 20 + 2); }
inline int Inventory_QuickSelectCalcWidth(int slots) { return 54 + (slots - 2) * 20; }

void Inventory_DrawQuickSelect(int x, int y, ItemStack* stacks, int count, int* selected);

int Inventory_Draw(int x, int y, int w, ItemStack* stacks, int count, int site);