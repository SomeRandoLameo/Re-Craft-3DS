#include <3ds.h>
#include <iostream>
#include "gui/ServerList/ServerList.hpp"



int main() {
    gfxInitDefault();
    consoleInit(GFX_BOTTOM, nullptr);
    std::atexit(gfxExit);

    ServerList serverList;
    serverList.initGui();
    serverList.pollAllAsync();
    serverList.drawScreen();

    while (aptMainLoop()) {
        gspWaitForVBlank();
        hidScanInput();
        u32 kDown = hidKeysDown();

        serverList.updateControlls(kDown);
        if (kDown & KEY_START) break;
    }

    return 0;
}
