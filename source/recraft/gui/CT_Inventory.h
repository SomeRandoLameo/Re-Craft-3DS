#pragma once

#include "../inventory/ItemStack.h"
#include "../GameStates.h"
#include "../misc/NumberUtils.h"
#include <cstdint>

class ReCraftCore;

class Inventory {
public:
    static constexpr int QUICKSELECT_MAX_SLOTS = 9;
    static constexpr int QUICKSELECT_HEIGHT = 23;  // 22 + 1
    static constexpr int MAX_PER_SITE = 27;

    // Calculate quickselect dimensions
    static constexpr int calculateQuickSelectSlots() noexcept {
        return QUICKSELECT_MAX_SLOTS;
    }

    static constexpr int calculateQuickSelectWidth(int slots) noexcept {
        return 54 + (slots - 2) * 20;
    }

    // Render the hotbar UI
    static void renderHotbar(int x, int y, ItemStack* stacks, int count, int& selected);

    // Draw inventory grid, returns current site number
    static int draw(int x, int y, int width, ItemStack* stacks, int count, int site);

private:
    // Internal state for drag-and-drop operations
    static inline ItemStack* sourceStack = nullptr;
    static inline ItemStack* proposedSourceStack = nullptr;

    // Helper function for click handling
    static void handleStackClick(ItemStack* stack);
};