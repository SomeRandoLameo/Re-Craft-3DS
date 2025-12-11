#include "ReCraftCore.h"

bool showDebugInfo = true;
ReCraftCore* ReCraftCore::theReCraftCore = nullptr;
ReCraftCore::ReCraftCore() {
  theReCraftCore = this;
  this->gamestate = GameState_SelectWorld;
  gfxInitDefault();
  // Enable N3DS 804MHz operation, where available
  osSetSpeedupEnable(true);
  gfxSet3D(true);
  romfsInit();

  SuperChunk_InitPools();
  savemgr.InitFileSystem();

  chunkWorker.AddHandler(
      WorkerItemType_PolyGen,
      (WorkerFuncObj){&PolyGen_GeneratePolygons, NULL, true});
  chunkWorker.AddHandler(
      WorkerItemType_BaseGen,
      (WorkerFuncObj){&SuperFlatGen::Generate, &flatGen, true});
  chunkWorker.AddHandler(WorkerItemType_BaseGen,
                         (WorkerFuncObj){&SmeaGen::Generate, &smeaGen, true});

  sino_init();

  world = new World(&chunkWorker.GetQueue());
  player = new Player(world);
  playerCtrl = new PlayerController(player);

  flatGen.Init(world);
  smeaGen.Init(world);

  AssetMgr.AutoLoad("font", "romfs:/ComicNeue.ttf");

  renderer = new Renderer(world, player, &chunkWorker.GetQueue());
  debugUI = new DebugUI();

  WorldSelect_Init();

  savemgr.Init(player);
  chunkWorker.AddHandler(
      WorkerItemType_Load,
      (WorkerFuncObj){SaveManager::LoadChunkCallback, &savemgr, true});
  chunkWorker.AddHandler(
      WorkerItemType_Save,
      (WorkerFuncObj){SaveManager::SaveChunkCallback, &savemgr, true});

  ImGuiManager::GetInstance()->RegisterCallback("DebugUI", [=, this]() {
    ImGui::Begin("Game Info");

    ImGui::Text("Performance");
    ImGui::Spacing();
    ImGui::Text("FPS: %.2f, DT: %.3f", 1000.f / Delta(), Delta());
    ImGui::Text("CPU: %5.2f%%", C3D_GetProcessingTime() * 6.f);
    ImGui::Text("GPU: %5.2f%%", C3D_GetDrawingTime() * 6.f);
    ImGui::Text("Buffer: %5.2f%%", C3D_GetCmdBufUsage() * 100.f);
    ImGui::Text("Linear Memory: %s",
                Amy::Utils::FormatBytes(linearSpaceFree()).c_str());
    ImGui::Separator();
    ImGui::Text("Player");
    ImGui::Spacing();
    ImGui::Text("Position: %.2f, %.2f, %.2f", player->position.x,
                player->position.y, player->position.z);
    ImGui::Text("Spawn: %.2f, %.2f, %.2f", player->spawnPos.x,
                player->spawnPos.y, player->spawnPos.z);
    ImGui::Text("HP: %i", player->hp);
    ImGui::Text("Hunger: %i (Timer: %i)", player->hunger, player->hungertimer);
    ImGui::Text("Velocity Y: %.3f, RndY: %.3f", player->velocity.y,
                player->rndy);
    ImGui::Separator();
    ImGui::Text("Inventory");
    ImGui::Spacing();
    ImGui::Text("Gamemode: %i", player->gamemode);
    ImGui::Text("Quickbar Slot: %i", player->quickSelectBarSlot);

    ImGui::End();
  });
  // i want to use imgui anywhere
}

ReCraftCore::~ReCraftCore() {
  ImGuiManager::GetInstance()->UnregisterCallback("DebugUI");

  if (this->gamestate == GameState_Playing) {
    ReleaseWorld(&chunkWorker, &savemgr, world);
  }

  if (this->gamestate == GameState_Playing_OnLine) {
    mcBridge.stopBackgroundThread();
    mcBridge.disconnect();
  }

  savemgr.~SaveManager();
  SuperChunk_DeinitPools();

  delete debugUI;
  delete world;
  sino_exit();
  WorldSelect_Deinit();

  delete renderer;
  chunkWorker.~ChunkWorker();
  delete renderer;

  romfsExit();
  gfxExit();
}

// TODO: Why isnt this in world?
void ReCraftCore::ReleaseWorld(ChunkWorker* chunkWorker, SaveManager* savemgr,
                               World* world) {
  for (int i = 0; i < CHUNKCACHE_SIZE; i++) {
    for (int j = 0; j < CHUNKCACHE_SIZE; j++) {
      world->UnloadChunk(world->chunkCache[i][j]);
    }
  }
  chunkWorker->Finish();
  world->Reset();

  savemgr->Unload();
}

