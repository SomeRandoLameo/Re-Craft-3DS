#include "gui/SpriteBatch.hpp"

#include "gui/DebugUI.hpp"
#include "gui/FontLoader.hpp"
#include "mcbridge/MCBridge.hpp"
#include "misc/NumberUtils.hpp"
#include "rendering/TextureMap.hpp"
#include "rendering/VertexFmt.hpp"


#include <stdarg.h>

struct Sprite {
    int      depth;
    C3D_Tex* texture;
    s16      x0, y0, x1, y1; // top left, right
    s16      x2, y2, x3, y3; // bottom left, right
    s16      u0, v0, u1, v1;
    s16      color;
};

static std::vector<Sprite>       cmdList;
static C3D_Tex*                  currentTexture = NULL;
static std::array<GuiVertex*, 2> vertexList;
static int                       projUniform;

static Font*   font;
static C3D_Tex whiteTex;
static C3D_Tex widgetsTex;
static C3D_Tex iconsTex;
static C3D_Tex menuBackgroundTex;

static C3D_Mtx iconModelMtx;

static int screenWidth = 0, screenHeight = 0;
static int guiScale = 2;

void SpriteBatch_Init(int projUniform_) {
    vertexList[0] = (GuiVertex*)linearAlloc(sizeof(GuiVertex) * 2 * (4096 + 1024));
    vertexList[1] = (GuiVertex*)linearAlloc(sizeof(GuiVertex) * 2 * (4096 + 1024));

    projUniform = projUniform_;

    font = (Font*)malloc(sizeof(Font));
    FontLoader fontLoader;
    fontLoader.LoadFont(font, "romfs:/assets/textures/font/ascii.png");

    Texture_Load(&widgetsTex, "romfs:/assets/textures/gui/widgets.png");

    Texture_Load(&iconsTex, "romfs:/assets/textures/gui/icons.png");

    u8 data[16 * 16];
    memset(data, 0xff, 16 * 16 * sizeof(u8));
    C3D_TexInit(&whiteTex, 16, 16, GPU_L8);
    C3D_TexLoadImage(&whiteTex, data, GPU_TEXFACE_2D, 0);

    Texture_Load(&menuBackgroundTex, "romfs:/assets/textures/gui/options_background.png");

    Mtx_Identity(&iconModelMtx);
    Mtx_RotateY(&iconModelMtx, M_PI / 4.f, false);
    Mtx_RotateX(&iconModelMtx, M_PI / 6.f, false);
}
void SpriteBatch_Deinit() {
    cmdList.clear();
    linearFree(vertexList[0]);
    linearFree(vertexList[1]);

    C3D_TexDelete(&font->texture);
    free(font);

    C3D_TexDelete(&whiteTex);
    C3D_TexDelete(&widgetsTex);
    C3D_TexDelete(&menuBackgroundTex);
}

void SpriteBatch_BindTexture(C3D_Tex* texture) {
    currentTexture = texture;
}
void SpriteBatch_BindGuiTexture(GuiTexture texture) {
    switch (texture) {
    case GuiTexture::Blank:
        currentTexture = &whiteTex;
        break;
    case GuiTexture::FontTex:
        currentTexture = &font->texture;
        break;
    case GuiTexture::Widgets:
        currentTexture = &widgetsTex;
        break;
    case GuiTexture::Icons:
        currentTexture = &iconsTex;
        break;
    case GuiTexture::MenuBackground:
        currentTexture = &menuBackgroundTex;
        break;
    default:
        break;
    }
}

void SpriteBatch_PushSingleColorQuad(int x, int y, int z, int w, int h, s16 color) {
    SpriteBatch_BindTexture(&whiteTex);
    SpriteBatch_PushQuadColor(x, y, z, w, h, 0, 0, 4, 4, color);
}
void SpriteBatch_PushQuad(int x, int y, int z, int w, int h, int rx, int ry, int rw, int rh) {
    SpriteBatch_PushQuadColor(x, y, z, w, h, rx, ry, rw, rh, INT16_MAX);
}
void SpriteBatch_PushQuadColor(int x, int y, int z, int w, int h, int rx, int ry, int rw, int rh, s16 color) {
    cmdList.push_back(Sprite{z, currentTexture, static_cast<s16>(x * guiScale), static_cast<s16>(y * guiScale),
                             static_cast<s16>((x + w) * guiScale), static_cast<s16>(y * guiScale),
                             static_cast<s16>(x * guiScale), static_cast<s16>((y + h) * guiScale),
                             static_cast<s16>((x + w) * guiScale), static_cast<s16>((y + h) * guiScale),
                             static_cast<s16>(rx), static_cast<s16>(ry), static_cast<s16>(rx + rw),
                             static_cast<s16>(ry + rh), color});
}

