#pragma once

#include <amethyst.hpp>

class DebugUI {
public:
    enum { StatusLines = (240 / 8), LogLines = 40, LineLength = 128 };
    DebugUI();
    ~DebugUI();

    void Text(const char* text, ...);
    void Log(const char* text, ...);
    void Draw();

    Iron::Drawlist::Ref RenderData;
};
