#pragma once

#include <3ds.h>
#include <citro3d.h>

#include <amethyst.hpp>
extern "C" {
#include <sino/sino.h>
}
#include "GameStates.hpp"
#include "client/entity/Player.hpp"
#include "client/gui/DebugUI.hpp"
#include "client/gui/Gui.hpp"
#include "client/gui/GuiChat.hpp"
#include "client/gui/GuiScreen.hpp"
#include "client/gui/ImGuiManager.hpp"
#include "client/renderer/PolyGen.hpp"
#include "client/renderer/Renderer.hpp"
//#include "client/renderer/block/ModelBakery.hpp"
#include "mcbridge/MCBridge.hpp"
#include "misc/Crash.hpp"
#include "world/ChunkWorker.hpp"
#include "world/NetworkWorld.hpp"
#include "world/World.hpp"
#include "world/savegame/SaveManager.hpp"
#include "world/savegame/SuperChunk.hpp"
#include "world/worldgen/EmptyGen.hpp"
#include "world/worldgen/SmeaGen.hpp"
#include "world/worldgen/SuperFlatGen.hpp"

class GuiScreen;

class ReCraftCore : public Amy::App {
public:
    ReCraftCore();
    ~ReCraftCore();
    static ReCraftCore* GetInstance() { return m_theReCraftCore; }

    void Main() override;

    void SetScreen(GuiScreen* pScreen, bool top);

    void InitSinglePlayer(char* path, char* name, const WorldGenType* worldType, Gamemode gamemode, bool newWorld);

    void InitMultiPlayer();

    GameState* GetGameState() { return &m_gamestate; }
    Amy::AssetMgr* GetAssetManager() { return &m_AssetMgr; }
    Player* GetPlayer() { return m_player; }
    //ModelBakery* GetModelBakery() { return m_modelBakery; }
    static const std::string& GetRootDir() { return m_root; }

    GuiScreen* TopScreen = nullptr;
    GuiScreen* BotScreen = nullptr;

private:
    static const std::string m_root;
    static ReCraftCore* m_theReCraftCore;
    SuperFlatGen m_flatGen;
    SmeaGen m_smeaGen;
    EmptyGen m_emptyGen;
    SaveManager m_savemgr;
    ChunkWorker m_chunkWorker;
    MCBridge m_mcBridge;
    DebugUI* m_debugUI = nullptr;

    GuiScreen* m_pTopQueuedScreen = nullptr;
    GuiScreen* m_pBotQueuedScreen = nullptr;
    bool m_bTopUsingCurrScreen = false;
    bool m_bTopHaveQueuedScreen = false;
    bool m_bBotUsingCurrScreen = false;
    bool m_bBotHaveQueuedScreen = false;

    Player* m_player = nullptr;
    World* m_world = nullptr;
    Renderer* m_renderer = nullptr;
    GuiChat* m_chat = nullptr;
    NetworkWorld* m_networkWorld = nullptr;
    GameState m_gamestate;
    Amy::AssetMgr m_AssetMgr;
    //ModelBakery* m_modelBakery = nullptr;
    float m_timeAccum = 0.f;


    void RunSinglePlayer();
    void ExitSinglePlayer();

    void RunMultiPlayer();
    void ExitMultiplayer();
    //void DebugModel(ModelBlock model);
};
