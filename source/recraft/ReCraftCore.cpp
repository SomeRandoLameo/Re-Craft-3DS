#include "ReCraftCore.h"

bool showDebugInfo = true;
ReCraftCore *ReCraftCore::m_theReCraftCore = nullptr;

ReCraftCore::ReCraftCore() {
    m_theReCraftCore = this;
    this->m_gamestate = GameState_SelectWorld;
    gfxInitDefault();
    // Enable N3DS 804MHz operation, where available
    osSetSpeedupEnable(true);
    gfxSet3D(true);
    romfsInit();

    SuperChunk_InitPools();
    m_savemgr.InitFileSystem();

    m_chunkWorker.AddHandler(
        WorkerItemType_PolyGen,
        (WorkerFuncObj) {&PolyGen_GeneratePolygons, nullptr, true}
    );

    m_chunkWorker.AddHandler(
        WorkerItemType_BaseGen,
        (WorkerFuncObj) {&SuperFlatGen::Generate, &m_flatGen, true}
    );

    m_chunkWorker.AddHandler(
        WorkerItemType_BaseGen,
        (WorkerFuncObj) {&SmeaGen::Generate, &m_smeaGen, true}
    );

    sino_init();

    m_world = new World(&m_chunkWorker.GetQueue());
    m_player = new Player(m_world);
    m_playerCtrl = new PlayerController(m_player);

    m_flatGen.Init(m_world);
    m_smeaGen.Init(m_world);

    auto fnt = Iron::Font::New();
    fnt->LoadBMF("romfs:/assets/textures/font/ascii.png");
    m_AssetMgr.Add("font", fnt);

    m_AssetMgr.AutoLoad("GuiTexture_Widgets", "romfs:/assets/textures/gui/widgets.png");

    //m_AssetMgr.AutoLoad("font", "romfs:/ComicNeue.ttf");

    m_renderer = new Renderer(m_world, m_player, &m_chunkWorker.GetQueue());
    m_debugUI = new DebugUI();

    WorldSelect_Init();

    m_savemgr.Init(m_player, m_world);
    m_chunkWorker.AddHandler(
        WorkerItemType_Load,
        (WorkerFuncObj) {SaveManager::LoadChunkCallback, &m_savemgr, true}
    );

    m_chunkWorker.AddHandler(
        WorkerItemType_Save,
        (WorkerFuncObj) {SaveManager::SaveChunkCallback, &m_savemgr, true}
    );

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
        ImGui::Text("Position: %.2f, %.2f, %.2f", m_player->position.x,
                    m_player->position.y, m_player->position.z);
        ImGui::Text("Spawn: %.2f, %.2f, %.2f", m_player->spawnPos.x,
                    m_player->spawnPos.y, m_player->spawnPos.z);
        ImGui::Text("HP: %i", m_player->hp);
        ImGui::Text("Hunger: %i (Timer: %i)", m_player->hunger, m_player->hungertimer);
        ImGui::Text("Velocity Y: %.3f, RndY: %.3f", m_player->velocity.y,
                    m_player->rndy);
        ImGui::Separator();
        ImGui::Text("Inventory");
        ImGui::Spacing();
        ImGui::Text("Gamemode: %i", m_player->gamemode);
        ImGui::Text("Quickbar Slot: %i", m_player->quickSelectBarSlot);

        ImGui::End();
    });
    // i want to use imgui anywhere
}

ReCraftCore::~ReCraftCore() {
    ImGuiManager::GetInstance()->UnregisterCallback("DebugUI");

    if (m_gamestate == GameState_Playing) {
        ReleaseWorld(&m_chunkWorker, &m_savemgr, m_world);
    }

    if (m_gamestate == GameState_Playing_OnLine) {
        m_mcBridge.stopBackgroundThread();
        m_mcBridge.disconnect();
    }

    m_savemgr.~SaveManager();
    SuperChunk_DeinitPools();

    delete m_debugUI;
    delete m_world;
    sino_exit();
    WorldSelect_Deinit();

    delete m_renderer;
    m_chunkWorker.~ChunkWorker();

    romfsExit();
    gfxExit();
}

