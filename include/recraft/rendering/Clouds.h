#pragma once

#include <citro3d.h>

#include "../world/CT_World.h"
#include "VertexFmt.h"

class Clouds {
public:
    Clouds();
    ~Clouds();

    void Draw(int projUniform, C3D_Mtx* projectionview, World* world, float tx, float tz);
private:
    WorldVertex* cloudVBO;
    C3D_Tex texture;

    static const int TEXTURE_SIZE = 64;
};
