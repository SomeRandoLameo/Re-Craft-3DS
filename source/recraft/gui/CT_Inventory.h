#pragma once

#include "../inventory/ItemStack.h"
#include "../GameStates.h"
#include "../misc/NumberUtils.h"


#define INVENTORY_QUICKSELECT_MAXSLOTS 9
#define INVENTORY_QUICKSELECT_HEIGHT (22 + 1)

#define INVENTORY_MAX_PER_SITE (32)

class ReCraftCore;

static inline int Inventory_QuickSelectCalcSlots() { return INVENTORY_QUICKSELECT_MAXSLOTS; }
static inline int Inventory_QuickSelectCalcWidth(int slots) { return 54 + (slots - 2) * 20; }

void Inventory_renderHotbar(int x, int y, ItemStack* stacks, int count, int* selected);

int Inventory_Draw(int x, int y, int w, ItemStack* stacks, int count, int site);