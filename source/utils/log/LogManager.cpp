#include "LogManager.hpp"

bool LogManager::enabled = true;

void LogManager::Init() {
    consoleInit(GFX_BOTTOM, NULL); 
    enabled = true;
}

void LogManager::Update() {
    hidScanInput();
    u32 kDown = hidKeysDown();

    if ((kDown & KEY_L) && (kDown & KEY_R)) {
        enabled = !enabled;
        Log(enabled ? "Logging enabled" : "Logging disabled", LogType::INFO);
    }
}

void LogManager::Log(const char* message, LogType type) {
    if (!enabled) return;
    PrintColored(message, type);
}

void LogManager::PrintColored(const char* message, LogType type) {
    switch (type) {
        case LogType::NORMAL:
            printf("\x1b[0m%s\n", message); 
            break;
        case LogType::INFO:
            printf("\x1b[33m%s\x1b[0m\n", message);
            break;
        case LogType::WARNING:
            printf("\x1b[31m%s\x1b[0m\n", message);
            break;
    }
}
