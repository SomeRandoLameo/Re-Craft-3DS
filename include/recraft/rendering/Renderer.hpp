#pragma once

#include "../GameStates.hpp"
#include "../entity/Player.hpp"
#include "../gui/DebugUI.hpp"
#include "../world/CT_World.hpp"
#include "../world/WorkQueue.hpp"
#include "WorldRenderer.hpp"
#include "gui/ImGuiManager.hpp"

#include <citro3d.h>

// TODO: Organize
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

#include "imgui.h"
#include "imgui_impl_citro3d.h"
#include "imgui_impl_ctr.h"

class ReCraftCore;

class Renderer {
public:
    enum {
        ScreenWidth = 400,
        ScreenHeight = 480,
        FrameBufferScale = 1,
        SkyClearColor = 0x90d9ffff,
        BlackClearColor = 0x000000ff
    };
    Renderer(World* world_, Player* player_, WorkQueue* queue);
    ~Renderer();

    void Render(DebugUI* debugUi);

    C3D_RenderTarget* GetTopTarget() const { return Top[0]->Ptr(); }
    C3D_RenderTarget* GetBottomTarget() const { return Bottom->Ptr(); }

private:
    Clouds* m_clouds = nullptr;
    std::array<C3D::Screen*, 2> Top = {nullptr, nullptr};
    C3D::Screen* Bottom = nullptr;

    Amy::C3D::Shader::Ref pWorldShader = nullptr;
    Amy::C3D::Shader::Ref pGuiShader = nullptr;

    int m_world_shader_uLocProjection = 0;
    int m_gui_shader_uLocProjection = 0;

    C3D_Tex m_logoTex;

    World* m_world;

    Player* m_player;
    WorldRenderer* m_worldRenderer = nullptr;
    WorkQueue* m_workqueue;
    Inventory* m_inventory;
    ImGuiIO* m_io;
    bool m_show_demo_window;
    std::string m_cstyle;
    std::vector<std::string> m_styles;

    void RenderFrame(int eyeIndex, float iod);
    void RenderLowerScreen(DebugUI* debugUi);

    void RenderExpBar();
    void RenderHealth();
    void RenderGameOverlay();
    void RenderHunger();
};
