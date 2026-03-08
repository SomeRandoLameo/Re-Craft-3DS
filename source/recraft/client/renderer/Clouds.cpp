#include "client/renderer/Clouds.hpp"
#include <stdint.h>
#include "client/renderer/VertexFmt.hpp"

#include "amethyst/include/amethyst.hpp"
#include "client/renderer/TextureMap.hpp"

static WorldVertex vertices[] = {{{-1, 0, -1}, {0, 0}, {255, 255, 255}, {0, 0, 0}},
                                 {{1, 0, -1}, {INT16_MAX, 0}, {255, 255, 255}, {0, 0, 0}},
                                 {{1, 0, 1}, {INT16_MAX, INT16_MAX}, {255, 255, 255}, {0, 0, 0}},
                                 {{1, 0, 1}, {INT16_MAX, INT16_MAX}, {255, 255, 255}, {0, 0, 0}},
                                 {{-1, 0, 1}, {0, INT16_MAX}, {255, 255, 255}, {0, 0, 0}},
                                 {{-1, 0, -1}, {0, 0}, {255, 255, 255}, {0, 0, 0}}};

Clouds::Clouds() {
    m_texture.Load("romfs:/assets/minecraft/textures/environment/clouds.png", Amy::Texture::Repeat);

    m_cloudVBO = (WorldVertex*)linearAlloc(sizeof(vertices));
    memcpy(m_cloudVBO, vertices, sizeof(vertices));
}

Clouds::~Clouds() {
    m_texture.Unload();
    linearFree(m_cloudVBO);
}

void Clouds::Draw(int projUniform, C3D_Mtx* projectionview, float tx, float tz) {
    C3D_Mtx model;
    Mtx_Identity(&model);
    Mtx_Translate(&model, tx, 90.f, tz, true);
    Mtx_Scale(&model, 90.f, 90.f, 90.f);

    C3D_CullFace(GPU_CULL_NONE);

    C3D_AlphaTest(true, GPU_GREATER, 0);

    m_texture.Bind();

    const int stepX = 4;
    const int stepZ = 6;
    if (((int)m_cloudVBO[0].uv[0]) - stepX < -INT16_MAX) {
        for (int i = 0; i < 6; i++) {
            if (m_cloudVBO[i].xyz[0] == -1) {
                m_cloudVBO[i].uv[0] = 0;
            } else {
                m_cloudVBO[i].uv[0] = INT16_MAX;
            }
        }
    } else {
        for (int i = 0; i < 6; i++) {
            m_cloudVBO[i].uv[0] -= stepX;
        }
    }
    if (((int)m_cloudVBO[0].uv[1]) + stepZ > INT16_MAX) {
        for (int i = 0; i < 6; i++) {
            if (m_cloudVBO[i].xyz[2] == 1) {
                m_cloudVBO[i].uv[1] = -INT16_MAX;
            } else {
                m_cloudVBO[i].uv[1] = 0;
            }
        }
    } else {
        for (int i = 0; i < 6; i++) {
            m_cloudVBO[i].uv[1] += stepZ;
        }
    }
    GSPGPU_FlushDataCache(m_cloudVBO, sizeof(vertices));

    C3D_Mtx mvp;
    Mtx_Multiply(&mvp, projectionview, &model);

    C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, projUniform, &mvp);

    C3D_BufInfo* bufInfo = C3D_GetBufInfo();
    BufInfo_Init(bufInfo);
    BufInfo_Add(bufInfo, m_cloudVBO, sizeof(WorldVertex), 4, 0x3210);

    C3D_DrawArrays(GPU_TRIANGLES, 0, 6);

    C3D_CullFace(GPU_CULL_BACK_CCW);

    C3D_AlphaTest(false, GPU_GREATER, 0);
}
