#include "ReCraftCore.h"

bool showDebugInfo = true;
ReCraftCore* ReCraftCore::m_theReCraftCore = nullptr;

ReCraftCore::ReCraftCore() {
    m_theReCraftCore = this;
    this->m_gamestate = GameState::SelectWorld;
    gfxInitDefault();
    // Enable N3DS 804MHz operation, where available
    osSetSpeedupEnable(true);
    gfxSet3D(true);
    romfsInit();

    SuperChunk_InitPools();
    SaveManager::InitFileSystem();

    m_chunkWorker.AddHandler(WorkerItemType::PolyGen, (WorkerFuncObj){&PolyGen_GeneratePolygons, nullptr, true});

    sino_init();

    m_world = new World(&m_chunkWorker.GetQueue());
    m_player = new Player(m_world);
    m_playerCtrl = new PlayerController(m_player);

    m_flatGen.Init(m_world);
    m_smeaGen.Init(m_world);
    m_emptyGen.Init(m_world);

    auto fnt = Iron::Font::New();
    fnt->LoadBMF("romfs:/assets/textures/font/ascii.png");
    m_AssetMgr.Add("font", fnt);

    m_AssetMgr.AutoLoad("GuiTexture_Widgets", "romfs:/assets/textures/gui/widgets.png");

    // m_AssetMgr.AutoLoad("font", "romfs:/ComicNeue.ttf");

    m_renderer = new Renderer(m_world, m_player, &m_chunkWorker.GetQueue());
    m_debugUI = new DebugUI();

    WorldSelect_Init();

    m_savemgr.Init(m_player, m_world);

    m_chunkWorker.AddHandler(WorkerItemType::Load, (WorkerFuncObj){SaveManager::LoadChunkCallback, &m_savemgr, true});

    m_chunkWorker.AddHandler(WorkerItemType::Save, (WorkerFuncObj){SaveManager::SaveChunkCallback, &m_savemgr, true});

    ImGuiManager::GetInstance()->RegisterCallback("DebugUI", [=, this]() {
        ImGui::Begin("Game Info");

        ImGui::Text("Performance");
        ImGui::Spacing();
        ImGui::Text("FPS: %.2f, DT: %.3f", 1000.f / Delta(), Delta());
        ImGui::Text("CPU: %5.2f%%", C3D_GetProcessingTime() * 6.f);
        ImGui::Text("GPU: %5.2f%%", C3D_GetDrawingTime() * 6.f);
        ImGui::Text("Buffer: %5.2f%%", C3D_GetCmdBufUsage() * 100.f);
        ImGui::Text("Heap: %s [%d] Allocations", Amy::Utils::FormatBytes(Amy::Memory::GetTotalAllocated()).c_str(),
                    Amy::Memory::GetAllocationCount());
        ImGui::Text("Linear Heap: %s", Amy::Utils::FormatBytes(linearSpaceFree()).c_str());
        ImGui::Separator();
        ImGui::Text("Player");
        ImGui::Spacing();
        ImGui::Text("Position: %.2f, %.2f, %.2f", m_player->position.x, m_player->position.y, m_player->position.z);
        ImGui::Text("Spawn: %.2f, %.2f, %.2f", m_player->spawnPos.x, m_player->spawnPos.y, m_player->spawnPos.z);
        ImGui::Text("HP: %i", m_player->hp);
        ImGui::Text("Hunger: %i (Timer: %i)", m_player->hunger, m_player->hungertimer);
        ImGui::Text("Velocity Y: %.3f, RndY: %.3f", m_player->velocity.y, m_player->rndy);
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

    if (m_gamestate == GameState::Playing) {
        ExitSinglePlayer();
    }

    if (m_gamestate == GameState::Playing_OnLine) {
        ExitMultiplayer();
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

void ReCraftCore::InitSinglePlayer(char* path, char* name, const WorldGenType* worldType, bool newWorld) {

    m_chunkWorker.AddHandler(WorkerItemType::BaseGen, (WorkerFuncObj){&SuperFlatGen::Generate, &m_flatGen, true});

    m_chunkWorker.AddHandler(WorkerItemType::BaseGen, (WorkerFuncObj){&SmeaGen::Generate, &m_smeaGen, true});

    strcpy(m_world->name, name);
    m_world->genSettings.type = *worldType;

    m_savemgr.Load(path);

    m_chunkWorker.SetHandlerActive(WorkerItemType::BaseGen, &m_flatGen, m_world->genSettings.type == WorldGenType::SuperFlat);

    m_chunkWorker.SetHandlerActive(WorkerItemType::BaseGen, &m_smeaGen, m_world->genSettings.type == WorldGenType::Smea);

    // TODO: Potential World::InitChunkCache() function?
    m_world->cacheTranslationX = WorldToChunkCoord(FastFloor(m_player->position.x));
    m_world->cacheTranslationZ = WorldToChunkCoord(FastFloor(m_player->position.z));
    for (int i = 0; i < World::ChunkCacheSize; i++) {
        for (int j = 0; j < World::ChunkCacheSize; j++) {
            m_world->columnCache[i][j] = m_world->LoadChunk(i - World::ChunkCacheSize / 2 + m_world->cacheTranslationX,
                                                            j - World::ChunkCacheSize / 2 + m_world->cacheTranslationZ);
        }
    }

    for (int i = 0; i < 3; i++) { // why 3?
        while (m_chunkWorker.IsWorking() || !m_chunkWorker.GetQueue().queue.empty()) {
            svcSleepThread(50000000); // 1 Tick
        }
        m_world->Tick();
    }

    if (newWorld) {
        int highestblock = 0;
        for (int x = -1; x < 1; x++) {
            for (int z = -1; z < 1; z++) {
                int height = m_world->GetHeight(x, z);
                if (height > highestblock)
                    highestblock = height;
            }
        }

        m_player->hunger = 20;
        m_player->hp = 20;
        m_player->position.y = (float)highestblock + 0.2f;
    }
    m_gamestate = GameState::Playing;
}
void ReCraftCore::RunSinglePlayer(InputData inputData) {
    while (m_timeAccum >= 20.f) {
        m_world->Tick();

        m_timeAccum = -20.f;
    }

    m_player->UpdateMovement(m_playerCtrl->GetControlScheme(), inputData, Delta() * 0.001);

    m_world->UpdateChunkCache(WorldToChunkCoord(FastFloor(m_player->position.x)),
                              WorldToChunkCoord(FastFloor(m_player->position.z)));
}
void ReCraftCore::ExitSinglePlayer() { ReleaseWorld(&m_chunkWorker, &m_savemgr,  m_world); }

// TODO: Something prevents the 03DS from connecting. It isnt memory. My guess is that world data takes too long to load
// and then server timeout?
void ReCraftCore::InitMultiPlayer() {


    m_chunkWorker.AddHandler(WorkerItemType::BaseGen, (WorkerFuncObj){&EmptyGen::Generate, &m_emptyGen, true});

    SwkbdState swkbd;
    char buffer1[256];

    swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 2, -1);
    swkbdSetHintText(&swkbd, "Username");
    swkbdInputText(&swkbd, buffer1, sizeof(buffer1));
    m_mcBridge.SetUsername(buffer1);

    char buffer2[256];
    swkbdInit(&swkbd, SWKBD_TYPE_NORMAL, 2, -1);
    swkbdSetHintText(&swkbd, "Server IP (xxx.xxx.xxx.xxx)");
    swkbdInputText(&swkbd, buffer2, sizeof(buffer2));
    m_mcBridge.SetIPAddress(buffer2);

    m_mcBridge.connect();
    m_world->genSettings.type = WorldGenType::Empty;

    m_player->hunger = 20;
    m_player->hp = 20;
    m_player->gamemode = 1;

    //m_player->position = m_mcBridge.getClient()->GetPlayerController()->GetPosition();

    m_chat = new GuiChat(m_mcBridge.getClient()->GetDispatcher(), m_mcBridge.getClient());
    m_networkWorld = new NetworkWorld(m_world, m_mcBridge.getClient()->GetDispatcher());

    m_chunkWorker.SetHandlerActive(WorkerItemType::BaseGen, &m_emptyGen, true);

    // TODO: Potential World::InitChunkCache() function?
    m_world->cacheTranslationX = WorldToChunkCoord(FastFloor(m_player->position.x));
    m_world->cacheTranslationZ = WorldToChunkCoord(FastFloor(m_player->position.z));
    for (int i = 0; i < World::ChunkCacheSize; i++) {
        for (int j = 0; j < World::ChunkCacheSize; j++) {
            m_world->columnCache[i][j] = m_world->LoadChunk(i - World::ChunkCacheSize / 2 + m_world->cacheTranslationX,
                                                            j - World::ChunkCacheSize / 2 + m_world->cacheTranslationZ);
        }
    }

    for (int i = 0; i < 3; i++) { // why 3?
        while (m_chunkWorker.IsWorking() || !m_chunkWorker.GetQueue().queue.empty()) {
            svcSleepThread(50000000); // 1 Tick
        }
        m_world->Tick();
    }

    //m_networkWorld->Test(); // Loading test blocks into world

    m_player->position = mc::Vector3d(0,63,0);

    m_mcBridge.startBackgroundThread();
    //m_gamestate = GameState::Playing;
    m_gamestate = GameState::Playing_OnLine;
}

void ReCraftCore::ExitMultiplayer() {
    m_mcBridge.stopBackgroundThread();
    m_mcBridge.disconnect();
}


void ReCraftCore::RunMultiPlayer(InputData inputData) {
    if (m_mcBridge.isConnected()) {
        m_mcBridge.withClient([&](mc::core::Client* client, mc::protocol::packets::PacketDispatcher* dispatcher) {
            //m_player->position = client->GetPlayerController()->GetPosition();

            // TODO: This is baaaad :D
            //client->GetPlayerController()->Move(mc::Vector3d(-m_playerCtrl->movement.x * (0.125 / 2),
            //                                                 -m_playerCtrl->movement.y * (0.125 / 2),
             //                                                -m_playerCtrl->movement.z * (0.125 / 2)));

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
            bool showChat = true; // TODO: Move somewhere into render to show globally instead
            // of console_activate & console_log, this is just temp
            m_chat->Render(&showChat);
        });
    }

    while (m_timeAccum >= 20.f) {
        m_world->Tick();

        m_timeAccum = -20.f;
    }

    m_player->UpdateMovement(m_playerCtrl->GetControlScheme(), inputData, Delta() * 0.001);

    m_world->UpdateChunkCache(WorldToChunkCoord(FastFloor(m_player->position.x)),
                              WorldToChunkCoord(FastFloor(m_player->position.z)));
}

// TODO: Why isnt this in world?
void ReCraftCore::ReleaseWorld(ChunkWorker* chunkWorker, SaveManager* savemgr, World* world) {
    for (int i = 0; i < World::ChunkCacheSize; i++) {
        for (int j = 0; j < World::ChunkCacheSize; j++) {
            world->UnloadChunk(world->columnCache[i][j]);
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
        if (m_gamestate == GameState::SelectWorld)
            Exit();
        else if (m_gamestate == GameState::Playing) {
            ExitSinglePlayer();

            m_gamestate = GameState::SelectWorld;

            WorldSelect_ScanWorlds();
        } else if (m_gamestate == GameState::Playing_OnLine) {
            ExitMultiplayer();
            m_gamestate = GameState::SelectWorld;
            WorldSelect_ScanWorlds();
        }
    }

    circlePosition circlePos;
    hidCircleRead(&circlePos);

    circlePosition cstickPos;
    hidCstickRead(&cstickPos);

    touchPosition touchPos;
    hidTouchRead(&touchPos);

    InputData inputData = (InputData){keysheld,    keysdown,    hidKeysUp(),  circlePos.dx, circlePos.dy,
                                      touchPos.px, touchPos.py, cstickPos.dx, cstickPos.dy};

    m_timeAccum += Delta();
    if (m_gamestate == GameState::Playing) {
        RunSinglePlayer(inputData);

    } else if (m_gamestate == GameState::Playing_OnLine) {
        RunMultiPlayer(inputData);

    } else if (m_gamestate == GameState::SelectWorld) {
        WorldSelect_Update(m_player);

    }
    Gui::InputData(inputData);
}
