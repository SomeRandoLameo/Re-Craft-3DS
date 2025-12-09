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
#include "world/T_World.h"
#include "world/savegame/SaveManager.h"
#include "world/savegame/SuperChunk.h"
#include "world/worldgen/SmeaGen.h"
#include "world/worldgen/SuperFlatGen.h"

class ReCraftCore : public Amy::App {
 public:
  ReCraftCore();
  ~ReCraftCore();
  static ReCraftCore* GetInstance() { return theReCraftCore; }

  void Main() override;

  GameState* GetGameState() { return &gamestate; }
  Amy::AssetMgr* GetAssetManager() { return &AssetMgr; }

 private:
  static ReCraftCore* theReCraftCore;
  SuperFlatGen flatGen;
  SmeaGen smeaGen;
  SaveManager savemgr;
  ChunkWorker chunkWorker;
  MCBridge mcBridge;
  DebugUI* debugUI = nullptr;
  Player* player = nullptr;
  World* world = nullptr;
  PlayerController* playerCtrl = nullptr;
  Renderer* renderer = nullptr;
  GuiChat* chat = nullptr;
  GameState gamestate;
  Amy::AssetMgr AssetMgr;
  float timeAccum = 0.f;
  void ReleaseWorld(ChunkWorker* chunkWorker, SaveManager* savemgr,
                    World* world);
};
