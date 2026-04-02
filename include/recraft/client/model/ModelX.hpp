#pragma once

#include <citro3d.h>
#include "client/renderer/Tessellator.hpp"
#include "client/renderer/TextureMap.hpp"
#include "block/Block.hpp"
#include "misc/NumberUtils.hpp"
#include <cmath>

class ModelX {
public:
    ModelX() = default;
    ~ModelX() = default;

    void Render(int projUniform, C3D_Mtx* vp,
               float worldX, float worldY, float worldZ,
               BlockID blockId, uint8_t metadata,
               float r, float g, float b);

private:
    void emitPlane(Tessellator* t,
                   float cx, float cy, float cz,
                   float angleY,
                   float u0, float v0,
                   float u1, float v1,
                   float r, float g, float b);
};