#pragma once

enum WorldGenType { Smea, SuperFlat, Empty, WorldGenTypes_Count };

enum Gamemode { Survival, Creative, adventure, Spectator, Gamemode_Count };

struct GeneratorSettings {
    u64          seed;
    WorldGenType type;
    // gamemode type;
    union {
        struct {
            // Keine Einstellungen...
        } superflat;
    } settings;
};
