#include "ReCraftCore.hpp"
#include "gui/screens/GuiInGame.hpp"
#include "gui/screens/SelectWorldScreen.hpp"
#include "gui/screens/StartScreen.hpp"

#include "input/InputManager.hpp"
#include "input/PlayerInput.hpp"

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

    SetScreen(new StartTopScreen, true);
    SetScreen(new StartBotScreen, false);
    // WorldSelect_Init();

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
        /*ImGui::Text("malloc: %lld - free: %lld\ncalloc: %lld - realloc: %lld\nmemalign: %lld",
                    Amy::Memory::GetMetrics()._malloc, Amy::Memory::GetMetrics()._free,
                    Amy::Memory::GetMetrics()._calloc, Amy::Memory::GetMetrics()._realloc,
                    Amy::Memory::GetMetrics()._memalign);
        */
        ImGui::Text("Linear Heap: %s", Amy::Utils::FormatBytes(linearSpaceFree()).c_str());
        ImGui::Separator();
        ImGui::Text("Player");
        ImGui::Separator();

        ImGui::Spacing();
        ImGui::Text("Position: %.2f, %.2f, %.2f", m_player->position.x, m_player->position.y, m_player->position.z);
        ImGui::Text("Spawn: %.2f, %.2f, %.2f", m_player->spawnPos.x, m_player->spawnPos.y, m_player->spawnPos.z);
        ImGui::Text("HP: %i", m_player->hp);
        ImGui::Text("Hunger: %i (Timer: %i)", m_player->hunger, m_player->hungertimer);
        ImGui::Text("Velocity Y: %.3f, RndY: %.3f", m_player->velocity.y, m_player->rndy);
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
    // WorldSelect_Deinit();

    delete m_renderer;
    m_chunkWorker.~ChunkWorker();

    romfsExit();
    gfxExit();
}

void ReCraftCore::InitSinglePlayer(char* path, char* name, const WorldGenType* worldType, Gamemode mode,
                                   bool newWorld) {
    m_chunkWorker.AddHandler(WorkerItemType::BaseGen, (WorkerFuncObj){&SuperFlatGen::Generate, &m_flatGen, true});

    m_chunkWorker.AddHandler(WorkerItemType::BaseGen, (WorkerFuncObj){&SmeaGen::Generate, &m_smeaGen, true});

    strcpy(m_world->name, name);
    m_world->genSettings.type = *worldType;

    m_savemgr.Load(path);

    m_chunkWorker.SetHandlerActive(WorkerItemType::BaseGen, &m_flatGen,
                                   m_world->genSettings.type == WorldGenType::SuperFlat);

    m_chunkWorker.SetHandlerActive(WorkerItemType::BaseGen, &m_smeaGen,
                                   m_world->genSettings.type == WorldGenType::Smea);

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

        m_player->gamemode = mode;
        m_player->hunger = 20;
        m_player->hp = 20;
        m_player->position.y = (float)highestblock + 0.2f;
    }

    SetScreen(new GuiInGameTop, true);
    SetScreen(new GuiInGameBot, false);
    m_gamestate = GameState::Playing;
}
void ReCraftCore::RunSinglePlayer() {
    while (m_timeAccum >= 20.f) {
        m_world->Tick();

        m_timeAccum = -20.f;
    }

    m_player->UpdateMovement(m_debugUI, Delta() * 0.001);

    m_world->UpdateChunkCache(WorldToChunkCoord(FastFloor(m_player->position.x)),
                              WorldToChunkCoord(FastFloor(m_player->position.z)));
}
void ReCraftCore::ExitSinglePlayer() {
    if (m_world)
        m_world->Release(&m_chunkWorker, &m_savemgr);
    SetScreen(new StartTopScreen, true);
    SetScreen(new SelectWorldBotScreen, false);
}

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

    // m_player->position = m_mcBridge.getClient()->GetPlayerController()->GetPosition();

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

    m_player->position = mc::Vector3d(0, 63, 0);

    m_mcBridge.startBackgroundThread();
    SetScreen(new GuiInGameTop, true);
    SetScreen(new GuiInGameBot, false);
    m_gamestate = GameState::Playing_OnLine;
}

void ReCraftCore::ExitMultiplayer() {
    m_mcBridge.stopBackgroundThread();
    m_mcBridge.disconnect();
    SetScreen(new StartTopScreen, true);
    SetScreen(new SelectWorldBotScreen, false);
}


