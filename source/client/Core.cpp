//
// Created by Elias on 01.10.2025.
//

#include "Core.h"
#include <iostream>
#include <3ds.h>


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


void Core::run() {

    consoleInit(GFX_BOTTOM, nullptr);


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
    const u64 ticksPerSecond = SYSCLOCK_ARM11;


    while (aptMainLoop()) {
        // Calculate delta time
        u64 currentTime = svcGetSystemTick();
        double frameTime = (double)(currentTime - lastTime) / (double)ticksPerSecond;
        lastTime = currentTime;

        accumulator += frameTime;

        hidScanInput();
        if (hidKeysDown() & KEY_SELECT) goto exit_loop;

        // Fixed update loop
        while (accumulator >= fixedDeltaTime) {
            updatePlayerMovement(&client);
            client.Update();
            accumulator -= fixedDeltaTime;
        }

        gspWaitForVBlank();
    }
    exit_loop:;

}

void Core::startGame() {
}

