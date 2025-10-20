#pragma once

#include "../entity/Player.h"
#include "../world/WorkQueue.h"
#include "../world/CT_World.h"
#include "../GameStates.h"
#include "../gui/DebugUI.h"

#include <citro3d.h>

class Renderer {
public:
    Renderer(World* world_, Player* player_, WorkQueue* queue, GameState* gamestate_);
    ~Renderer();

    void Render(DebugUI* debugUi);

private:
    C3D_RenderTarget* renderTargets[2];
    C3D_RenderTarget* lowerScreen;

    DVLB_s *world_dvlb, *gui_dvlb;
    shaderProgram_s world_shader, gui_shader;
    int world_shader_uLocProjection, gui_shader_uLocProjection;

    C3D_AttrInfo world_vertexAttribs, gui_vertexAttribs;

    C3D_Tex logoTex;

    World* world;
    Player* player;
    WorkQueue* workqueue;

    GameState* gamestate;

    void RenderFrame(int eyeIndex, float iod);
    void RenderLowerScreen(DebugUI* debugUi);
};