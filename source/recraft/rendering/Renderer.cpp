#include "rendering/Renderer.h"

#include "ReCraftCore.h"
#include "blocks/CT_Block.h"
#include "gui/CT_Inventory.h"
#include "gui/Gui.h"
#include "gui/SpriteBatch.h"
#include "gui/WorldSelect.h"
#include "rendering/Camera.h"
#include "rendering/Clouds.h"
#include "rendering/Cursor.h"
#include "rendering/PolyGen.h"
#include "rendering/TextureMap.h"

#include <gui_shbin.h>
#include <world_shbin.h>

const auto SCREEN_WIDTH = 400.0f;
const auto SCREEN_HEIGHT = 480.0f;
const auto FB_SCALE = 1.0f;

C3D::Screen *Top[2];
C3D::Screen *Bottom;

#define CLEAR_COLOR_SKY 0x90d9ffff
#define CLEAR_COLOR_BLACK 0x000000ff

// TODO: Fix this
extern bool showDebugInfo;

Renderer::Renderer(World *world, Player *player, WorkQueue *queue) {
    // i actually prefer this
    m_world = world;
    m_player = player;
    m_workqueue = queue;

    m_clouds = new Clouds();
    C3D::Init();

    Top[0] = C3D::CreateScreen(GFX_TOP);
    Top[1] = C3D::CreateScreen(GFX_TOP, GFX_RIGHT);
    Bottom = C3D::CreateScreen(GFX_BOTTOM);
    m_renderTargets[0] = Top[0]->Ptr();
    m_renderTargets[1] = Top[1]->Ptr();
    m_lowerScreen = Bottom->Ptr();

    m_world_dvlb = DVLB_ParseFile((u32 *)world_shbin, world_shbin_size);
    shaderProgramInit(&m_world_shader);
    shaderProgramSetVsh(&m_world_shader, &m_world_dvlb->DVLE[0]);
    m_world_shader_uLocProjection = shaderInstanceGetUniformLocation(m_world_shader.vertexShader, "projection");

    m_gui_dvlb = DVLB_ParseFile((u32 *)gui_shbin, gui_shbin_size);
    shaderProgramInit(&m_gui_shader);
    shaderProgramSetVsh(&m_gui_shader, &m_gui_dvlb->DVLE[0]);
    m_gui_shader_uLocProjection = shaderInstanceGetUniformLocation(m_gui_shader.vertexShader, "projection");

    AttrInfo_Init(&m_world_vertexAttribs);
    AttrInfo_AddLoader(&m_world_vertexAttribs, 0, GPU_SHORT, 3);
    AttrInfo_AddLoader(&m_world_vertexAttribs, 1, GPU_SHORT, 2);
    AttrInfo_AddLoader(&m_world_vertexAttribs, 2, GPU_UNSIGNED_BYTE, 3);
    AttrInfo_AddLoader(&m_world_vertexAttribs, 3, GPU_UNSIGNED_BYTE, 3);

    AttrInfo_Init(&m_gui_vertexAttribs);
    AttrInfo_AddLoader(&m_gui_vertexAttribs, 0, GPU_SHORT, 3);
    AttrInfo_AddLoader(&m_gui_vertexAttribs, 1, GPU_SHORT, 3);

    PolyGen_Init(world, player);

    m_worldRenderer = new WorldRenderer(player, world, m_workqueue, m_world_shader_uLocProjection);

    Iron::Init();
    SpriteBatch_Init(m_gui_shader_uLocProjection);
    Gui::Init();
    C3D_CullFace(GPU_CULL_BACK_CCW);
    Block_Init();
    Texture_Load(&m_logoTex, "romfs:/assets/textures/gui/title/craftus.png");
    ImGuiManager::GetInstance()->Initialize();
    m_show_demo_window = false;
}

