#pragma once

#include <citro3d.h>

#include "../world/CT_World.hpp"
#include "VertexFmt.hpp"

class CubeRenderer {
public:
    CubeRenderer();
    ~CubeRenderer();

    void Draw(int projUniform, C3D_Mtx* projectionview, World* world, mc::Vector3f position);

private:
    WorldVertex* m_cloudVBO = nullptr;
    float m_posX = 0, m_posY = 0, m_posZ = 0;
};