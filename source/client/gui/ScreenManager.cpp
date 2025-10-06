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
    // Clean up current screens
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
    // Initialize graphics
    gfxInitDefault();

    // Set 3D slider to 0 for 2D screens
    gfxSet3D(false);

    // Initialize both screens to nullptr
    currentTopScreen = nullptr;
    currentBottomScreen = nullptr;
}

void ScreenManager::update(float ticks) {
    // Update top screen
    if (currentTopScreen) {
        currentTopScreen->updateScreen();
        // Entferne Framebuffer-Handling und drawScreen-Aufruf
    }

    // Update bottom screen
    if (currentBottomScreen) {
        currentBottomScreen->updateScreen();
        // Entferne Framebuffer-Handling und drawScreen-Aufruf
    }

    // Entferne gfxFlushBuffers, gfxSwapBuffers, gspWaitForVBlank aus dieser Methode
}

void ScreenManager::displayScreen(Screen* screen, bool isTopScreen) {
    if (isTopScreen) {
        // Close current top screen if exists
        if (currentTopScreen) {
            currentTopScreen->onGUIClosed();
            delete currentTopScreen;
            currentTopScreen = nullptr;
        }

        // Set new screen
        currentTopScreen = screen;

        if (currentTopScreen) {
            setupScreen(currentTopScreen);
        }
    } else {
        // Close current bottom screen if exists
        if (currentBottomScreen) {
            currentBottomScreen->onGUIClosed();
            delete currentBottomScreen;
            currentBottomScreen = nullptr;
        }

        // Set new screen
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

    // Set the screen manager reference
    screen->screenManager = this;

    // Initialize the GUI if not already initialized
    if (!screen->isInitialized) {
        screen->initGUI();
        screen->isInitialized = true;
    }
}
