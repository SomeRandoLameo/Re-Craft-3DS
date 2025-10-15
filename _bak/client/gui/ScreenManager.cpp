#include "ScreenManager.h"
#include <cstring>

ScreenManager::ScreenManager()
        : currentTopScreen(nullptr),
          currentBottomScreen(nullptr),
          topScreenWidth(400),
          topScreenHeight(240),
          bottomScreenWidth(320),
          bottomScreenHeight(240) {
}

ScreenManager::~ScreenManager() {

    if (currentTopScreen) {
        currentTopScreen->onGUIClosed();
        delete currentTopScreen;
    }
    if (currentBottomScreen) {
        currentBottomScreen->onGUIClosed();
        delete currentBottomScreen;
    }
}

void ScreenManager::init() {

    gfxInitDefault();

    gfxSet3D(false);

    currentTopScreen = nullptr;
    currentBottomScreen = nullptr;
}

void ScreenManager::update(float ticks) {

    if (currentTopScreen) {
        currentTopScreen->updateScreen();
    }

    if (currentBottomScreen) {
        currentBottomScreen->updateScreen();
    }
}

void ScreenManager::displayScreen(Screen* screen, bool isTopScreen) {
    if (isTopScreen) {
        if (currentTopScreen) {
            currentTopScreen->onGUIClosed();
            delete currentTopScreen;
            currentTopScreen = nullptr;
        }

        currentTopScreen = screen;

        if (currentTopScreen) {
            setupScreen(currentTopScreen);
        }
    } else {

        if (currentBottomScreen) {
            currentBottomScreen->onGUIClosed();
            delete currentBottomScreen;
            currentBottomScreen = nullptr;
        }


        currentBottomScreen = screen;

        if (currentBottomScreen) {
            setupScreen(currentBottomScreen);
        }
    }
}

void ScreenManager::clearScreen(bool isTopScreen) {
    displayScreen(nullptr, isTopScreen);
}

void ScreenManager::setupScreen(Screen* screen) {
    if (!screen) return;

    screen->screenManager = this;

    if (!screen->isInitialized) {
        screen->initGUI();
        screen->isInitialized = true;
    }
}
