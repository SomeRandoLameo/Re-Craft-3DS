#include "rendering/Renderer.h"

#include "blocks/CT_Block.h"
#include "gui/Gui.h"
#include "gui/CT_Inventory.h"
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

C3D_RenderTarget* Top;
C3D_RenderTarget* Bottom;


#define DISPLAY_TRANSFER_FLAGS																										  \
	(GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) | GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | \
	 GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) | \
GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO))

#define CLEAR_COLOR_SKY 0x90d9ffff
#define CLEAR_COLOR_BLACK 0x000000ff

//TODO: Fix this
extern bool showDebugInfo;

Renderer::Renderer(World* world_, Player* player_, WorkQueue* queue){
    // i actually prefer this
    this->world_dvlb = nullptr;
    this->gui_dvlb = nullptr;
    this->world = world_;
    this->player = player_;
    this->workqueue = queue;
    this->worldRenderer = nullptr;
    this->world_shader_uLocProjection = 0;
    this->gui_shader_uLocProjection = 0;


	renderTargets[0] = nullptr;
	renderTargets[1] = nullptr;
	lowerScreen = nullptr;

	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);

	renderTargets[0] = C3D_RenderTargetCreate(240, 400, GPU_RB_RGBA8, GPU_RB_DEPTH16);
	renderTargets[1] = C3D_RenderTargetCreate(240, 400, GPU_RB_RGBA8, GPU_RB_DEPTH16);
	C3D_RenderTargetSetOutput(renderTargets[0], GFX_TOP, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);
	C3D_RenderTargetSetOutput(renderTargets[1], GFX_TOP, GFX_RIGHT, DISPLAY_TRANSFER_FLAGS);

	lowerScreen = C3D_RenderTargetCreate(240, 320, GPU_RB_RGBA8, GPU_RB_DEPTH16);
	C3D_RenderTargetSetOutput(lowerScreen, GFX_BOTTOM, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);

    Top = renderTargets[0];
    Bottom = lowerScreen;

	world_dvlb = DVLB_ParseFile((u32*)world_shbin, world_shbin_size);
	shaderProgramInit(&world_shader);
	shaderProgramSetVsh(&world_shader, &world_dvlb->DVLE[0]);
	world_shader_uLocProjection = shaderInstanceGetUniformLocation(world_shader.vertexShader, "projection");

	gui_dvlb = DVLB_ParseFile((u32*)gui_shbin, gui_shbin_size);
	shaderProgramInit(&gui_shader);
	shaderProgramSetVsh(&gui_shader, &gui_dvlb->DVLE[0]);
	gui_shader_uLocProjection = shaderInstanceGetUniformLocation(gui_shader.vertexShader, "projection");

	AttrInfo_Init(&world_vertexAttribs);
	AttrInfo_AddLoader(&world_vertexAttribs, 0, GPU_SHORT, 3);
	AttrInfo_AddLoader(&world_vertexAttribs, 1, GPU_SHORT, 2);
	AttrInfo_AddLoader(&world_vertexAttribs, 2, GPU_UNSIGNED_BYTE, 3);
	AttrInfo_AddLoader(&world_vertexAttribs, 3, GPU_UNSIGNED_BYTE, 3);

	AttrInfo_Init(&gui_vertexAttribs);
	AttrInfo_AddLoader(&gui_vertexAttribs, 0, GPU_SHORT, 3);
	AttrInfo_AddLoader(&gui_vertexAttribs, 1, GPU_SHORT, 3);

	PolyGen_Init(world, player);

	worldRenderer = new WorldRenderer(player, world, workqueue, world_shader_uLocProjection);

	SpriteBatch_Init(gui_shader_uLocProjection);

	Gui_Init();

	C3D_CullFace(GPU_CULL_BACK_CCW);

	Block_Init();

	Texture_Load(&logoTex, "romfs:/assets/textures/gui/title/craftus.png");

    ImGuiManager::GetInstance()->Initialize();

    show_demo_window = false;
}

Renderer::~Renderer() {
	C3D_TexDelete(&logoTex);

	Block_Deinit();

	PolyGen_Deinit();

	delete worldRenderer;
	worldRenderer = nullptr;

	Gui_Deinit();

	SpriteBatch_Deinit();

	shaderProgramFree(&gui_shader);
	if (gui_dvlb) DVLB_Free(gui_dvlb);

	shaderProgramFree(&world_shader);
	if (world_dvlb) DVLB_Free(world_dvlb);

	C3D_Fini();
}


void Renderer::Render(DebugUI* debugUi) {
    float iod = osGet3DSliderState() * PLAYER_HALFEYEDIFF;

    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);

    if (*ReCraftCore::GetInstance()->GetGameState() == GameState_Playing)
        PolyGen_Harvest(debugUi);

    for (int i = 0; i < 2; i++) {
        RenderFrame(i, iod);
        if (iod <= 0.f) break;
    }

    RenderLowerScreen(debugUi);

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(SCREEN_WIDTH, SCREEN_HEIGHT);
    io.DisplayFramebufferScale = ImVec2(FB_SCALE, FB_SCALE);

    ImGuiManager::GetInstance()->BeginFrame();

    ImGuiManager::GetInstance()->ExecuteCallbacks();

    C3D_FrameDrawOn(renderTargets[0]);
    C3D_FrameDrawOn(lowerScreen);

    ImGuiManager::GetInstance()->EndFrame(
            reinterpret_cast<void*>(renderTargets[0]),
            reinterpret_cast<void*>(lowerScreen)
    );

    C3D_DepthTest(true, GPU_GREATER, GPU_WRITE_ALL);
    C3D_CullFace(GPU_CULL_BACK_CCW);

    C3D_BindProgram(&world_shader);
    C3D_SetAttrInfo(&world_vertexAttribs);

    C3D_FrameEnd(0);
}


