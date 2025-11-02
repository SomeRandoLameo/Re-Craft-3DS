#include "ReCraftCore.h"


bool showDebugInfo = true;

ReCraftCore* ReCraftCore::theReCraftCore = nullptr;

ReCraftCore::ReCraftCore() {
    theReCraftCore = this;
}

void ReCraftCore::ReleaseWorld(ChunkWorker* chunkWorker, SaveManager* savemgr, World* world) {
	for (int i = 0; i < CHUNKCACHE_SIZE; i++) {
		for (int j = 0; j < CHUNKCACHE_SIZE; j++) {
			World_UnloadChunk(world, world->chunkCache[i][j]);
		}
	}
	chunkWorker->Finish();
	World_Reset(world);

    savemgr->Unload();
}

void ReCraftCore::Run() {






    /**
    * INIT/SETUP CODE
    */
    GameState gamestate = GameState_SelectWorld;
	//printf("gfxinit\n");
	gfxInitDefault();

	// Enable N3DS 804MHz operation, where available
	osSetSpeedupEnable(true);

	//consoleInit(GFX_TOP, NULL);
	gfxSet3D(true);
	//printf("romfsinit\n");
	romfsInit();

	SuperFlatGen flatGen;
	SmeaGen smeaGen;

	SuperChunk_InitPools();
    SaveManager savemgr;
    savemgr.InitFileSystem();

	ChunkWorker chunkWorker;

	chunkWorker.AddHandler(WorkerItemType_PolyGen, (WorkerFuncObj){&PolyGen_GeneratePolygons, NULL, true});
	chunkWorker.AddHandler(WorkerItemType_BaseGen, (WorkerFuncObj){&SuperFlatGen_Generate, &flatGen, true});
	chunkWorker.AddHandler(WorkerItemType_BaseGen, (WorkerFuncObj){&SmeaGen_Generate, &smeaGen, true});

	sino_init();

	World* world = (World*)malloc(sizeof(World));

	//Sound BackgroundSound;
	//Sound PlayerSound;
	Player player(world);
	PlayerController playerCtrl(&player);

	SuperFlatGen_Init(&flatGen, world);
	SmeaGen_Init(&smeaGen, world);

    Renderer renderer(world, &player, &chunkWorker.GetQueue(), &gamestate);

    DebugUI debugUI;

	WorldSelect_Init();

	World_Init(world, &chunkWorker.GetQueue());

    MCBridge mcBridge;

    savemgr.Init(&player);
    chunkWorker.AddHandler(WorkerItemType_Load, (WorkerFuncObj){SaveManager::LoadChunkCallback, &savemgr, true});
    chunkWorker.AddHandler(WorkerItemType_Save, (WorkerFuncObj){SaveManager::SaveChunkCallback, &savemgr, true});






    /**
    * RUN/GAMELOOP CODE
    */

	uint64_t lastTime = svcGetSystemTick();
	float dt = 0.f, timeAccum = 0.f, fpsClock = 0.f;
	int frameCounter = 0, fps = 0;
	//bool initBackgroundSound = false;
	while (aptMainLoop())
	{
		//if (initBackgroundSound)
		//{
		//	initBackgroundSound = false;
		//	BackgroundSound.background = true;
		//	char *soundfile = "romfs:/assets/sound/music/1.opus";
		//	BackgroundSound.path[0] = '\0';
		//	strncat(BackgroundSound.path, soundfile, sizeof(BackgroundSound.path) - 1);
		//	playopus(&BackgroundSound);
		//}

		debugUI.Text("%d FPS  Usage: CPU: %5.2f%% GPU: %5.2f%% Buf: %5.2f%% Lin: %d", fps, C3D_GetProcessingTime() * 6.f,
		C3D_GetDrawingTime() * 6.f, C3D_GetCmdBufUsage() * 100.f, linearSpaceFree());
        debugUI.Text("X: %f, Y: %f, Z: %f", f3_unpack(player.position));
        debugUI.Text("HP: %i",player.hp);
        debugUI.Text("velocity: %f rndy: %f",player.velocity.y,player.rndy);
		//debugUI.Text("Time: %i Cause: %c",dmg.time,dmg.cause);
        debugUI.Text("SX: %f SY: %f SZ: %f",player.spawnx,player.spawny,player.spawnz);
        debugUI.Text("Hunger: %i Hungertimer: %i",player.hunger,player.hungertimer);
        debugUI.Text("Gamemode: %i",player.gamemode);
        debugUI.Text("quickbar %i",player.quickSelectBarSlot);

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
		u32 keysheld = hidKeysHeld(), keysdown = hidKeysDown();
		if (keysdown & KEY_START) {
			if (gamestate == GameState_SelectWorld)
				break;
			else if (gamestate == GameState_Playing) {
				ReleaseWorld(&chunkWorker, &savemgr, world);

				gamestate = GameState_SelectWorld;

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

		InputData inputData = (InputData){keysheld,    keysdown,    hidKeysUp(),  circlePos.dx, circlePos.dy,
						  touchPos.px, touchPos.py, cstickPos.dx, cstickPos.dy};

		if (gamestate == GameState_Playing) {
			while (timeAccum >= 1.f / 20.f) {
				World_Tick(world);

				timeAccum -= 1.f / 20.f;
			}

            playerCtrl.Update(&debugUI, /*&PlayerSound,*/ inputData, dt);

			World_UpdateChunkCache(world, WorldToChunkCoord(FastFloor(player.position.x)),
					       WorldToChunkCoord(FastFloor(player.position.z)));
		}else if(gamestate == GameState_Playing_OnLine){

            //ONLINE LOGIC HERE
            s32 dimension;

            if(mcBridge.isConnected()){
                mcBridge.withClient([&](mc::core::Client* client, mc::protocol::packets::PacketDispatcher* dispatcher) {

                    mc::Vector3d playerPos = client->GetPlayerController()->GetPosition();

                    player.position.x = playerPos.x;
                    player.position.y = playerPos.y;
                    player.position.z = playerPos.z;

                    dimension = client->GetConnection()->GetDimension();
                    playerCtrl.Update(&debugUI, /*&PlayerSound,*/ inputData, dt);

                    //TODO: This is baaaad :D
                    client->GetPlayerController()->Move(mc::Vector3d(
                            -playerCtrl.movement.x*(0.125/2),
                            -playerCtrl.movement.y*(0.125/2),
                            -playerCtrl.movement.z*(0.125/2)));

                    client->GetPlayerController()->SetPitch(-playerCtrl.player->pitch);
                    client->GetPlayerController()->SetYaw(-playerCtrl.player->yaw);

                    // TODO: packet spam prevention... This is enough for now
                    for (int i = 0; i < 8; i++) {



                        auto item = mcBridge.MCLIBSlotToCTItemStack(client->GetHotbar().GetItem(i));
                        player.quickSelectBar[i] = item;
                        debugUI.Text("%d ",item.block);
                    }
                });
            }

            debugUI.Text("%d ",dimension);




        } else if (gamestate == GameState_SelectWorld) {
			char path[256];
			char name[WORLD_NAME_SIZE] = {'\0'};
			WorldGenType worldType;
			bool newWorld = false;
			bool isMultiplayer = false;
			if (WorldSelect_Update(path, name, &worldType, &newWorld, &isMultiplayer)) {
                if(!isMultiplayer){
                    strcpy(world->name, name);
                    world->genSettings.type = worldType;

                    savemgr.Load(path);

                    chunkWorker.SetHandlerActive(WorkerItemType_BaseGen, &flatGen,
                                     world->genSettings.type == WorldGen_SuperFlat);
                    chunkWorker.SetHandlerActive(WorkerItemType_BaseGen, &smeaGen,
                                     world->genSettings.type == WorldGen_Smea);

                    world->cacheTranslationX = WorldToChunkCoord(FastFloor(player.position.x));
                    world->cacheTranslationZ = WorldToChunkCoord(FastFloor(player.position.z));
                    for (int i = 0; i < CHUNKCACHE_SIZE; i++) {
                        for (int j = 0; j < CHUNKCACHE_SIZE; j++) {
                            world->chunkCache[i][j] =
                                World_LoadChunk(world, i - CHUNKCACHE_SIZE / 2 + world->cacheTranslationX,
                                        j - CHUNKCACHE_SIZE / 2 + world->cacheTranslationZ);
                        }
                    }

                    for (int i = 0; i < 3; i++) {
                        while (chunkWorker.IsWorking() || chunkWorker.GetQueue().queue.length > 0) {
                            svcSleepThread(50000000);  // 1 Tick
                        }
                        World_Tick(world);
                    }

                    if (newWorld) {
                        int highestblock = 0;
                        for (int x = -1; x < 1; x++) {
                            for (int z = -1; z < 1; z++) {
                                int height = World_GetHeight(world, x, z);
                                if (height > highestblock) highestblock = height;
                            }
                        }
                        player.hunger=20;
                        player.hp=20;
                        player.position.y = (float)highestblock + 0.2f;
                    }
                    gamestate = GameState_Playing;
                    lastTime = svcGetSystemTick();  // fix timing
                } else {

                    mcBridge.connect();
                    mcBridge.startBackgroundThread();
                    gamestate = GameState_Playing_OnLine;
                }
			}
		}
		Gui_InputData(inputData);
	}









    /**
     * EXIT/CLEANUP CODE
     */

	if (gamestate == GameState_Playing)
	{
		ReleaseWorld(&chunkWorker, &savemgr, world);
	}

    if (gamestate == GameState_Playing_OnLine)
	{
        mcBridge.stopBackgroundThread();
        mcBridge.disconnect();
	}

	savemgr.~SaveManager();

	SuperChunk_DeinitPools();

	free(world);
/*
	if (BackgroundSound.threaid != NULL)
	{
		DoQuit(0);
		threadJoin(BackgroundSound.threaid, 50000);
		threadFree(BackgroundSound.threaid);
		if (BackgroundSound.opusFile)
		{
			op_free(BackgroundSound.opusFile);
		}
		audioExit(0);
	}
	if (PlayerSound.threaid != NULL)
	{
		DoQuit(1);
		threadJoin(PlayerSound.threaid, 50000);
		threadFree(PlayerSound.threaid);
		if (PlayerSound.opusFile)
		{
			op_free(PlayerSound.opusFile);
		}
		audioExit(1);
	}
*/
	ndspExit();
	sino_exit();

	WorldSelect_Deinit();

    debugUI.~DebugUI();

	chunkWorker.~ChunkWorker();

    renderer.~Renderer();

	romfsExit();

	gfxExit();
}
