#pragma once

#include <amethyst.hpp>

class DebugUI {
public:
    DebugUI();
    ~DebugUI();

    void Text(const char *text, ...);
    void Log(const char *text, ...);
    void Draw();

    Iron::Drawlist* RenderData;
};