void ReCraftCore::Main() {
  renderer->Render(debugUI);

  hidScanInput();
  u32 keysheld = hidKeysHeld(), keysdown = hidKeysDown();
  if (keysdown & KEY_START) {
    if (this->gamestate == GameState_SelectWorld)
      Exit();
    else if (this->gamestate == GameState_Playing) {
      ReleaseWorld(&chunkWorker, &savemgr, world);

      this->gamestate = GameState_SelectWorld;

      WorldSelect_ScanWorlds();
    }
  }

  circlePosition circlePos;
  hidCircleRead(&circlePos);

  circlePosition cstickPos;
  hidCstickRead(&cstickPos);

  touchPosition touchPos;
  hidTouchRead(&touchPos);

  InputData inputData = (InputData){keysheld,     keysdown,     hidKeysUp(),
                                    circlePos.dx, circlePos.dy, touchPos.px,
                                    touchPos.py,  cstickPos.dx, cstickPos.dy};

  timeAccum += Delta();
  if (this->gamestate == GameState_Playing) {
    while (timeAccum >= 20.f) {
      world->Tick();

      timeAccum = -20.f;
    }

    playerCtrl->Update(debugUI, inputData, Delta() * 0.001);

    world->UpdateChunkCache(WorldToChunkCoord(FastFloor(player->position.x)),
                            WorldToChunkCoord(FastFloor(player->position.z)));

  } else if (this->gamestate == GameState_Playing_OnLine) {
    // ONLINE LOGIC HERE
    s32 dimension;

    if (mcBridge.isConnected()) {
      mcBridge.withClient([&](mc::core::Client* client,
                              mc::protocol::packets::PacketDispatcher*
                                  dispatcher) {
        player->position = client->GetPlayerController()->GetPosition();

        dimension = client->GetConnection()->GetDimension();
        playerCtrl->Update(debugUI, inputData, Delta() * 0.001);

        // TODO: This is baaaad :D
        client->GetPlayerController()->Move(
            mc::Vector3d(-playerCtrl->movement.x * (0.125 / 2),
                         -playerCtrl->movement.y * (0.125 / 2),
                         -playerCtrl->movement.z * (0.125 / 2)));

        client->GetPlayerController()->SetPitch(-playerCtrl->player->pitch);
        client->GetPlayerController()->SetYaw(-playerCtrl->player->yaw);

        // TODO: packet spam prevention... This is enough for now
        for (int i = 0; i < 9; i++) {
          auto item = client->GetHotbar().GetItem(i);
          player->quickSelectBar[i] = item;
          debugUI->Text("%d ", item.GetItemId());
        }

        bool showChat =
            true;  // TODO: Move somewhere into render to show globally instead
                   // of console_activate & console_log, this is just temp
        chat->Render(&showChat);

        /*
         *  BUG: When player joins the world, the chunks arent loaded causing
         this to crash when joining the world.
         *
                            debugUI.Text("===============");


                            auto chunkpos = mc::ToVector3i(mc::Vector3d(
                                    player->position.x,
                                    0,
                                    player->position.z
                            ));

                            auto blockpos = mc::ToVector3i(mc::Vector3d(
                                    player->position.x,
                                    player->position.y - 1,
                                    player->position.z
                            ));

                            auto relativeBlockPos = mc::Vector3i(
                                    blockpos.x & 15,
                                    blockpos.y,
                                    blockpos.z & 15
                            );

                            auto readBlockPos =
         client->GetWorld()->GetChunk(chunkpos)->GetBlock(relativeBlockPos);

                            debugUI.Text(
                                    "%s %d,%f at: %s in chunk: %s",
                                    readBlockPos->GetName().c_str(),
                                    readBlockPos->GetType(),
                                    static_cast<float>(readBlockPos->GetType()),
                                    to_string(mc::Vector3i(0,3,0)).c_str(),
                                    to_string(mc::Vector3i(0,0,0)).c_str()
                            );
                            debugUI.Text("===============");
                            */
      });
    }

    debugUI->Text("%d ", dimension);

  } else if (this->gamestate == GameState_SelectWorld) {
    char path[256];
    char name[WORLD_NAME_SIZE] = {'\0'};
    WorldGenType worldType;
    bool newWorld = false;
    bool isMultiplayer = false;
    if (WorldSelect_Update(path, name, &worldType, player, &newWorld,
                           &isMultiplayer)) {
      if (!isMultiplayer) {
        strcpy(world->name, name);
        world->genSettings.type = worldType;

        savemgr.Load(path);

        chunkWorker.SetHandlerActive(
            WorkerItemType_BaseGen, &flatGen,
            world->genSettings.type == WorldGen_SuperFlat);
        chunkWorker.SetHandlerActive(WorkerItemType_BaseGen, &smeaGen,
                                     world->genSettings.type == WorldGen_Smea);

        world->cacheTranslationX =
            WorldToChunkCoord(FastFloor(player->position.x));
        world->cacheTranslationZ =
            WorldToChunkCoord(FastFloor(player->position.z));
        for (int i = 0; i < CHUNKCACHE_SIZE; i++) {
          for (int j = 0; j < CHUNKCACHE_SIZE; j++) {
            world->chunkCache[i][j] = world->LoadChunk(
                i - CHUNKCACHE_SIZE / 2 + world->cacheTranslationX,
                j - CHUNKCACHE_SIZE / 2 + world->cacheTranslationZ);
          }
        }

        for (int i = 0; i < 3; i++) {
          while (chunkWorker.IsWorking() ||
                 !chunkWorker.GetQueue().queue.empty()) {
            svcSleepThread(50000000);  // 1 Tick
          }
          world->Tick();
        }

        if (newWorld) {
          int highestblock = 0;
          for (int x = -1; x < 1; x++) {
            for (int z = -1; z < 1; z++) {
              int height = world->GetHeight(x, z);
              if (height > highestblock) highestblock = height;
            }
          }
          player->hunger = 20;
          player->hp = 20;
          player->position.y = (float)highestblock + 0.2f;
        }
        this->gamestate = GameState_Playing;
      } else {
        mcBridge.connect();
        // terra::T_World world(mcBridge.getClient()->GetDispatcher());
        // auto mesh_gen =
        // std::make_shared<terra::render::ChunkMeshGenerator>(&world,
        // camera.GetPosition());
        this->chat = new GuiChat(mcBridge.getClient()->GetDispatcher(),
                                 mcBridge.getClient());
        mcBridge.startBackgroundThread();
        this->gamestate = GameState_Playing_OnLine;
      }
    }
  }
  Gui::InputData(inputData);
}