void Renderer::RenderFrame(int eyeIndex, float iod) {
	C3D_RenderTargetClear(renderTargets[eyeIndex], C3D_CLEAR_ALL, CLEAR_COLOR_SKY, 0);
	C3D_FrameDrawOn(renderTargets[eyeIndex]);

	SpriteBatch_StartFrame(400, 240);

	C3D_TexEnv* env = C3D_GetTexEnv(0);
	C3D_TexEnvInit(env);
	C3D_TexEnvSrc(env, C3D_Both, GPU_TEXTURE0, GPU_PRIMARY_COLOR, (GPU_TEVSRC)0);
	C3D_TexEnvFunc(env, C3D_Both, GPU_MODULATE);

	C3D_BindProgram(&world_shader);
	C3D_SetAttrInfo(&world_vertexAttribs);

	if (*ReCraftCore::GetInstance()->GetGameState() == GameState_Playing) {
		C3D_TexBind(0, (C3D_Tex*)Block_GetTextureMap());

		worldRenderer->Render(!eyeIndex ? -iod : iod);

		SpriteBatch_BindGuiTexture(GuiTexture_Widgets);
		if (iod == 0.f) SpriteBatch_PushQuad(200 / 2 - 16 / 2, 120 / 2 - 16 / 2, 0, 16, 16, 240, 0, 16, 16);
	} else if (*ReCraftCore::GetInstance()->GetGameState() == GameState_Playing) {
        C3D_TexBind(0, (C3D_Tex*)Block_GetTextureMap());

        worldRenderer->Render(!eyeIndex ? -iod : iod);

        SpriteBatch_BindGuiTexture(GuiTexture_Widgets);
        if (iod == 0.f) SpriteBatch_PushQuad(200 / 2 - 16 / 2, 120 / 2 - 16 / 2, 0, 16, 16, 240, 0, 16, 16);
    } else if (*ReCraftCore::GetInstance()->GetGameState() == GameState_Playing_OnLine) {
        C3D_TexBind(0, (C3D_Tex*)Block_GetTextureMap());

        //TODO: There needs to be a world to render :D
        //worldRenderer->Render(!eyeIndex ? -iod : iod);

        SpriteBatch_BindGuiTexture(GuiTexture_Widgets);
        if (iod == 0.f) SpriteBatch_PushQuad(200 / 2 - 16 / 2, 120 / 2 - 16 / 2, 0, 16, 16, 240, 0, 16, 16);
    } else {
		C3D_Mtx projection;
		Mtx_PerspStereoTilt(&projection, C3D_AngleFromDegrees(90.f), ((400.f) / (240.f)), 0.22f, 4.f * CHUNK_SIZE,
							!eyeIndex ? -iod : iod, 3.f, false);

		C3D_Mtx view;
		Mtx_Identity(&view);
		Mtx_Translate(&view, 0.f, -70.f, 0.f, false);

		Mtx_RotateX(&view, -C3D_AngleFromDegrees(30.f), true);

		C3D_Mtx vp;
		Mtx_Multiply(&vp, &projection, &view);

		Clouds_Render(world_shader_uLocProjection, &vp, world, 0.f, 0.f);

		SpriteBatch_BindTexture(&logoTex);

		SpriteBatch_SetScale(2);
		SpriteBatch_PushQuad(100 / 2 - 76 / 2, 120 / 2, 0, 256, 64, 0, 0, 128, 32);

		SpriteBatch_PushText(0, 0, 0, INT16_MAX, true, INT_MAX, NULL, "v" CRAFTUS_VERSION_STR);
	}

	C3D_BindProgram(&gui_shader);
	C3D_SetAttrInfo(&gui_vertexAttribs);

	SpriteBatch_Render(GFX_TOP);
}

void Renderer::RenderLowerScreen(DebugUI* debugUi) {
	C3D_RenderTargetClear(lowerScreen, C3D_CLEAR_ALL, CLEAR_COLOR_BLACK, 0);
	C3D_FrameDrawOn(lowerScreen);

	SpriteBatch_StartFrame(320, 240);

	if (*ReCraftCore::GetInstance()->GetGameState() == GameState_SelectWorld) {
		WorldSelect_Render();
	} else {
		SpriteBatch_SetScale(2);
		player->quickSelectBarSlots = Inventory::calculateQuickSelectSlots();
        Inventory::renderHotbar(160 / 2 - Inventory::calculateQuickSelectWidth(player->quickSelectBarSlots) / 2,
                               120 - Inventory::QUICKSELECT_HEIGHT, player->quickSelectBar, player->quickSelectBarSlots,
                               player->quickSelectBarSlot);
		player->inventorySite = Inventory::draw(16, 0, 170, player->inventory, sizeof(player->inventory) / sizeof(ItemStack), player->inventorySite);

		if (showDebugInfo) debugUi->Draw();
	}

	Gui_Frame();

	SpriteBatch_Render(GFX_BOTTOM);
}