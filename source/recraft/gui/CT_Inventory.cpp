#include "gui/CT_Inventory.h"
#include "ReCraftCore.h"
#include "gui/Gui.h"
#include "gui/SpriteBatch.h"
#include "rendering/VertexFmt.h"
#include <algorithm>

void Inventory::handleStackClick(mc::inventory::Slot* stack) {
    if (sourceStack == nullptr && stack != proposedSourceStack) {
        proposedSourceStack = stack;
    }
    else if (proposedSourceStack == stack) {
        sourceStack = stack;
        proposedSourceStack = nullptr;
    }
    else if (sourceStack != nullptr) {
        if (sourceStack != stack) {
            // Convert to ItemStack
            ItemStack sourceItemStack = MCBridge::MCLIBSlotToCTItemStack(*sourceStack);
            ItemStack destItemStack = MCBridge::MCLIBSlotToCTItemStack(*stack);

            // Perform the transfer
            Inventory::Transfer(&sourceItemStack, &destItemStack);

            // Convert back and write to the original slots
            *sourceStack = MCBridge::CTItemStackToMCLIBSlot(sourceItemStack);
            *stack = MCBridge::CTItemStackToMCLIBSlot(destItemStack);
        }
        sourceStack = nullptr;
    }
}

void Inventory::renderHotbar(int x, int y, mc::inventory::Slot* stacks, int count, int& selected) {
    SpriteBatch_BindGuiTexture(GuiTexture_Widgets);

    for (int i = 0; i < count; ++i) {
        SpriteBatch_SetScale(1);

        const int rx = (i * 20 + x + 3) * 2;
        const int ry = (y + 3) * 2;

        // Render item icon if stack has items
        if (MCBridge::MCLIBSlotToCTItemStack(stacks[i]).amount > 0) {
            SpriteBatch_PushIcon(stacks[i], rx, ry, 11);
        }

        // Handle cursor interaction
        if (Gui_EnteredCursorInside(rx - 4, ry - 4, 18 * 2, 18 * 2)) {
            selected = i;
            handleStackClick(&stacks[i]);

            if (*ReCraftCore::GetInstance()->GetGameState() == GameState_Playing_OnLine) {
                // TODO: client->GetHotbar().SelectSlot(i);
            }
        }

        SpriteBatch_SetScale(2);

        // Highlight source stack
        if (sourceStack == &stacks[i]) {
            SpriteBatch_PushSingleColorQuad(
                    rx / 2 - 2, ry / 2 - 2, 9, 18, 18,
                    SHADER_RGB(20, 5, 2)
            );
            SpriteBatch_BindGuiTexture(GuiTexture_Widgets);
        }

        // Draw slot separator (except for last two slots)
        if (i < count - 2) {
            SpriteBatch_PushQuad(i * 20 + 21 + x, y, 10, 20, 22, 21, 0, 20, 22);
        }
    }

    SpriteBatch_SetScale(2);

    // Draw hotbar ends
    SpriteBatch_PushQuad(x, y, 10, 21, 22, 0, 0, 21, 22);
    SpriteBatch_PushQuad(21 + 20 * (count - 2) + x, y, 10, 21, 22, 161, 0, 21, 22);

    // Draw selection indicator
    SpriteBatch_PushQuad(x + selected * 20 - 1, y - 1, 14, 24, 24, 0, 22, 24, 24);
}

int Inventory::draw(int x, int y, int width, mc::inventory::Slot* stacks, int count, int site) {
    SpriteBatch_SetScale(1);

    int headX = x;
    int headY = y;
    int currentSite = site;
    bool even = false;
    bool newLine = false;

    const int16_t colors[2] = {
            SHADER_RGB_DARKEN(SHADER_RGB(20, 20, 21), 9),
            SHADER_RGB_DARKEN(SHADER_RGB(20, 20, 21), 8)
    };

    // Draw pagination buttons if needed
    if (count > MAX_PER_SITE) {
        Gui_Offset(0, 60);
        if (Gui_Button(0.f, " << ") && currentSite > 1) {
            --currentSite;
        }

        Gui_Offset(270, 60);
        if (Gui_Button(0.f, " >> ") && currentSite * MAX_PER_SITE < count) {
            ++currentSite;
        }
    }

    const int startIndex = (currentSite - 1) * MAX_PER_SITE;
    const int endIndex = std::min(currentSite * MAX_PER_SITE, count);

    for (int i = startIndex; i < endIndex; ++i) {
        // Only draw valid inventory items
        if (!stacks[i].GetItemId() || stacks[i].GetItemCount() <= 0) {
            continue;
        }

        newLine = false;

        // Check if we need to wrap to next line
        if ((headX + 16) >= width) {
            headX = x;
            headY += 17;
            newLine = true;
        }

        // Draw item icon
        SpriteBatch_PushIcon(stacks[i], headX * 2, headY * 2, 10);

        // Handle cursor interaction
        if (Gui_EnteredCursorInside(headX * 2, headY * 2, 16 * 2, 16 * 2)) {
            handleStackClick(&stacks[i]);
        }

        // Draw background quad with appropriate color
        const int16_t backgroundColor = (sourceStack == &stacks[i])
                                        ? SHADER_RGB(20, 5, 2)
                                        : colors[even];

        SpriteBatch_PushSingleColorQuad(
                headX * 2, headY * 2, 9, 16 * 2, 16 * 2,
                backgroundColor
        );

        even = !even;
        headX += 16;

        // Draw separator line between inventory rows
        if (newLine) {
            even = false;
            SpriteBatch_PushSingleColorQuad(
                    x * 2, (headY - 1) * 2, 10, (width - 32) * 2, 2,
                    SHADER_RGB(7, 7, 7)
            );
        }
    }

    SpriteBatch_SetScale(2);
    return currentSite;
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
