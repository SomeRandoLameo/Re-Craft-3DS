#include "gui/CT_Inventory.hpp"

#include <algorithm>

#include "GameStates.hpp"
#include "ReCraftCore.hpp"
#include "gui/Gui.hpp"
#include "gui/SpriteBatch.hpp"
#include "inventory/ItemStack.hpp"
#include "mcbridge/MCBridge.hpp"
#include "misc/NumberUtils.hpp"
#include "rendering/VertexFmt.hpp"

void Inventory::handleStackClick(mc::inventory::Slot* stack) {
    if (m_sourceStack == nullptr && stack != m_proposedSourceStack) {
        m_proposedSourceStack = stack;
    } else if (m_proposedSourceStack == stack) {
        m_sourceStack         = stack;
        m_proposedSourceStack = nullptr;
    } else if (m_sourceStack != nullptr) {
        if (m_sourceStack != stack) {
            // Convert to ItemStack
            ItemStack sourceItemStack = MCBridge::MCLIBSlotToCTItemStack(*m_sourceStack);
            ItemStack destItemStack   = MCBridge::MCLIBSlotToCTItemStack(*stack);

            // Perform the transfer
            Inventory::Transfer(&sourceItemStack, &destItemStack);

            // Convert back and write to the original slots
            *m_sourceStack = MCBridge::CTItemStackToMCLIBSlot(sourceItemStack);
            *stack         = MCBridge::CTItemStackToMCLIBSlot(destItemStack);
        }
        m_sourceStack = nullptr;
    }
}

void Inventory::renderHotbar(int x, int y, mc::inventory::Slot* stacks, int& selected) {
    SpriteBatch_BindGuiTexture(GuiTexture::Widgets);

    for (int i = 0; i < 9; ++i) {
        SpriteBatch_SetScale(1);

        const int rx = (i * 20 + x + 3) * 2;
        const int ry = (y + 3) * 2;

        // Render item icon if stack has items
        if (stacks[i].GetItemCount() > 0) {
            SpriteBatch_PushIcon(stacks[i], rx, ry, 11);
        }

        // Handle cursor interaction
        if (Gui::EnteredCursorInside(rx - 4, ry - 4, 18 * 2, 18 * 2)) {
            selected = i;
            handleStackClick(&stacks[i]);

            // if (*ReCraftCore::GetInstance()->GetGameState() == GameState::Playing_OnLine) {
            // TODO: client->GetHotbar().SelectSlot(i);
            //}
        }

        SpriteBatch_SetScale(2);

        // Highlight source stack
        if (m_sourceStack == &stacks[i]) {
            SpriteBatch_PushSingleColorQuad(rx / 2 - 2, ry / 2 - 2, 9, 18, 18, SHADER_RGB(20, 5, 2));
            SpriteBatch_BindGuiTexture(GuiTexture::Widgets);
        }

        // Draw slot separator (except for last two slots)
        if (i < 9 - 2) {
            SpriteBatch_PushQuad(i * 20 + 21 + x, y, 10, 20, 22, 21, 0, 20, 22);
        }
    }

    SpriteBatch_SetScale(2);

    // Draw hotbar ends
    SpriteBatch_PushQuad(x, y, 10, 21, 22, 0, 0, 21, 22);
    SpriteBatch_PushQuad(21 + 20 * 7 + x, y, 10, 21, 22, 161, 0, 21, 22);

    // Draw selection indicator
    SpriteBatch_PushQuad(x + selected * 20 - 1, y - 1, 14, 24, 24, 0, 22, 24, 24);
}

void Inventory::drawSlot(mc::inventory::Slot* slot, int x, int y) {

    SpriteBatch_PushIcon(*slot, x * 2, y * 2, 10);

    if (Gui::EnteredCursorInside(x * 2, y * 2, 16 * 2, 16 * 2)) {
        handleStackClick(slot);
    }

    const s16 backgroundColor =
        (m_sourceStack == slot) ? SHADER_RGB(20, 5, 2) : SHADER_RGB_DARKEN(SHADER_RGB(20, 20, 21), 9);

    SpriteBatch_PushSingleColorQuad(x * 2, y * 2, 9, 16 * 2, 16 * 2, backgroundColor);
}

void Inventory::draw(int x, int y, mc::inventory::Slot* stacks, int count, int site) {
    SpriteBatch_SetScale(1);

    int headX   = x;
    int headY   = y;
    currentSite = site;

    if (count > 27) {
        Gui::Offset(0, 155);
        if (Gui::Button(0.f, " << ") && currentSite > 1) {
            --currentSite;
        }

        Gui::Offset(270, 155);
        if (Gui::Button(0.f, " >> ") && currentSite * 27 < count) {
            ++currentSite;
        }
    }

    const int startIndex = (currentSite - 1) * 27;
    const int endIndex   = std::min(currentSite * 27, count);

    for (int i = startIndex; i < endIndex; ++i) {

        if (!stacks[i].GetItemId() || stacks[i].GetItemCount() <= 0) {
            continue;
        }

        if ((i - startIndex) % 9 == 0 && i != startIndex) {
            headX = x;
            headY += 16;
        }

        drawSlot(&stacks[i], headX, headY);

        headX += 16;
    }

    SpriteBatch_SetScale(2);
}

void Inventory::Transfer(ItemStack* src, ItemStack* dst) {
    if ((src->block == dst->block && src->meta == dst->meta) || dst->amount == 0) {
        int vol = std::min(static_cast<int>(src->amount), ItemStack::MAX - static_cast<int>(dst->amount));
        src->amount -= vol;
        dst->amount += vol;
        dst->block = src->block;
        dst->meta  = src->meta;
    } else {
        ItemStack tmp = *src;
        *src          = *dst;
        *dst          = tmp;
    }
}
