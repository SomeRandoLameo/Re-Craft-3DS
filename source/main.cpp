#include <3ds.h>
#include <iostream>
#include "gui/ServerList/ServerList.hpp"

#include "gui/Screen.hpp"

int main() {
    gfxInitDefault();
    consoleInit(GFX_BOTTOM, nullptr);
    std::atexit(gfxExit);

    ServerList serverList;
    Screen::currentScreen = &serverList;
    serverList.initGui();
    serverList.pollAllAsync();
    serverList.drawScreen();

    while (aptMainLoop()) {
        gspWaitForVBlank();
        hidScanInput();
        u32 kDown = hidKeysDown();

        if (Screen::currentScreen) {
            Screen::currentScreen->updateControls(kDown);
        }
    
        if (kDown & KEY_START) break;
    }

    return 0;
}
