#include "rendering/Renderer.hpp"

#include "GameStates.hpp"
#include "ReCraftCore.hpp"
#include "blocks/CT_Block.hpp"
#include "entity/Player.hpp"
#include "gui/CT_Inventory.hpp"
#include "gui/DebugUI.hpp"
#include "gui/Gui.hpp"
#include "gui/ImGuiManager.hpp"
#include "gui/Screen.hpp"
#include "gui/SpriteBatch.hpp"
#include "rendering/Camera.hpp"
#include "rendering/Clouds.hpp"
#include "rendering/Cursor.hpp"
#include "rendering/PolyGen.hpp"
#include "rendering/TextureMap.hpp"
#include "rendering/WorldRenderer.hpp"
#include "world/CT_World.hpp"
#include "world/WorkQueue.hpp"

#include <fstream>
#include <functional>
#include <iostream>

// TODO: Fix this
extern bool showDebugInfo;

Renderer::Renderer(World* world, Player* player, WorkQueue* queue) {
    m_workqueue = queue;
    C3D::Init();

    Top[0] = C3D::CreateScreen(GFX_TOP);
    Top[1] = C3D::CreateScreen(GFX_TOP, GFX_RIGHT);
    Bottom = C3D::CreateScreen(GFX_BOTTOM);

    pWorldShader = C3D::Shader::New();
    pWorldShader->Load("romfs:/shaders/world.shbin");
    m_world_shader_uLocProjection = pWorldShader->loc("projection");

    pGuiShader = C3D::Shader::New();
    pGuiShader->Load("romfs:/shaders/gui.shbin");
    m_gui_shader_uLocProjection = pGuiShader->loc("projection");

    pWorldShader->Input(GPU_SHORT, 3);
    pWorldShader->Input(GPU_SHORT, 2);
    pWorldShader->Input(GPU_UNSIGNED_BYTE, 3);
    pWorldShader->Input(GPU_UNSIGNED_BYTE, 3);

    pGuiShader->Input(GPU_SHORT, 3);
    pGuiShader->Input(GPU_SHORT, 3);

    PolyGen_Init(world, player);

    m_worldRenderer = new WorldRenderer(player, world, m_workqueue, m_world_shader_uLocProjection);

    Iron::Init();
    SpriteBatch_Init(m_gui_shader_uLocProjection);
    Gui::Init();
    C3D_CullFace(GPU_CULL_BACK_CCW);

    Block_Init();

    ImGuiManager::GetInstance()->Initialize();
}

Renderer::~Renderer() {
    Block_Deinit();
    PolyGen_Deinit();
    // delete m_worldRenderer;
    m_worldRenderer = nullptr;
    Gui::Deinit();
    SpriteBatch_Deinit();

    delete Top[0];
    delete Top[1];
    delete Bottom;
    C3D::Deinit();
}

void Renderer::Render(DebugUI* debugUi) {
    float iod = osGet3DSliderState() * Player::HalfEyeDiff;

    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

    if (*ReCraftCore::GetInstance()->GetGameState() != GameState::SelectWorld) {
        PolyGen_Harvest(debugUi);
    }

    for (int i = 0; i < 2; i++) {
        RenderFrame(i, iod);
        if (iod <= 0.f) {
            break;
        }
    }

    RenderLowerScreen(debugUi);

    ImGuiIO& io                = ImGui::GetIO();
    io.DisplaySize             = ImVec2(ScreenWidth, ScreenHeight);
    io.DisplayFramebufferScale = ImVec2(FrameBufferScale, FrameBufferScale);

    ImGuiManager::GetInstance()->BeginFrame();

    ImGuiManager::GetInstance()->ExecuteCallbacks();

    Iron::NewFrame();

    Top[0]->Use();
    Iron::DrawOn(Top[0]);
    debugUi->RenderData->DrawRectFilled(0, 50, 0xff00ff00);
    Iron::Draw(debugUi->RenderData->Data());
    debugUi->RenderData->Clear();

    // C3D_FrameDrawOn(m_lowerScreen);
    // Iron::DrawOn(Bottom);
    // Iron::Draw(*Gui::RenderData);
    // Gui::RenderData->Clear();

    ImGuiManager::GetInstance()->EndFrame(reinterpret_cast<void*>(Top[0]->Ptr()),
                                          reinterpret_cast<void*>(Bottom->Ptr()));

    C3D_FrameEnd(0);
}

void Renderer::RenderFrame(int eyeIndex, float iod) {
    Top[eyeIndex]->Clear(SkyClearColor);
    C3D_FrameDrawOn(Top[eyeIndex]->Ptr());

    SpriteBatch_StartFrame(400, 240);

    C3D_TexEnv* env = C3D_GetTexEnv(0);
    C3D_TexEnvInit(env);
    C3D_TexEnvSrc(env, C3D_Both, GPU_TEXTURE0, GPU_PRIMARY_COLOR, (GPU_TEVSRC)0);
    C3D_TexEnvFunc(env, C3D_Both, GPU_MODULATE);

    pWorldShader->Use();

    C3D_DepthTest(true, GPU_GREATER, GPU_WRITE_ALL);
    C3D_CullFace(GPU_CULL_BACK_CCW);

    if (ReCraftCore::GetInstance()->m_pTopScreen){
        ReCraftCore::GetInstance()->m_pTopScreen->Render3D(0, 0, eyeIndex, m_world_shader_uLocProjection, iod, 0 /*  TODO: 0 is enough for now, but later we need   1000.f / Amy::App::Delta()*/);
        ReCraftCore::GetInstance()->m_pTopScreen->Render(0, 0, 0 /*  TODO: 0 is enough for now, but later we need   1000.f / Amy::App::Delta()*/);
    }

    // TODO: REMOVE
    if (*ReCraftCore::GetInstance()->GetGameState() != GameState::SelectWorld) {
        C3D_TexBind(0, (C3D_Tex*)Block_GetTextureMap());

        m_worldRenderer->Render(!eyeIndex ? -iod : iod);
    }

    pGuiShader->Use();

    SpriteBatch_Render(GFX_TOP);
}

void Renderer::RenderLowerScreen(DebugUI* debugUi) {
    Bottom->Clear(BlackClearColor);
    Bottom->Use();

    SpriteBatch_StartFrame(320, 240);


    if (ReCraftCore::GetInstance()->m_pBotScreen){
        Gui::GetCursorMovement(&m_mouseX, &m_mouseY);
        ReCraftCore::GetInstance()->m_pBotScreen->Render(
            m_mouseX, m_mouseY, 0 /*  TODO: 0 is enough for now, but later we need   1000.f / Amy::App::Delta()*/);
    }

    if (showDebugInfo) {
        debugUi->Draw();
    }

    Gui::Frame();

    SpriteBatch_Render(GFX_BOTTOM);
}
