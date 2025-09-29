#include <3ds.h>
#include <iostream>

#include "mclib/common/Common.h"
#include "mclib/core/Client.h"
#include "mclib/util/Forge.h"
#include "mclib/util/Hash.h"
#include "mclib/util/Utility.h"
#include "mclib/util/VersionFetcher.h"

//#include "gui/ServerList/ServerList.hpp"

//#include "gui/Screen.hpp"

namespace {

    const std::string server("192.168.2.101");
    const u16 port = 25565;
    const std::string username("Nintendo3DS");
    const std::string password("");
    const bool useProfileToken = false;

} // ns


int run(mc::protocol::Version version, mc::util::ForgeHandler& forge) {
    mc::protocol::packets::PacketDispatcher dispatcher;
    mc::core::Client client(&dispatcher, version);

    forge.SetConnection(client.GetConnection());

    client.GetPlayerController()->SetHandleFall(true);
    client.GetConnection()->GetSettings()
            .SetMainHand(mc::MainHand::Right)
            .SetViewDistance(static_cast<s32>(16));


    //example::Logger logger(&client, &dispatcher);

    try {
        mc::core::AuthToken token;

        client.Login(server, port, username, password, mc::core::UpdateMethod::Block);

    } catch (std::exception& e) {
        std::wcout << e.what() << std::endl;
        return 1;
    }

    return 0;
}

int main() {
    gfxInitDefault();
    consoleInit(GFX_BOTTOM, nullptr);
    std::atexit(gfxExit);

    mc::util::VersionFetcher versionFetcher(server, port);

    auto version = versionFetcher.GetVersion();
    mc::block::BlockRegistry::GetInstance()->RegisterVanillaBlocks(version);

    std::cout << "You connected successfully\n";
    int err = run(version, versionFetcher.GetForge());


    while (aptMainLoop()) {
        hidScanInput();
        if (hidKeysDown() & KEY_SELECT) break;
        gspWaitForVBlank();
    }

    return 0;
}
