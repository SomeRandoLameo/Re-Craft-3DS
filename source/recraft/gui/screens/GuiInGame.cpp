#include "gui/screens/GuiInGame.hpp"
#include "gui/SpriteBatch.hpp"

bool GuiInGameTop::IsInGameScreen() {
    return true;
}

void GuiInGameTop::Render(int mouseX, int mouseY, float delta) {
    SpriteBatch_BindGuiTexture(GuiTexture::Icons);
    SpriteBatch_PushQuad(200 / 2 - 16 / 2, 120 / 2 - 16 / 2, 0, 16, 16, 0, 0, 16, 16);

    if (m_ReCraftCore->GetPlayer()->gamemode == 0) {
        RenderExpBar();
        RenderHealth();
        RenderHunger();
    }

    Screen::Render(mouseX, mouseY, delta);
}

// this is actual minecraft ported code
// TODO: Add support for damage animations, potion effects like poisin, wither
// and more
// TODO: Move this somewhere else once the new renderer works, Preferrably as a GuiOverlay
void GuiInGameTop::RenderHealth() {
    // TODO: Damage flicker bounce + healing white flicker + wither and more
    int health = m_ReCraftCore->GetPlayer()->hp;
    int yPos = 99;
    int spriteSize = 9;
    SpriteBatch_BindGuiTexture(GuiTexture::Icons);
    for (int amount = 0; amount < 10; ++amount) {

        int var6 = 0;
        bool var9 = false;
        if (var9) {
            var6 = 1;
        }

        int prevHealth = m_ReCraftCore->GetPlayer()->hp;

        if (health <= 4) {
            yPos += nextafter(2, 0);
        }

        SpriteBatch_PushQuad(spriteSize + (amount * 8), yPos, -1, spriteSize, spriteSize, 16 + var6 * spriteSize, 0,
                             spriteSize, spriteSize);

        if (var9) {
            if ((amount << 1) + 1 < prevHealth) {
                SpriteBatch_PushQuad(spriteSize + (amount * 8), yPos, 0, spriteSize, spriteSize, 70, 0, spriteSize,
                                     spriteSize);
            }

            if ((amount << 1) + 1 == prevHealth) {
                SpriteBatch_PushQuad(spriteSize + (amount * 8), yPos, 0, spriteSize, spriteSize, 79, 0, spriteSize,
                                     spriteSize);
            }
        }

        if ((amount << 1) + 1 < health) {
            SpriteBatch_PushQuad(spriteSize + (amount * 8), yPos, 0, spriteSize, spriteSize, 52, 0, spriteSize,
                                 spriteSize);
        }

        if ((amount << 1) + 1 == health) {
            SpriteBatch_PushQuad(spriteSize + (amount * 8), yPos, 0, spriteSize, spriteSize, 61, 0, spriteSize,
                                 spriteSize);
        }
    }
}

void GuiInGameTop::RenderExpBar() {
    // harcoded cap for now
    int barCap = 10;

    SpriteBatch_BindGuiTexture(GuiTexture::Icons);

    if (barCap > 0) {
        int barLength = 182;
        int xpFill = (int)(m_ReCraftCore->GetPlayer()->experience * (float)(barLength + 1));

        int y = 120 - 9;
        SpriteBatch_PushQuad(200 / 2 - 182 / 2, y, 0, barLength, 5, 0, 64, barLength, 5);

        if (xpFill > 0) {
            SpriteBatch_PushQuad(200 / 2 - 182 / 2, y, 1, xpFill, 5, 0, 69, xpFill, 5);
        }
    }

    if (m_ReCraftCore->GetPlayer()->experienceLevel > 0) {
        char experienceStr[20]; // buffer to hold the string representation of
                                // experience level

        int experienceInt = (int)m_ReCraftCore->GetPlayer()->experienceLevel;
        snprintf(experienceStr, sizeof(experienceStr), "%d",
                 experienceInt); // Format as integer

        int textWidth = SpriteBatch_CalcTextWidth(experienceStr);

        int textY = 10;

        SpriteBatch_PushText(200 / 2 - textWidth / 2 + 1, 120 - textY, 2, SHADER_RGB(0, 0, 0), false, INT_MAX, 0,
                             experienceStr);
        SpriteBatch_PushText(200 / 2 - textWidth / 2 - 1, 120 - textY, 2, SHADER_RGB(0, 0, 0), false, INT_MAX, 0,
                             experienceStr);
        SpriteBatch_PushText(200 / 2 - textWidth / 2, 120 - textY + 1, 2, SHADER_RGB(0, 0, 0), false, INT_MAX, 0,
                             experienceStr);
        SpriteBatch_PushText(200 / 2 - textWidth / 2, 120 - textY - 1, 2, SHADER_RGB(0, 0, 0), false, INT_MAX, 0,
                             experienceStr);
        SpriteBatch_PushText(200 / 2 - textWidth / 2, 120 - textY, 3, SHADER_RGB(100, 255, 32), false, INT_MAX, 0,
                             experienceStr);
    }
}

// TODO: Hunger effects
void GuiInGameTop::RenderHunger() {
    int spriteSize = 9;
    int xpos = 190;
    int ypos = 99;

    int saturationLevel = m_ReCraftCore->GetPlayer()->hunger;

    for (int amount = 0; amount < 10; ++amount) {

        int l6 = 16;
        int j7 = 0;
        /*
                // Hunger Effect
                if (entityplayer.isPotionActive(MobEffects.HUNGER))
                {
                    l6 += 36;
                    j7 = 13;
                }

                //Shaking effect
                if (entityplayer.getFoodStats().getSaturationLevel() <= 0.0F &&
           this.updateCounter % (k * 3 + 1) == 0)
                {
                    ypos = j1 + (this.rand.nextInt(3) - 1);
                }
        */

        int spriteXpos = xpos - amount * 8 - 9;
        SpriteBatch_PushQuad(spriteXpos, ypos, -1, spriteSize, spriteSize, 16 + j7 * 9, 27, 9, 9);

        if (amount * 2 + 1 < saturationLevel) {
            SpriteBatch_PushQuad(spriteXpos, ypos, 0, spriteSize, spriteSize, l6 + 36, 27, 9, 9);
        }

        if (amount * 2 + 1 == saturationLevel) {
            SpriteBatch_PushQuad(spriteXpos, ypos, 0, spriteSize, spriteSize, l6 + 45, 27, 9, 9);
        }
    }
}




void GuiInGameBot::Init() {
    m_player = m_ReCraftCore->GetPlayer();
    m_inventory = new Inventory();
}
void GuiInGameBot::Removed() {
    delete m_player;
    delete m_inventory;
}

bool GuiInGameBot::IsInGameScreen() {
    return true;
}

void GuiInGameBot::Render(int mouseX, int mouseY, float delta) {
    SpriteBatch_SetScale(2);

    m_inventory->renderHotbar(160 / 2 - 194 / 2, 120 - Inventory::QUICKSELECT_HEIGHT, m_player->quickSelectBar,
                              m_player->quickSelectBarSlot);

    m_inventory->draw(((137 / 2) - (120 / 2)), 10, m_player->inventory,
                      sizeof(m_player->inventory) / sizeof(ItemStack), m_player->inventorySite);

    m_player->inventorySite = m_inventory->currentSite;
    Screen::Render(mouseX, mouseY, delta);
}
void GuiInGameBot::ButtonClicked() { Screen::ButtonClicked(); }

