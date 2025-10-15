#pragma once

#include <3ds.h>

class ScreenManager;

class Screen {
public:
    Screen();
    virtual ~Screen();

    virtual void initGUI() = 0;
    virtual void updateScreen() = 0;
    virtual void drawScreen(float ticks) = 0;
    virtual void onGUIClosed() = 0;
    virtual bool doesGUIPauseGame() = 0;

    void switchToScreen(Screen* newScreen, bool isTopScreen);

protected:
    ScreenManager* screenManager;
    bool isInitialized;

    friend class ScreenManager;
};