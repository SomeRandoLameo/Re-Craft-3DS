#pragma once

#include <amethyst.hpp>
#include <citro3d.h>
#include "client/renderer/Tessellator.hpp"
#include "client/renderer/TextureMap.hpp"

class ChestRenderer {
public:
    ChestRenderer();
    ~ChestRenderer();

    void Draw(int projUniform, C3D_Mtx* vp, float worldX, float worldY, float worldZ);

private:
    Amy::Texture m_chestTexture;

    // TODO: Move somewhere generally used
    void emitFace(Tessellator* t, float x0, float y0, float z0, float x1, float y1, float z1,
                  int axis, // 0=X  1=Y  2=Z
                  bool positiveDir, // which side of the slab
                  float u0, float v0, float u1, float v1, float r, float g, float b);

    void emitBox(Tessellator* tess, float offx, float offy, float offz, float width, float height, float depth, int texX, int texY, float r,
                 float g, float b);
};