void ReCraftCore::RunMultiPlayer() {
    if (m_mcBridge.isConnected()) {
        m_mcBridge.withClient([&](mc::core::Client* client, mc::protocol::packets::PacketDispatcher* dispatcher) {
            // TODO: Move this into some NetworkPlayer class that gets updated from here.
            // NetworkPlayer needs to listen to the server propperly to initialize the player (eg. setting position,
            // rotation and much more)
            // TODO: Yaw is inverted, Server recieves movement as jittery, might be mclib
            mc::protocol::packets::out::PlayerPositionAndLookPacket response(
                m_player->position, m_player->yaw * 180.0f / 3.14159f, m_player->pitch * 180.0f / 3.14159f,
                m_player->grounded);
            client->GetConnection()->SendPacket(&response);

            // Experimental, doesnt work
            // if (m_player->crouching && m_player->releasedCrouch) {
            //     mc::protocol::packets::out::EntityActionPacket packet(0,
            //     mc::protocol::packets::out::EntityActionPacket::Action::StopSneak);
            //
            //     client->GetConnection()->SendPacket(&packet);
            //
            //     m_player->crouching = false;
            // } else if (!m_player->crouching && !m_player->releasedCrouch) {
            //     mc::protocol::packets::out::EntityActionPacket packet(0,
            //     mc::protocol::packets::out::EntityActionPacket::Action::StartSneak);

            //    client->GetConnection()->SendPacket(&packet);

            //    m_player->crouching = true;
            //}

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

    m_player->UpdateMovement(m_debugUI, Delta() * 0.001);

    m_world->UpdateChunkCache(WorldToChunkCoord(FastFloor(m_player->position.x)),
                              WorldToChunkCoord(FastFloor(m_player->position.z)));
}

void ReCraftCore::Main() {
    m_renderer->Render(m_debugUI);

    if (Input::isKeyDown(KEY_START)) { // TODO: Change this in favor of pause screen
        if (m_gamestate == GameState::SelectWorld)
            Exit();
        else if (m_gamestate == GameState::Playing) {
            ExitSinglePlayer();

            m_gamestate = GameState::SelectWorld;

            // WorldSelect_ScanWorlds();
        } else if (m_gamestate == GameState::Playing_OnLine) {
            ExitMultiplayer();
            m_gamestate = GameState::SelectWorld;
            // WorldSelect_ScanWorlds();
        }
    }

    Input::getMgr()->poll();

    m_timeAccum += Delta();


    if (m_pTopScreen) {
        m_bTopUsingCurrScreen = true;
        // m_debugUI->Log("UPDATE TOP");
        m_pTopScreen->UpdateEvents();
        m_bTopUsingCurrScreen = false;
        if (m_bTopHaveQueuedScreen) {
            SetScreen(m_pTopQueuedScreen, true);
            m_pTopQueuedScreen = nullptr;
            m_bTopHaveQueuedScreen = false;
        }
        // return;
    }
    if (m_pBotScreen) {
        m_bBotUsingCurrScreen = true;
        // m_debugUI->Log("UPDATE BOT");
        m_pBotScreen->UpdateEvents();
        m_bBotUsingCurrScreen = false;
        if (m_bBotHaveQueuedScreen) {
            SetScreen(m_pBotQueuedScreen, false);
            m_pBotQueuedScreen = nullptr;
            m_bBotHaveQueuedScreen = false;
        }
        // return;
    }

    if (m_gamestate == GameState::Playing) {
        RunSinglePlayer();

    } else if (m_gamestate == GameState::Playing_OnLine) {
        RunMultiPlayer();

    } else if (m_gamestate == GameState::SelectWorld) {
        // WorldSelect_Update(m_player);
    }
}

void ReCraftCore::SetScreen(Screen* pScreen, bool top) {
    bool& usingCurrScreen = top ? m_bTopUsingCurrScreen : m_bBotUsingCurrScreen;
    bool& haveQueuedScreen = top ? m_bTopHaveQueuedScreen : m_bBotHaveQueuedScreen;
    Screen*& queuedScreen = top ? m_pTopQueuedScreen : m_pBotQueuedScreen;
    Screen*& currentScreen = top ? m_pTopScreen : m_pBotScreen;

    int screenWidth = top ? int(400 * 0.5f) : int(320 * 0.5);
    int screenHeight = int(240 * 0.5);

    if (usingCurrScreen) {
        haveQueuedScreen = true;
        queuedScreen = pScreen;
    } else if (!pScreen || !pScreen->IsErrorScreen()) {
        if (currentScreen) {
            currentScreen->Removed();
            delete currentScreen;
        }

        currentScreen = pScreen;
        if (pScreen) {
            pScreen->Init(this, screenWidth, screenHeight);
        }
    } else {
        delete pScreen;
    }
}
