#pragma once

#include <3ds.h>
#include <citro3d.h>

#include <amethyst.hpp>
extern "C" {
#include <sino/sino.h>
}
#include "GameStates.hpp"
#include "entity/Player.hpp"
#include "entity/PlayerController.hpp"
#include "gui/DebugUI.hpp"
#include "gui/Gui.hpp"
#include "gui/GuiChat.hpp"
#include "gui/ImGuiManager.hpp"
#include "gui/WorldSelect.hpp"
#include "mcbridge/MCBridge.hpp"
#include "misc/Crash.hpp"
#include "rendering/PolyGen.hpp"
#include "rendering/Renderer.hpp"
#include "world/CT_World.hpp"
#include "world/ChunkWorker.hpp"
#include "world/NetworkWorld.hpp"
#include "world/savegame/SaveManager.hpp"
#include "world/savegame/SuperChunk.hpp"
#include "world/worldgen/EmptyGen.hpp"
#include "world/worldgen/SmeaGen.hpp"
#include "world/worldgen/SuperFlatGen.hpp"

class ReCraftCore : public Amy::App {
public:
    ReCraftCore();
    ~ReCraftCore();
    static ReCraftCore* GetInstance() { return m_theReCraftCore; }

    void Main() override;

    void InitSinglePlayer(char* path, char* name, const WorldGenType* worldType, bool newWorld);

    void InitMultiPlayer();

    GameState* GetGameState() { return &m_gamestate; }
    Amy::AssetMgr* GetAssetManager() { return &m_AssetMgr; }
private:
    static ReCraftCore* m_theReCraftCore;
    SuperFlatGen m_flatGen;
    SmeaGen m_smeaGen;
    EmptyGen m_emptyGen;
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


    void RunSinglePlayer(InputData inputData);
    void ExitSinglePlayer();

    void RunMultiPlayer(InputData inputData);
    void ExitMultiplayer();
};
