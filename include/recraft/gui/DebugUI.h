#pragma once

class DebugUI {
public:
    DebugUI();
    ~DebugUI();

    void Text(const char *text, ...);

    void Log(const char *text, ...);

    void Draw();
};