static float rot = 0.f;


void SpriteBatch_PushIcon(mc::inventory::Slot block, int x, int y, int z) {
    WorldVertex vertices[6 * 6];
    memcpy(vertices, cube_sides_lut, sizeof(cube_sides_lut));
    for (int i = 0; i < 6; i++) {
        if (i != Direction::Top && i != Direction::South && i != Direction::West) {
            continue;
        }
        s16 iconUV[2];
        Block_GetTexture(block, (Direction)i, iconUV);

        u8 color[3];
        Block_GetColor(MCBridge::MCLIBSlotToCTItemStack(block).block, MCBridge::MCLIBSlotToCTItemStack(block).meta,
                       (Direction)i, color);

        for (int j = 0; j < 5; j++) {
            int      k         = i * 6 + j;
            C3D_FVec p         = FVec3_New((float)vertices[k].xyz[0] - 0.5f, (float)vertices[k].xyz[1] - 0.5f,
                                           (float)vertices[k].xyz[2] - 0.5f);
            C3D_FVec v         = Mtx_MultiplyFVec3(&iconModelMtx, p);
            vertices[k].xyz[0] = FastFloor(v.x * 20.f * guiScale) + (x + 16) * guiScale;
            vertices[k].xyz[1] = -FastFloor(v.y * 20.f * guiScale) + (y + 16) * guiScale; // invertieren auf der Y-Achse
        }

        WorldVertex bottomLeft  = vertices[i * 6 + 0];
        WorldVertex bottomRight = vertices[i * 6 + 1];
        WorldVertex topRight    = vertices[i * 6 + 2];
        WorldVertex topLeft     = vertices[i * 6 + 4];

        C3D_Tex* texture = &((Texture_Map*)Block_GetTextureMap())->texture;

        s16 color16 = SHADER_RGB(color[0] >> 3, color[1] >> 3, color[2] >> 3);
        if (i == Direction::South) {
            color16 = SHADER_RGB_DARKEN(color16, 14);
        } else if (i == Direction::West) {
            color16 = SHADER_RGB_DARKEN(color16, 10);
        }

#define unpackP(x) (x).xyz[0], (x).xyz[1]
        cmdList.push_back(
            Sprite{z, texture, unpackP(topLeft), unpackP(topRight), unpackP(bottomLeft), unpackP(bottomRight),
                   static_cast<s16>(iconUV[0] / 256), static_cast<s16>(iconUV[1] / 256 + Texture::TileSize),
                   static_cast<s16>(iconUV[0] / 256 + Texture::TileSize), static_cast<s16>(iconUV[1] / 256), color16});

#undef unpackP
    }
}

int SpriteBatch_PushText(int x, int y, int z, s16 color, bool shadow, int wrap, int* ySize, const char* fmt, ...) {
    va_list arg;
    va_start(arg, fmt);
    int length = SpriteBatch_PushTextVargs(x, y, z, color, shadow, wrap, ySize, fmt, arg);
    va_end(arg);
    return length;
}

int SpriteBatch_PushTextVargs(int x, int y, int z, s16 color, bool shadow, int wrap, int* ySize, const char* fmt,
                              va_list arg) {
    SpriteBatch_BindTexture(&font->texture);

    char buffer[256];
    vsprintf(buffer, fmt, arg);

    int offsetX = 0;
    int offsetY = 0;

    int maxWidth = 0;

    char* text = buffer;

    while (*text != '\0') {
        bool implicitBreak = offsetX + font->fontWidth[(int)*text] >= wrap;
        if (*text == '\n' || implicitBreak) {
            offsetY += SpriteBatch::CharHeight;
            maxWidth = MAX(maxWidth, offsetX);
            offsetX  = 0;
            if (implicitBreak) {
                --text;
            }
        } else if (*text == '\t') {
            offsetX = ((offsetX / CharWidth) / TabSize + 1) * TabSize * CharWidth;
        } else {
            if (*text != ' ') {
                int texX = *text % 16 * 8, texY = *text / 16 * 8;
                SpriteBatch_PushQuadColor(x + offsetX, y + offsetY, z, 8, 8, texX, texY, 8, 8, color);
                if (shadow) {
                    SpriteBatch_PushQuadColor(x + offsetX + 1, y + offsetY + 1, z - 1, 8, 8, texX, texY, 8, 8,
                                              SHADER_RGB(10, 10, 10));
                }
            }
            offsetX += font->fontWidth[(int)*text];
        }
        ++text;
    }

    maxWidth = MAX(maxWidth, offsetX);

    if (ySize != NULL) {
        *ySize = offsetY + SpriteBatch::CharHeight;
    }

    return maxWidth;
}