Renderer::~Renderer() {
    C3D_TexDelete(&m_logoTex);
    Block_Deinit();
    PolyGen_Deinit();
    m_clouds->~Clouds();
    delete m_worldRenderer;
    m_worldRenderer = nullptr;
    Gui::Deinit();
    SpriteBatch_Deinit();
    shaderProgramFree(&m_gui_shader);

    if (m_gui_dvlb) {
        DVLB_Free(m_gui_dvlb);
    }

    shaderProgramFree(&m_world_shader);

    if (m_world_dvlb) {
        DVLB_Free(m_world_dvlb);
    }

    delete Top[0];
    delete Top[1];
    delete Bottom;
    C3D::Deinit();
}

// this is actual minecraft ported code
// TODO: Add support for damage animations, potion effects like poisin, wither
// and more
// TODO: Move this somewhere else once the new renderer works, Preferrably as a GuiOverlay
void Renderer::RenderHealth() {
    //TODO: Damage flicker bounce + healing white flicker + wither and more
    int health = m_player->hp;
    int yPos = 99;
    int spriteSize = 9;
    SpriteBatch_BindGuiTexture(GuiTexture_Icons);
    for (int amount = 0; amount < 10; ++amount) {

        int var6 = 0;
        bool var9 = false;
        if (var9) {
            var6 = 1;
        }

        int prevHealth = m_player->hp;

        if (health <= 4) {
            yPos += nextafter(2, 0);
        }

        SpriteBatch_PushQuad(spriteSize + (amount * 8), yPos, -1, spriteSize,
            spriteSize, 16 + var6 * spriteSize, 0, spriteSize,
            spriteSize
        );

        if (var9) {
            if ((amount << 1) + 1 < prevHealth) {
                SpriteBatch_PushQuad(spriteSize + (amount * 8), yPos, 0, spriteSize,
                    spriteSize, 70, 0, spriteSize, spriteSize
                );
            }

            if ((amount << 1) + 1 == prevHealth) {
                SpriteBatch_PushQuad(spriteSize + (amount * 8), yPos, 0, spriteSize,
                    spriteSize, 79, 0, spriteSize, spriteSize
                );
            }
        }

        if ((amount << 1) + 1 < health) {
            SpriteBatch_PushQuad(spriteSize + (amount * 8), yPos, 0, spriteSize,
                spriteSize, 52, 0, spriteSize, spriteSize
            );
        }

        if ((amount << 1) + 1 == health) {
            SpriteBatch_PushQuad(spriteSize + (amount * 8), yPos, 0, spriteSize,
                spriteSize, 61, 0, spriteSize, spriteSize
            );
        }
    }
}

void Renderer::RenderExpBar() {
    // harcoded cap for now
    int barCap = 10;

    SpriteBatch_BindGuiTexture(GuiTexture_Icons);

    if (barCap > 0) {
        int barLength = 182;
        int xpFill = (int)(m_player->experience * (float)(barLength + 1));

        int y = 120 - 9;
        SpriteBatch_PushQuad(200 / 2 - 182 / 2, y, 0, barLength, 5, 0, 64,
                         barLength, 5);

        if (xpFill > 0) {
            SpriteBatch_PushQuad(200 / 2 - 182 / 2, y, 1, xpFill, 5, 0, 69, xpFill,
                           5);
        }
    }

    if (m_player->experienceLevel > 0) {
        char experienceStr[20]; // buffer to hold the string representation of
                            // experience level

        int experienceInt = (int)m_player->experienceLevel;
        snprintf(experienceStr, sizeof(experienceStr), "%d",
             experienceInt); // Format as integer

        int textWidth = SpriteBatch_CalcTextWidth(experienceStr);

        int textY = 10;

        SpriteBatch_PushText(200 / 2 - textWidth / 2 + 1, 120 - textY, 2,
                         SHADER_RGB(0, 0, 0), false, INT_MAX, 0, experienceStr);
        SpriteBatch_PushText(200 / 2 - textWidth / 2 - 1, 120 - textY, 2,
                         SHADER_RGB(0, 0, 0), false, INT_MAX, 0, experienceStr);
        SpriteBatch_PushText(200 / 2 - textWidth / 2, 120 - textY + 1, 2,
                         SHADER_RGB(0, 0, 0), false, INT_MAX, 0, experienceStr);
        SpriteBatch_PushText(200 / 2 - textWidth / 2, 120 - textY - 1, 2,
                         SHADER_RGB(0, 0, 0), false, INT_MAX, 0, experienceStr);
        SpriteBatch_PushText(200 / 2 - textWidth / 2, 120 - textY, 3,
                         SHADER_RGB(100, 255, 32), false, INT_MAX, 0,
                         experienceStr);
    }
}

