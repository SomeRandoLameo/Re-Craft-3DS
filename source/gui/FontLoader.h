#pragma once

#include <stdint.h>

#include <citro3d.h>

typedef struct {
	uint8_t fontWidth[1 << 8];
	C3D_Tex texture;
} Font;

class FontLoader{
public:
    void LoadFont(Font* font, const char* filename);
};