int SpriteBatch_CalcTextWidth(const char* text, ...) {
    va_list args;
    va_start(args, text);
    int length = SpriteBatch_CalcTextWidthVargs(text, args);
    va_end(args);

    return length;
}

int SpriteBatch_CalcTextWidthVargs(const char* text, va_list args) {
    char fmtedText[256];
    vsprintf(fmtedText, text, args);

    char* it = fmtedText;

    int length    = 0;
    int maxLength = 0;
    while (*it != '\0') {
        if (*it == '\n') {
            maxLength = MAX(maxLength, length);
            length    = 0;
        } else {
            length += font->fontWidth[(int)*(it++)];
        }
    }

    maxLength = MAX(maxLength, length);

    return maxLength;
}

static int compareDrawCommands(const void* a, const void* b) {
    Sprite* ga = ((Sprite*)a);
    Sprite* gb = ((Sprite*)b);

    return ga->depth == gb->depth ? gb->texture - ga->texture : gb->depth - ga->depth;
}

int SpriteBatch_GetWidth() {
    return screenWidth / guiScale;
}
int SpriteBatch_GetHeight() {
    return screenHeight / guiScale;
}

void SpriteBatch_SetScale(int scale) {
    guiScale = scale;
}
int SpriteBatch_GetScale() {
    return guiScale;
}

void SpriteBatch_StartFrame(int width, int height) {
    screenWidth  = width;
    screenHeight = height;
}

void SpriteBatch_Render(gfxScreen_t screen) {
    rot += M_PI / 60.f;

    // sort
    std::sort(cmdList.begin(), cmdList.end(),
              [](const Sprite& a, const Sprite& b) { return compareDrawCommands(&a, &b) < 0; });

    C3D_Mtx projMtx;
    Mtx_OrthoTilt(&projMtx, 0.f, screenWidth, screenHeight, 0.f, 1.f, -1.f, false);

    C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, projUniform, &projMtx);

    C3D_DepthTest(false, GPU_GREATER, GPU_WRITE_ALL);

    C3D_TexEnv* env = C3D_GetTexEnv(0);
    C3D_TexEnvInit(env);
    C3D_TexEnvSrc(env, C3D_Both, GPU_TEXTURE0, GPU_PRIMARY_COLOR, (GPU_TEVSRC)0);
    C3D_TexEnvFunc(env, C3D_Both, GPU_MODULATE);

    GuiVertex* usedVertexList = vertexList[screen];

    int verticesTotal = 0;

    size_t vtx = 0;
    while (!cmdList.empty()) {
        size_t vtxStart = vtx;

        C3D_Tex* texture = cmdList.back().texture;
        if (texture == nullptr) {
            continue;
        }
        float divW = 1.f / texture->width * INT16_MAX;
        float divH = 1.f / texture->height * INT16_MAX;

        while (!cmdList.empty() && cmdList.back().texture == texture) {
            Sprite cmd = cmdList.back();
            cmdList.pop_back();

            s16 color = cmd.color;

            s16 u0 = (s16)(cmd.u0 * divW);
            s16 v0 = (s16)(cmd.v0 * divH);
            s16 u1 = (s16)(cmd.u1 * divW);
            s16 v1 = (s16)(cmd.v1 * divH);

            usedVertexList[vtx++] = {{cmd.x3, cmd.y3, 0}, {u1, v1, color}};
            usedVertexList[vtx++] = {{cmd.x1, cmd.y1, 0}, {u1, v0, color}};
            usedVertexList[vtx++] = {{cmd.x0, cmd.y0, 0}, {u0, v0, color}};

            usedVertexList[vtx++] = {{cmd.x0, cmd.y0, 0}, {u0, v0, color}};
            usedVertexList[vtx++] = {{cmd.x2, cmd.y2, 0}, {u0, v1, color}};
            usedVertexList[vtx++] = {{cmd.x3, cmd.y3, 0}, {u1, v1, color}};
        }

        C3D_TexBind(0, texture);

        C3D_BufInfo* bufInfo = C3D_GetBufInfo();
        BufInfo_Init(bufInfo);
        BufInfo_Add(bufInfo, usedVertexList + vtxStart, sizeof(GuiVertex), 2, 0x10);

        C3D_DrawArrays(GPU_TRIANGLES, 0, vtx - vtxStart);

        verticesTotal += vtx - vtxStart;
    }
    C3D_DepthTest(true, GPU_GREATER, GPU_WRITE_ALL);

    currentTexture = NULL;
    guiScale       = 2;
}
