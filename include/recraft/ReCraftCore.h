#pragma once

#include <3ds.h>
#include <citro3d.h>

#include <amethyst.hpp>
extern "C" {
#include <sino/sino.h>
}
#include "GameStates.h"
#include "entity/Player.h"
#include "entity/PlayerController.h"
#include "gui/DebugUI.h"
#include "gui/Gui.h"
#include "gui/GuiChat.h"
#include "gui/ImGuiManager.h"
#include "gui/WorldSelect.h"
#include "mcbridge/MCBridge.h"
#include "misc/Crash.h"
#include "rendering/PolyGen.h"
#include "rendering/Renderer.h"
#include "world/CT_World.h"
#include "world/ChunkWorker.h"
#include "world/NetworkWorld.h"
#include "world/savegame/SaveManager.h"
#include "world/savegame/SuperChunk.h"
#include "world/worldgen/SmeaGen.h"
#include "world/worldgen/SuperFlatGen.h"

class ReCraftCore : public Amy::App {
public:
    ReCraftCore();
    ~ReCraftCore();
    static ReCraftCore* GetInstance() { return m_theReCraftCore; }

    void Main() override;

    GameState* GetGameState() { return &m_gamestate; }
    Amy::AssetMgr* GetAssetManager() { return &m_AssetMgr; }

private:
    static ReCraftCore* m_theReCraftCore;
    SuperFlatGen m_flatGen;
    SmeaGen m_smeaGen;
    SaveManager m_savemgr;
    ChunkWorker m_chunkWorker;
    MCBridge m_mcBridge;
    DebugUI* m_debugUI = nullptr;
    Player* m_player = nullptr;
    World* m_world = nullptr;
    PlayerController* m_playerCtrl = nullptr;
    Renderer* m_renderer = nullptr;
    GuiChat* m_chat = nullptr;

    NetworkWorld* m_networkWorld = nullptr;
    GameState m_gamestate;
    Amy::AssetMgr m_AssetMgr;
    float m_timeAccum = 0.f;

    //TODO: MOVE INTO WORLD FOR *** SAKE
    void ReleaseWorld(ChunkWorker* chunkWorker, SaveManager* savemgr, World* world);

    void InitSinglePlayer(char* path, char* name, const WorldGenType* worldType, bool newWorld);
    void RunSinglePlayer(InputData inputData);
    void ExitSinglePlayer();

    void InitMultiPlayer();
    void RunMultiPlayer(InputData inputData);
    void ExitMultiplayer();

    void RunSelectWorld();
};
