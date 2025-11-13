#pragma once

#include "../entity/Player.h"
#include "../world/WorkQueue.h"
#include "../world/CT_World.h"
#include "../GameStates.h"
#include "../gui/DebugUI.h"
#include "WorldRenderer.h"
#include "../ReCraftCore.h"
#include <citro3d.h>

//TODO: Organize
#include <3ds.h>
#include <citro3d.h>
#include <tex3ds.h>

#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <vector>

// Enable Npi Assert to write Errors to file
#define IMGUI_IMPL_CITRO3D_USE_NPI_ASSERT
#define IMGUI_IMPL_CIR_USE_NPI_ASSERT

#include "../../imgui/imgui.h"
#include "../../imgui-impl/imgui_impl_citro3d.h"
#include "../../imgui-impl/imgui_impl_ctr.h"

struct NpiEasyTex {
    C3D_Tex* tex = NULL;
    Tex3DS_Texture t3x;
};

class ReCraftCore;

class Renderer {
public:
    Renderer(World* world_, Player* player_, WorkQueue* queue);
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
    WorldRenderer* worldRenderer;
    WorkQueue* workqueue;

    ImGuiIO* io;
    NpiEasyTex ntex;
    bool show_demo_window;

    void RenderFrame(int eyeIndex, float iod);
    void RenderLowerScreen(DebugUI* debugUi);
};