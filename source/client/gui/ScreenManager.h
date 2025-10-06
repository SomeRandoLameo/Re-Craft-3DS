#pragma once

#include <3ds.h>
#include "Screen.h"

class ScreenManager {
public:
    ScreenManager();
    ~ScreenManager();

    void init();

    void update(float ticks);

    void displayScreen(Screen* screen, bool isTopScreen);

    // Get current screens
    Screen* getCurrentTopScreen() const { return currentTopScreen; }
    Screen* getCurrentBottomScreen() const { return currentBottomScreen; }

    void clearScreen(bool isTopScreen);

private:
    Screen* currentTopScreen;
    Screen* currentBottomScreen;

    int topScreenWidth;
    int topScreenHeight;
    int bottomScreenWidth;
    int bottomScreenHeight;

    void setupScreen(Screen* screen);
};