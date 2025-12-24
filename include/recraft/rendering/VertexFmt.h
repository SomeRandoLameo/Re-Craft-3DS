#pragma once

#include <cstdint>

#include "../misc/NumberUtils.h"

typedef struct {
	int16_t xyz[3];
	int16_t uv[2];
	uint8_t rgb[3];
	uint8_t fxyz[3];
} WorldVertex;

extern const WorldVertex cube_sides_lut[6 * 6];

typedef struct {
	int16_t xyz[3];
	int16_t uvc[3];
} GuiVertex;

#define SHADER_RGB(r, g, b) (((b & 0x1f)) | (((g)&0x1f) << 5) | (((r)&0x1f) << 10))

#define SHADER_R(r) (((r) >> 10) & 0x1f)
#define SHADER_G(g) (((g) >> 5) & 0x1f)
#define SHADER_B(b) ((b)&0x1f)
#define SHADER_RGB_MIX(a, b) SHADER_RGB((SHADER_R(a) + SHADER_R(b)) / 2, (SHADER_G(a) + SHADER_G(b)) / 2, (SHADER_B(a) + SHADER_B(b)) / 2)

#define SHADER_RGB_DARKEN(a, f) SHADER_RGB(((int)SHADER_R(a)) * f / 16, ((int)SHADER_G(a)) * f / 16, ((int)SHADER_B(a)) * f / 16)