// TODO: Hunger effects
void Renderer::RenderHunger() {
    int spriteSize = 9;
    int xpos = 190;
    int ypos = 99;

    int saturationLevel = m_player->hunger;

    for (int amount = 0; amount < 10; ++amount) {

        int l6 = 16;
        int j7 = 0;
    /*
            //whatever this is
            if (entityplayer.isPotionActive(MobEffects.HUNGER))
            {
                l6 += 36;
                j7 = 13;
            }

            //Shaking effect
            if (entityplayer.getFoodStats().getSaturationLevel() <= 0.0F &&
       this.updateCounter % (k * 3 + 1) == 0)
            {
                ypos = j1 + (this.rand.nextInt(3) - 1);
            }
    */

        int spriteXpos = xpos - amount * 8 - 9;
        SpriteBatch_PushQuad(spriteXpos, ypos, -1, spriteSize, spriteSize,
                         16 + j7 * 9, 27, 9, 9);

        if (amount * 2 + 1 < saturationLevel) {
            SpriteBatch_PushQuad(spriteXpos, ypos, 0, spriteSize, spriteSize, l6 + 36,
                           27, 9, 9);
        }

        if (amount * 2 + 1 == saturationLevel) {
            SpriteBatch_PushQuad(spriteXpos, ypos, 0, spriteSize, spriteSize, l6 + 45,
                           27, 9, 9);
        }
    }
}

void Renderer::RenderGameOverlay() {
    SpriteBatch_BindGuiTexture(GuiTexture_Icons);
    SpriteBatch_PushQuad(200 / 2 - 16 / 2, 120 / 2 - 16 / 2, 0, 16, 16, 0, 0, 16,
                       16);

    if (m_player->gamemode == 0) {
        RenderExpBar();
        RenderHealth();
        RenderHunger();
    }
}

void Renderer::Render(DebugUI *debugUi) {
    float iod = osGet3DSliderState() * PLAYER_HALFEYEDIFF;

    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

    if (*ReCraftCore::GetInstance()->GetGameState() == (GameState_Playing || GameState_Playing_OnLine)) {
        PolyGen_Harvest(debugUi);
    }

    for (int i = 0; i < 2; i++) {
        RenderFrame(i, iod);
        if (iod <= 0.f) break;
    }

    RenderLowerScreen(debugUi);

    ImGuiIO &io = ImGui::GetIO();
    io.DisplaySize = ImVec2(SCREEN_WIDTH, SCREEN_HEIGHT);
    io.DisplayFramebufferScale = ImVec2(FB_SCALE, FB_SCALE);

    ImGuiManager::GetInstance()->BeginFrame();

    ImGuiManager::GetInstance()->ExecuteCallbacks();

    C3D_FrameDrawOn(m_renderTargets[0]);
    Iron::NewFrame();
    Iron::DrawOn(Top[0]);
    Iron::Draw(*debugUi->RenderData);
    debugUi->RenderData->Clear();
    C3D_FrameDrawOn(m_lowerScreen);

    ImGuiManager::GetInstance()->EndFrame(
      reinterpret_cast<void *>(m_renderTargets[0]),
      reinterpret_cast<void *>(m_lowerScreen));

    C3D_DepthTest(true, GPU_GREATER, GPU_WRITE_ALL);
    C3D_CullFace(GPU_CULL_BACK_CCW);

    C3D_BindProgram(&m_world_shader);
    C3D_SetAttrInfo(&m_world_vertexAttribs);

    C3D_FrameEnd(0);
}

