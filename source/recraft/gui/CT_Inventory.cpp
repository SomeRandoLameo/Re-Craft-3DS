#include "gui/CT_Inventory.hpp"
#include <algorithm>
#include "ReCraftCore.hpp"
#include "gui/Gui.hpp"
#include "gui/SpriteBatch.hpp"
#include "rendering/VertexFmt.hpp"

void Inventory::handleStackClick(mc::inventory::Slot* stack) {
    if (m_sourceStack == nullptr && stack != m_proposedSourceStack) {
        m_proposedSourceStack = stack;
    }
    else if (m_proposedSourceStack == stack) {
        m_sourceStack = stack;
        m_proposedSourceStack = nullptr;
    }
    else if (m_sourceStack != nullptr) {
        if (m_sourceStack != stack) {
            // Convert to ItemStack
            ItemStack sourceItemStack = MCBridge::MCLIBSlotToCTItemStack(*m_sourceStack);
            ItemStack destItemStack = MCBridge::MCLIBSlotToCTItemStack(*stack);

            // Perform the transfer
            Inventory::Transfer(&sourceItemStack, &destItemStack);

            // Convert back and write to the original slots
            *m_sourceStack = MCBridge::CTItemStackToMCLIBSlot(sourceItemStack);
            *stack = MCBridge::CTItemStackToMCLIBSlot(destItemStack);
        }
        m_sourceStack = nullptr;
    }
}

void Inventory::Transfer(ItemStack* src, ItemStack* dst) {
    if ((src->block == dst->block && src->meta == dst->meta) || dst->amount == 0) {
        int vol = std::min(static_cast<int>(src->amount), ItemStack::MAX - static_cast<int>(dst->amount));
        src->amount -= vol;
        dst->amount += vol;
        dst->block = src->block;
        dst->meta = src->meta;
    } else {
        ItemStack tmp = *src;
        *src = *dst;
        *dst = tmp;
    }
}