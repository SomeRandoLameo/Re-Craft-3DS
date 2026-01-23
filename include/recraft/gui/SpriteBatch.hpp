#pragma once

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>


#include "../misc/InputData.hpp"
#include "../world/CT_World.hpp"

#include "mclib/inventory/Slot.h"


#include <citro3d.h>

void SpriteBatch_Init(int projUniform_);
void SpriteBatch_Deinit();

typedef enum {
    Blank,
    FontTex,
    Icons,
    Widgets,
    MenuBackground
} GuiTexture;

enum SpriteBatch {
    CharWidth = 8,
    CharHeight = 8,
    TabSize = 4
};

void SpriteBatch_BindGuiTexture(GuiTexture texture);
void SpriteBatch_BindTexture(C3D_Tex* texture);

void SpriteBatch_PushSingleColorQuad(int x, int y, int z, int w, int h, int16_t color);
void SpriteBatch_PushQuadColor(int x, int y, int z, int w, int h, int rx, int ry, int rw, int rh, int16_t color);
void SpriteBatch_PushQuad(int x, int y, int z, int w, int h, int rx, int ry, int rw, int rh);
void SpriteBatch_PushIcon(mc::inventory::Slot block, int x, int y, int z);

int SpriteBatch_PushTextVargs(int x, int y, int z, int16_t color, bool shadow, int wrap, int* ySize, const char* fmt, va_list arg);
int SpriteBatch_PushText(int x, int y, int z, int16_t color, bool shadow, int wrap, int* ySize, const char* fmt, ...);

int SpriteBatch_CalcTextWidthVargs(const char* text, va_list args);
int SpriteBatch_CalcTextWidth(const char* text, ...);

bool SpriteBatch_RectIsVisible(int x, int y, int w, int h);

void SpriteBatch_SetScale(int scale);
int SpriteBatch_GetScale();

int SpriteBatch_GetWidth();
int SpriteBatch_GetHeight();

void SpriteBatch_StartFrame(int width, int height);
void SpriteBatch_Render(gfxScreen_t screen);