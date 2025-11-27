#pragma once

#include "../inventory/ItemStack.h"
#include "../GameStates.h"
#include "../misc/NumberUtils.h"
#include <cstdint>

class ReCraftCore;

class Inventory {
public:
    static constexpr int QUICKSELECT_HEIGHT = 23;  // 22 + 1
    static constexpr int MAX_PER_SITE = 27;

    // Render the hotbar UI
    static void renderHotbar(int x, int y, mc::inventory::Slot* stacks, int& selected);

    // Draw inventory grid, returns current site number
    static int draw(int x, int y, mc::inventory::Slot* stacks, int count, int site);

    // Transfer itemStacks
    static void Transfer(ItemStack* src, ItemStack* dst);
private:
    // Internal state for drag-and-drop operations
    static inline mc::inventory::Slot* sourceStack = nullptr;
    static inline mc::inventory::Slot* proposedSourceStack = nullptr;

    // Helper function for click handling
    static void handleStackClick(mc::inventory::Slot* stack);

    static void drawSlot(mc::inventory::Slot *slot, int x, int y);
};