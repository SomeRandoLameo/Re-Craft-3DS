#pragma once

#include "../Screen.h"

// Example implementation of a screen
class GuiInGame : public Screen {
public:
    GuiInGame();
    ~GuiInGame() override;

    void initGUI() override;
    void updateScreen() override;
    void drawScreen(float ticks) override;
    void onGUIClosed() override;
    bool doesGUIPauseGame() override;

private:
    int selectedOption;
    u32 kDown;
};