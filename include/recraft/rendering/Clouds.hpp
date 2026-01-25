#pragma once

#include <citro3d.h>

class WorldVertex;
class Clouds {
public:
    Clouds();
    ~Clouds();

    void Draw(int projUniform, C3D_Mtx* projectionview, float tx, float tz);

private:
    WorldVertex* m_cloudVBO = nullptr;
    C3D_Tex      m_texture;

    static const int TEXTURE_SIZE = 64;
};
