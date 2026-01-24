#pragma once

#include <cstdint>
#include "../GameStates.hpp"
#include "../inventory/ItemStack.hpp"
#include "../misc/NumberUtils.hpp"

class ReCraftCore;

class Inventory {
public:
    static constexpr int QUICKSELECT_HEIGHT = 23;  // 22 + 1
    static constexpr int MAX_PER_SITE = 27;

    int currentSite = 0;

    void renderHotbar(int x, int y, mc::inventory::Slot* stacks, int& selected);

    void draw(int x, int y, mc::inventory::Slot* stacks, int count, int site);

    void Transfer(ItemStack* src, ItemStack* dst);
private:
    mc::inventory::Slot* m_sourceStack = nullptr;
    mc::inventory::Slot* m_proposedSourceStack = nullptr;

    void handleStackClick(mc::inventory::Slot* stack);

    void drawSlot(mc::inventory::Slot *slot, int x, int y);
};