#pragma once

#include <3ds.h>
#include <citro3d.h>
extern "C" {
#include <sino/sino.h>
}
#include "GameStates.h"
#include "entity/Player.h"
#include "entity/PlayerController.h"
#include "gui/DebugUI.h"
#include "gui/Gui.h"
#include "gui/WorldSelect.h"
#include "rendering/PolyGen.h"
#include "rendering/Renderer.h"
#include "world/ChunkWorker.h"
#include "world/CT_World.h"
#include "world/savegame/SaveManager.h"
#include "world/savegame/SuperChunk.h"
#include "world/worldgen/SmeaGen.h"
#include "world/worldgen/SuperFlatGen.h"
#include "misc/Crash.h"

class ReCraftCore {
public:
    static ReCraftCore* Instance; // Global access point

    ReCraftCore();
    ~ReCraftCore();

    void Init();
    void Run();
    void Shutdown();

    // Accessors for subsystems
    inline World* GetWorld() { return world; }
    inline Player* GetPlayer() { return &player; }
    inline Renderer* GetRenderer() { return &renderer; }
    inline DebugUI* GetDebugUI() { return &debugUI; }
    inline SaveManager* GetSaveManager() { return &saveManager; }
    inline ChunkWorker* GetChunkWorker() { return &chunkWorker; }

private:
    void ReleaseWorld();

    GameState gameState = GameState_SelectWorld;
    bool showDebugInfo = false;

    World* world = nullptr;
    Player player;
    PlayerController playerController;
    Renderer renderer;
    DebugUI debugUI;
    SaveManager saveManager;
    ChunkWorker chunkWorker;
    SuperFlatGen flatGen;
    SmeaGen smeaGen;

    uint64_t lastTime = 0;
    float dt = 0.f, timeAccum = 0.f, fpsClock = 0.f;
    int frameCounter = 0, fps = 0;
};
