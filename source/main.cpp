#include <3ds.h>
#include <citro3d.h>
#include <cstring>
#include <stdio.h>
#include "vshader_shbin.h"

//#include "utils/LogManager.hpp"

#define CLEAR_COLOR 0x68B0D8FF
#define DISPLAY_TRANSFER_FLAGS \
    (GX_TRANSFER_FLIP_VERT(0) | GX_TRANSFER_OUT_TILED(0) | GX_TRANSFER_RAW_COPY(0) | \
    GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) | GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) | \
    GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO))

struct Vertex { float x, y, z; };

static const Vertex vertices[] = {
    { 0.0f, 200.0f, 0.5f },
    { -100.0f, 40.0f, 0.5f },
    { 100.0f, 40.0f, 0.5f },
};

static constexpr size_t vertexCount = sizeof(vertices) / sizeof(vertices[0]);

static DVLB_s* vshader;
static shaderProgram_s program;
static int uLocProjection;
static C3D_Mtx projTop, projBot;
static void* vbo;

static void initScene() {
    vshader = DVLB_ParseFile(const_cast<u32*>(reinterpret_cast<const u32*>(vshader_shbin)), vshader_shbin_size);
    shaderProgramInit(&program);
    shaderProgramSetVsh(&program, &vshader->DVLE[0]);
    C3D_BindProgram(&program);

    uLocProjection = shaderInstanceGetUniformLocation(program.vertexShader, "projection");

    auto* attr = C3D_GetAttrInfo();
    AttrInfo_Init(attr);
    AttrInfo_AddLoader(attr, 0, GPU_FLOAT, 3);
    AttrInfo_AddFixed(attr, 1);
    C3D_FixedAttribSet(1, 1.0f, 1.0f, 1.0f, 1.0f);

    Mtx_OrthoTilt(&projTop, -200.0f, 200.0f, 0.0f, 240.0f, 0.0f, 1.0f, true);
    Mtx_OrthoTilt(&projBot, -160.0f, 160.0f, 0.0f, 240.0f, 0.0f, 1.0f, true);

    vbo = linearAlloc(sizeof(vertices));
    std::memcpy(vbo, vertices, sizeof(vertices));

    auto* buf = C3D_GetBufInfo();
    BufInfo_Init(buf);
    BufInfo_Add(buf, vbo, sizeof(Vertex), 1, 0x0);

    auto* env = C3D_GetTexEnv(0);
    C3D_TexEnvInit(env);
    C3D_TexEnvSrc(env, C3D_Both, GPU_PRIMARY_COLOR, GPU_PRIMARY_COLOR, GPU_PRIMARY_COLOR);
    C3D_TexEnvFunc(env, C3D_Both, GPU_REPLACE);
}

static void renderScene(float t, bool top) {
    float val = (cosf(C3D_Angle(t)) + 1.0f) / 2.0f;
    if (!top) val = 1.0f - val;
    C3D_FixedAttribSet(1, val, val, val, 1.0f);
    C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, uLocProjection, top ? &projTop : &projBot);
    C3D_DrawArrays(GPU_TRIANGLES, 0, vertexCount);
}

static void exitScene() {
    linearFree(vbo);
    shaderProgramFree(&program);
    DVLB_Free(vshader);
}

int main() {
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);

    C3D_RenderTarget* top = C3D_RenderTargetCreate(240, 400, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
    C3D_RenderTargetSetOutput(top, GFX_TOP, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);
    
	//LogManager::Init();

    initScene();

	//LogManager::Log("Normal message");
    //LogManager::Log("Info message", LogType::INFO);
    //LogManager::Log("Warning message", LogType::WARNING);

    float count = 0.0f;
    while (aptMainLoop()) {
		//LogManager::Update();
        hidScanInput();
        if (hidKeysDown() & KEY_START) break;

        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
            C3D_RenderTargetClear(top, C3D_CLEAR_ALL, CLEAR_COLOR, 0);
            C3D_FrameDrawOn(top);
            renderScene(count, true);
        C3D_FrameEnd(0);


        count += 1.0f / 128.0f;
    }

    exitScene();
    C3D_Fini();
    gfxExit();
    return 0;
}
