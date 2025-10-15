#include "Screen.h"
#include "ScreenManager.h"

Screen::Screen() : screenManager(nullptr), isInitialized(false) {
}

Screen::~Screen() {
}

void Screen::switchToScreen(Screen* newScreen, bool isTopScreen) {
    if (screenManager) {
        screenManager->displayScreen(newScreen, isTopScreen);
    }
}