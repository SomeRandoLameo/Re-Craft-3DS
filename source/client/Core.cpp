//
// Created by Elias on 01.10.2025.
//

#include "Core.h"
#include "gui/ScreenManager.h"
#include "gui/screens/GuiInGame.h"
#include <citro3d.h>
#include <iostream>
#include <3ds.h>


#define DISPLAY_TRANSFER_FLAGS \
	(GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) | \
	GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) | \
	GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO))

namespace {

    const std::string server("192.168.2.101");
    const u16 port = 25565;
    const std::string username("Nintendo3DS");
    const std::string password("");
    const bool useProfileToken = false;

} // ns

Core* Core::coreInstance = nullptr;

Core::Core(){
    printf("core started\n");
    gfxInitDefault();

    this->coreInstance = this;
}

//TODO: THIS IS ONLY PROOF OF CONCEPT AND WILL BE REWRITTEN
void updatePlayerMovement(mc::core::Client* client) {
    hidScanInput();
    u32 kDown = hidKeysHeld();

    mc::Vector3d direction;

    if (kDown & KEY_DUP) {

        mc::Vector3d forwards = mc::Vector3Normalize(mc::Vector3d(1, 0, 0));

        direction += forwards;

        // if (m_Window.IsKeyDown(GLFW_KEY_LEFT_CONTROL)) {
        //     m_Sprinting = true;
        // }
    }

    if (kDown & KEY_DDOWN) {
        mc::Vector3d backwards = mc::Vector3Normalize(mc::Vector3d(1, 0, 0));

        direction -= backwards;
        // m_Sprinting = false;
    }

    if (kDown & KEY_DLEFT) {
        mc::Vector3d right = mc::Vector3Normalize(mc::Vector3d(0, 0, 1));

        direction -= right;
    }

    if (kDown & KEY_DRIGHT) {
        mc::Vector3d right = mc::Vector3Normalize(mc::Vector3d(0, 0, 1));

        direction += right;
    }

    // if (!m_Player->OnGround()) {
    //     direction *= 0.2;
    // }


    if (direction.x != 0.0 || direction.y != 0.0 || direction.z != 0.0) {
        client->GetPlayerController()->Move(direction);
    }
}



//TODO: Multithreading for network and rendering
void Core::run() {
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    //consoleInit(GFX_BOTTOM, nullptr);

    // Initialize the render targets
    C3D_RenderTarget* topTarget = C3D_RenderTargetCreate(240, 400, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
    C3D_RenderTargetSetOutput(topTarget, GFX_TOP, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);

    C3D_RenderTarget* bottomTarget = C3D_RenderTargetCreate(240, 320, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
    C3D_RenderTargetSetOutput(bottomTarget, GFX_BOTTOM, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);

    // Initialize the scene


    mc::util::VersionFetcher versionFetcher(server, port);
    auto version = versionFetcher.GetVersion();
    mc::block::BlockRegistry::GetInstance()->RegisterVanillaBlocks(version);

    std::cout << "You connected successfully\n";

    mc::protocol::packets::PacketDispatcher dispatcher;
    mc::core::Client client(&dispatcher, version);

    versionFetcher.GetForge().SetConnection(client.GetConnection());

    client.GetPlayerController()->SetHandleFall(true);
    client.GetConnection()->GetSettings()
            .SetMainHand(mc::MainHand::Right)
            .SetViewDistance(static_cast<s32>(4));

    try {
        mc::core::AuthToken token;
        client.Login(server, port, username, password, mc::core::UpdateMethod::Manual);
    } catch (std::exception& e) {
        std::wcout << e.what() << std::endl;
    }

    const double targetFPS = 60.0;
    const double fixedDeltaTime = 1.0 / targetFPS;
    u64 lastTime = svcGetSystemTick();
    double accumulator = 0.0;
    float ticks = 0.0f;
    const u64 ticksPerSecond = SYSCLOCK_ARM11;

    ScreenManager screenManager;
    screenManager.init();

    screenManager.displayScreen(new GuiInGame(), true);

    while (aptMainLoop()) {
        // Calculate delta time
        u64 currentTime = svcGetSystemTick();
        double frameTime = (double)(currentTime - lastTime) / (double)ticksPerSecond;
        lastTime = currentTime;

        accumulator += frameTime;

        hidScanInput();

        // Respond to user input
        u32 kDown = hidKeysDown();
        if (kDown & KEY_START)
            break;

        // Fixed update loop
        while (accumulator >= fixedDeltaTime) {
            C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

            updatePlayerMovement(&client);
            screenManager.update(ticks);
            client.Update();

            // Rendering für Top-Screen
            C3D_RenderTargetClear(topTarget, C3D_CLEAR_ALL, 0x000000FF, 0);
            C3D_FrameDrawOn(topTarget);
            if (screenManager.getCurrentTopScreen())
                screenManager.getCurrentTopScreen()->drawScreen(ticks);

            // Rendering für Bottom-Screen (optional, falls genutzt)
            C3D_RenderTargetClear(bottomTarget, C3D_CLEAR_ALL, 0x000000FF, 0);
            C3D_FrameDrawOn(bottomTarget);
            if (screenManager.getCurrentBottomScreen())
                screenManager.getCurrentBottomScreen()->drawScreen(ticks);

            C3D_FrameEnd(0);

            accumulator -= fixedDeltaTime;
            ticks += 1.0f;  // Increment ticks each fixed update
        }

        gspWaitForVBlank();
    }

    // Deinitialize graphics
    C3D_Fini();
    gfxExit();
}

void Core::startGame() {
}

