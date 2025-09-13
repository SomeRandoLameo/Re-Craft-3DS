#pragma once
#include <3ds.h>
#include <cstdio>

enum class LogType {
    NORMAL,
    INFO,
    WARNING
};

class LogManager {
public:
    // Initialisiert den LogManager
    static void Init();

    // Gibt eine Nachricht aus
    static void Log(const char* message, LogType type = LogType::NORMAL);

    // Aktualisiert den LogManager (Tastenkombinationen)
    static void Update();

    // Zeigt an, ob Logging aktuell aktiviert ist
    static bool IsEnabled() { return enabled; }

private:
    static bool enabled;
    static void PrintColored(const char* message, LogType type);
};
