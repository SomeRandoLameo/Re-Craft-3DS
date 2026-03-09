#pragma once

#include <citro3d.h>

#include <amethyst.hpp>

#include "VertexFmt.hpp"
#include "world/World.hpp"

class Clouds {
public:
    Clouds();
    ~Clouds();

    void Draw(int projUniform, C3D_Mtx* projectionview, float tx, float tz);

private:
    WorldVertex* m_cloudVBO = nullptr;
    Amy::Texture m_texture;

    static const int TEXTURE_SIZE = 64;
};
