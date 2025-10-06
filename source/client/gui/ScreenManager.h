#pragma once

#include <3ds.h>
#include "Screen.h"

class ScreenManager {
public:
    ScreenManager();
    ~ScreenManager();

    // Initialize the screen manager
    void init();

    // Update function to be called each frame
    void update(float ticks);

    // Display a screen on either top or bottom screen
    void displayScreen(Screen* screen, bool isTopScreen);

    // Get current screens
    Screen* getCurrentTopScreen() const { return currentTopScreen; }
    Screen* getCurrentBottomScreen() const { return currentBottomScreen; }

    // Clear a specific screen
    void clearScreen(bool isTopScreen);

private:
    Screen* currentTopScreen;
    Screen* currentBottomScreen;

    // Screen dimensions
    int topScreenWidth;
    int topScreenHeight;
    int bottomScreenWidth;
    int bottomScreenHeight;

    // Helper to setup screen
    void setupScreen(Screen* screen);
};