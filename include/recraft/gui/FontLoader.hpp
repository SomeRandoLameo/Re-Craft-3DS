#pragma once


#include <citro3d.h>

struct Font {
    u8      fontWidth[1 << 8];
    C3D_Tex texture;
};

class FontLoader {
public:
    void LoadFont(Font* font, const char* filename);
};
