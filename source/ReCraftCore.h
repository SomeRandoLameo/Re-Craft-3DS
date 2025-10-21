#pragma once

#include <3ds.h>
#include <citro3d.h>
extern "C" {
#include <sino/sino.h>
}
#include "GameStates.h"
#include "entity/Player.h"
#include "entity/PlayerController.h"
#include "gui/DebugUI.h"
#include "gui/Gui.h"
#include "gui/WorldSelect.h"
#include "rendering/PolyGen.h"
#include "rendering/Renderer.h"
#include "world/ChunkWorker.h"
#include "world/CT_World.h"
#include "world/savegame/SaveManager.h"
#include "world/savegame/SuperChunk.h"
#include "world/worldgen/SmeaGen.h"
#include "world/worldgen/SuperFlatGen.h"
#include "misc/Crash.h"

class ReCraftCore {
public:
    ReCraftCore();
    void Run();
private:
    static ReCraftCore* theReCraftCore;
    void ReleaseWorld(ChunkWorker* chunkWorker, SaveManager* savemgr, World* world);
};