// TODO: Why isnt this in world?
void ReCraftCore::ReleaseWorld(ChunkWorker *chunkWorker, SaveManager *savemgr, World *world) {
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
    m_renderer->Render(m_debugUI);

    hidScanInput();
    u32 keysheld = hidKeysHeld(), keysdown = hidKeysDown();
    if (keysdown & KEY_START) {
        if (m_gamestate == GameState_SelectWorld)
            Exit();
        else if (m_gamestate == GameState_Playing) {
            ReleaseWorld(&m_chunkWorker, &m_savemgr, m_world);

            m_gamestate = GameState_SelectWorld;

            WorldSelect_ScanWorlds();
        }
    }

    circlePosition circlePos;
    hidCircleRead(&circlePos);

    circlePosition cstickPos;
    hidCstickRead(&cstickPos);

    touchPosition touchPos;
    hidTouchRead(&touchPos);

    InputData inputData = (InputData) {
        keysheld, keysdown, hidKeysUp(),
        circlePos.dx, circlePos.dy, touchPos.px,
        touchPos.py, cstickPos.dx, cstickPos.dy
    };

    m_timeAccum += Delta();
    if (m_gamestate == GameState_Playing) {
        while (m_timeAccum >= 20.f) {
            m_world->Tick();

            m_timeAccum = -20.f;
        }

        m_player->UpdateMovement(m_playerCtrl->GetControlScheme(),inputData, Delta() * 0.001);

        m_world->UpdateChunkCache(
            WorldToChunkCoord(FastFloor(m_player->position.x)),
            WorldToChunkCoord(FastFloor(m_player->position.z))
        );

    } else if (m_gamestate == GameState_Playing_OnLine) {

        if (m_mcBridge.isConnected()) {
            m_mcBridge.withClient([&](mc::core::Client *client, mc::protocol::packets::PacketDispatcher * dispatcher) {
                m_player->position = client->GetPlayerController()->GetPosition();

                //dimension = client->GetConnection()->GetDimension();
                m_player->UpdateMovement(m_playerCtrl->GetControlScheme(), inputData, Delta() * 0.001);

                // TODO: This is baaaad :D

                client->GetPlayerController()->Move(
                    mc::Vector3d(-m_playerCtrl->movement.x * (0.125 / 2),
                         -m_playerCtrl->movement.y * (0.125 / 2),
                         -m_playerCtrl->movement.z * (0.125 / 2)
                    )
                );

                client->GetPlayerController()->SetPitch(-m_playerCtrl->player->pitch);
                client->GetPlayerController()->SetYaw(-m_playerCtrl->player->yaw);
                /*
                 // TODO: packet spam prevention... This is enough for now
                 for (int i = 0; i < 9; i++) {
                   auto item = client->GetHotbar().GetItem(i);
                   player->quickSelectBar[i] = item;
                   debugUI->Text("%d ", item.GetItemId());
                 }
         */
                bool showChat = true;  // TODO: Move somewhere into render to show globally instead
                // of console_activate & console_log, this is just temp
                m_chat->Render(&showChat);

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


            m_world->UpdateChunkCache(WorldToChunkCoord(FastFloor(m_player->position.x)),
                                    WorldToChunkCoord(FastFloor(m_player->position.z)));
        }

    } else if (m_gamestate == GameState_SelectWorld) {
        char path[256];
        char name[WORLD_NAME_SIZE] = {'\0'};
        WorldGenType worldType;
        bool newWorld = false;
        bool isMultiplayer = false;
        if (WorldSelect_Update(path, name, &worldType, m_player, &newWorld,
                               &isMultiplayer)) {
            if (!isMultiplayer) {
                strcpy(m_world->name, name);
                m_world->genSettings.type = worldType;

                m_savemgr.Load(path);

                m_chunkWorker.SetHandlerActive(
                    WorkerItemType_BaseGen, &m_flatGen,
                    m_world->genSettings.type == WorldGen_SuperFlat
                );

                m_chunkWorker.SetHandlerActive(
                    WorkerItemType_BaseGen, &m_smeaGen,
                    m_world->genSettings.type == WorldGen_Smea
                );

                m_world->cacheTranslationX = WorldToChunkCoord(FastFloor(m_player->position.x));
                m_world->cacheTranslationZ = WorldToChunkCoord(FastFloor(m_player->position.z));
                for (int i = 0; i < CHUNKCACHE_SIZE; i++) {
                    for (int j = 0; j < CHUNKCACHE_SIZE; j++) {
                        m_world->chunkCache[i][j] = m_world->LoadChunk(
                            i - CHUNKCACHE_SIZE / 2 + m_world->cacheTranslationX,
                            j - CHUNKCACHE_SIZE / 2 + m_world->cacheTranslationZ
                        );
                    }
                }

                for (int i = 0; i < 3; i++) {
                    while (m_chunkWorker.IsWorking() ||
                           !m_chunkWorker.GetQueue().queue.empty()) {
                        svcSleepThread(50000000);  // 1 Tick
                    }
                    m_world->Tick();
                }

                if (newWorld) {
                    int highestblock = 0;
                    for (int x = -1; x < 1; x++) {
                        for (int z = -1; z < 1; z++) {
                            int height = m_world->GetHeight(x, z);
                            if (height > highestblock) highestblock = height;
                        }
                    }

                    m_player->hunger = 20;
                    m_player->hp = 20;
                    m_player->position.y = (float) highestblock + 0.2f;
                }
                m_gamestate = GameState_Playing;
            } else {
                m_mcBridge.connect();
                m_player->gamemode = 1;  // Creative for now
                m_chat = new GuiChat(m_mcBridge.getClient()->GetDispatcher(), m_mcBridge.getClient());
                m_onlineWorld = new OnlineWorld();
                m_mcBridge.startBackgroundThread();
                m_gamestate = GameState_Playing_OnLine;
            }
        }
    }
    Gui::InputData(inputData);
}