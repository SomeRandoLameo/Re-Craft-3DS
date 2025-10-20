#include "ReCraftCore.h"

ReCraftCore* ReCraftCore::Instance = nullptr;

ReCraftCore::ReCraftCore()
        : player(nullptr), renderer(nullptr, nullptr, nullptr, nullptr) {
    Instance = this;
}

ReCraftCore::~ReCraftCore() {
    Instance = nullptr;
}

void ReCraftCore::Init() {
    gfxInitDefault();
    osSetSpeedupEnable(true);
    gfxSet3D(true);
    romfsInit();
    sino_init();

    SuperChunk_InitPools();
    SaveManager_InitFileSystem();

    // Chunk worker setup
    ChunkWorker_Init(&chunkWorker);
    ChunkWorker_AddHandler(&chunkWorker, WorkerItemType_PolyGen, (WorkerFuncObj){&PolyGen_GeneratePolygons, NULL, true});
    ChunkWorker_AddHandler(&chunkWorker, WorkerItemType_BaseGen, (WorkerFuncObj){&SuperFlatGen_Generate, &flatGen, true});
    ChunkWorker_AddHandler(&chunkWorker, WorkerItemType_BaseGen, (WorkerFuncObj){&SmeaGen_Generate, &smeaGen, true});

    world = (World*)malloc(sizeof(World));
    World_Init(world, &chunkWorker.queue);

    player = Player(world);
    PlayerController_Init(&playerController, &player);

    SuperFlatGen_Init(&flatGen, world);
    SmeaGen_Init(&smeaGen, world);

    renderer = Renderer(world, &player, &chunkWorker.queue, &gameState);
    debugUI = DebugUI();

    SaveManager_Init(&saveManager, &player);
    ChunkWorker_AddHandler(&chunkWorker, WorkerItemType_Load, (WorkerFuncObj){&SaveManager_LoadChunk, &saveManager, true});
    ChunkWorker_AddHandler(&chunkWorker, WorkerItemType_Save, (WorkerFuncObj){&SaveManager_SaveChunk, &saveManager, true});

    WorldSelect_Init();
    lastTime = svcGetSystemTick();
}

void ReCraftCore::Run() {
    while (aptMainLoop()) {
        debugUI.Text("%d FPS  Usage: CPU: %5.2f%% GPU: %5.2f%% Buf: %5.2f%% Lin: %d", fps,
                     C3D_GetProcessingTime() * 6.f,
                     C3D_GetDrawingTime() * 6.f,
                     C3D_GetCmdBufUsage() * 100.f,
                     linearSpaceFree());

        debugUI.Text("X: %f, Y: %f, Z: %f", f3_unpack(player.position));
        debugUI.Text("HP: %i", player.hp);

        renderer.Render(&debugUI);

        uint64_t currentTime = svcGetSystemTick();
        dt = ((float)(currentTime / (float)CPU_TICKS_PER_MSEC) - (float)(lastTime / (float)CPU_TICKS_PER_MSEC)) / 1000.f;
        lastTime = currentTime;
        timeAccum += dt;

        frameCounter++;
        fpsClock += dt;
        if (fpsClock >= 1.f) {
            fps = frameCounter;
            frameCounter = 0;
            fpsClock = 0.f;
        }

        hidScanInput();
        u32 keysHeld = ::hidKeysHeld();
        u32 keysDown = ::hidKeysDown();

        if (keysDown & KEY_START) {
            if (gameState == GameState_SelectWorld)
                break;
            else if (gameState == GameState_Playing) {
                ReleaseWorld();
                gameState = GameState_SelectWorld;
                WorldSelect_ScanWorlds();
                lastTime = svcGetSystemTick();
            }
        }

        circlePosition circlePos;
        hidCircleRead(&circlePos);
        circlePosition cstickPos;
        hidCstickRead(&cstickPos);
        touchPosition touchPos;
        hidTouchRead(&touchPos);

        InputData inputData = (InputData){keysHeld,    keysDown,    hidKeysUp(),  circlePos.dx, circlePos.dy,
                                          touchPos.px, touchPos.py, cstickPos.dx, cstickPos.dy};

        if (gameState == GameState_Playing) {
            while (timeAccum >= 1.f / 20.f) {
                World_Tick(world);
                timeAccum -= 1.f / 20.f;
            }

            PlayerController_Update(&playerController, &debugUI, inputData, dt);
            World_UpdateChunkCache(world,
                                   WorldToChunkCoord(FastFloor(player.position.x)),
                                   WorldToChunkCoord(FastFloor(player.position.z)));
        } else if (gameState == GameState_SelectWorld) {
            char path[256];
            char name[WORLD_NAME_SIZE] = {'\0'};
            WorldGenType worldType;
            bool newWorld = false;

            if (WorldSelect_Update(path, name, &worldType, &newWorld)) {
                strcpy(world->name, name);
                world->genSettings.type = worldType;
                SaveManager_Load(&saveManager, path);

                ChunkWorker_SetHandlerActive(&chunkWorker, WorkerItemType_BaseGen, &flatGen,
                                             world->genSettings.type == WorldGen_SuperFlat);
                ChunkWorker_SetHandlerActive(&chunkWorker, WorkerItemType_BaseGen, &smeaGen,
                                             world->genSettings.type == WorldGen_Smea);

                world->cacheTranslationX = WorldToChunkCoord(FastFloor(player.position.x));
                world->cacheTranslationZ = WorldToChunkCoord(FastFloor(player.position.z));

                for (int i = 0; i < CHUNKCACHE_SIZE; i++) {
                    for (int j = 0; j < CHUNKCACHE_SIZE; j++) {
                        world->chunkCache[i][j] =
                                World_LoadChunk(world,
                                                i - CHUNKCACHE_SIZE / 2 + world->cacheTranslationX,
                                                j - CHUNKCACHE_SIZE / 2 + world->cacheTranslationZ);
                    }
                }

                for (int i = 0; i < 3; i++) {
                    while (chunkWorker.working || chunkWorker.queue.queue.length > 0)
                        svcSleepThread(50000000);
                    World_Tick(world);
                }

                if (newWorld) {
                    int highestBlock = 0;
                    for (int x = -1; x < 1; x++) {
                        for (int z = -1; z < 1; z++) {
                            int height = World_GetHeight(world, x, z);
                            if (height > highestBlock)
                                highestBlock = height;
                        }
                    }
                    player.hunger = 20;
                    player.hp = 20;
                    player.position.y = (float)highestBlock + 0.2f;
                }

                gameState = GameState_Playing;
                lastTime = svcGetSystemTick();
            }
        }

        Gui_InputData(inputData);
    }
}

void ReCraftCore::ReleaseWorld() {
    for (int i = 0; i < CHUNKCACHE_SIZE; i++) {
        for (int j = 0; j < CHUNKCACHE_SIZE; j++) {
            World_UnloadChunk(world, world->chunkCache[i][j]);
        }
    }
    ChunkWorker_Finish(&chunkWorker);
    World_Reset(world);
    SaveManager_Unload(&saveManager);
}

void ReCraftCore::Shutdown() {
    if (gameState == GameState_Playing)
        ReleaseWorld();

    SaveManager_Deinit(&saveManager);
    SuperChunk_DeinitPools();

    free(world);

    WorldSelect_Deinit();
    debugUI.~DebugUI();
    ChunkWorker_Deinit(&chunkWorker);
    renderer.~Renderer();

    ndspExit();
    sino_exit();
    romfsExit();
    gfxExit();
}