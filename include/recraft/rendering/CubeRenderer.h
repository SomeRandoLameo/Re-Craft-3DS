#pragma once

#include <citro3d.h>

#include "../world/CT_World.h"
#include "VertexFmt.h"

class CubeRenderer {
public:
    CubeRenderer();
    ~CubeRenderer();

    void Draw(int projUniform, C3D_Mtx* projectionview, World* world, mc::Vector3d position);

private:
    WorldVertex* m_cloudVBO = nullptr;
    float m_posX = 0, m_posY = 0, m_posZ = 0;
};