void Renderer::RenderFrame(int eyeIndex, float iod) {
    C3D_RenderTargetClear(m_renderTargets[eyeIndex], C3D_CLEAR_ALL, CLEAR_COLOR_SKY,
                        0);
    C3D_FrameDrawOn(m_renderTargets[eyeIndex]);

    SpriteBatch_StartFrame(400, 240);

    C3D_TexEnv *env = C3D_GetTexEnv(0);
    C3D_TexEnvInit(env);
    C3D_TexEnvSrc(env, C3D_Both, GPU_TEXTURE0, GPU_PRIMARY_COLOR, (GPU_TEVSRC)0);
    C3D_TexEnvFunc(env, C3D_Both, GPU_MODULATE);

    C3D_BindProgram(&m_world_shader);
    C3D_SetAttrInfo(&m_world_vertexAttribs);

    if (*ReCraftCore::GetInstance()->GetGameState() == GameState_Playing) {
        C3D_TexBind(0, (C3D_Tex *)Block_GetTextureMap());

        m_worldRenderer->Render(!eyeIndex ? -iod : iod);

        RenderGameOverlay();
    } else if (*ReCraftCore::GetInstance()->GetGameState() == GameState_Playing_OnLine) {
        C3D_TexBind(0, (C3D_Tex *)Block_GetTextureMap());

        // TODO: There needs to be a world to render :D
        m_worldRenderer->Render(!eyeIndex ? -iod : iod);

        RenderGameOverlay();
    } else {
        C3D_Mtx projection;
        Mtx_PerspStereoTilt(&projection, C3D_AngleFromDegrees(90.f),
                            ((400.f) / (240.f)), 0.22f, 4.f * CHUNK_SIZE,
                            !eyeIndex ? -iod : iod, 3.f, false);

        C3D_Mtx view;
        Mtx_Identity(&view);
        Mtx_Translate(&view, 0.f, -70.f, 0.f, false);

        Mtx_RotateX(&view, -C3D_AngleFromDegrees(30.f), true);

        C3D_Mtx vp;
        Mtx_Multiply(&vp, &projection, &view);

        m_clouds->Draw(m_world_shader_uLocProjection, &vp, m_world, 0.f, 0.f);

        SpriteBatch_BindTexture(&m_logoTex);

        SpriteBatch_SetScale(2);
        SpriteBatch_PushQuad(100 / 2 - 76 / 2, 120 / 2, 0, 256, 64, 0, 0, 128, 32);

        SpriteBatch_PushText(0, 0, 0, INT16_MAX, true, INT_MAX, NULL,
                             "v" CRAFTUS_VERSION_STR);
    }

    C3D_BindProgram(&m_gui_shader);
    C3D_SetAttrInfo(&m_gui_vertexAttribs);

    SpriteBatch_Render(GFX_TOP);
}

void Renderer::RenderLowerScreen(DebugUI *debugUi) {
    C3D_RenderTargetClear(m_lowerScreen, C3D_CLEAR_ALL, CLEAR_COLOR_BLACK, 0);
    C3D_FrameDrawOn(m_lowerScreen);

    SpriteBatch_StartFrame(320, 240);

    if (*ReCraftCore::GetInstance()->GetGameState() == GameState_SelectWorld) {
        WorldSelect_Render();
    } else {
        SpriteBatch_SetScale(2);

        Inventory::renderHotbar(160 / 2 - 194 / 2,
            120 - Inventory::QUICKSELECT_HEIGHT,
            m_player->quickSelectBar, m_player->quickSelectBarSlot
        );

        m_player->inventorySite = Inventory::draw(
            ((137 / 2) - (120 / 2)), 10, m_player->inventory,
            sizeof(m_player->inventory) / sizeof(ItemStack), m_player->inventorySite
        );

        if (showDebugInfo) {
            debugUi->Draw();
        }

    }

    Gui::Frame();

    SpriteBatch_Render(GFX_BOTTOM);
}