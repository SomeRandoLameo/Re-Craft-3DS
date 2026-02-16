#pragma once

#include "gui/Screen.hpp"


class GuiInGameTop : public Screen {
public:
    void Init() override;
    bool IsInGameScreen() override;
    void Render(int mouseX, int mouseY, float delta) override;
private:
    void RenderHealth();
    void RenderExpBar();
    void RenderHunger();
};

// TODO: this is probably something entirely different but for now its enough
class GuiInGameBot : public Screen {
public:
    void Init() override;
    void Removed() override;

    bool IsInGameScreen() override;
    void Render(int mouseX, int mouseY, float delta) override;
    void ButtonClicked() override;
private:
    void RenderHotbar(int x, int y, mc::inventory::Slot* stacks, int& selected);
    void RenderInventory(int x, int y, mc::inventory::Slot* stacks, int count, int site);
    void RenderSlot(mc::inventory::Slot *slot, int x, int y); //TODO: Generic GUI Function or something like this
    Player* m_player = nullptr;
    Inventory* m_inventory = nullptr;
};
