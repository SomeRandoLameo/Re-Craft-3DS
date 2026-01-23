#pragma once

#include "gui/Screen.hpp"


class GuiInGameTop : public Screen {
public:
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
    Player* m_player = nullptr;
    Inventory* m_inventory = nullptr;
};
