#pragma once
#include <3ds.h>

class Screen {
public:
    virtual ~Screen() {}
    virtual void drawScreen() const = 0;
    virtual void updateControls(u32 kDown) = 0;
    static Screen* currentScreen;
};
