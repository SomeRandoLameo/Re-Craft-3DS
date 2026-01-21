#include "rendering/Clouds.h"

#include "rendering/VertexFmt.h"
extern "C" {
#include <sino/sino.h>
}
#include <stdint.h>

#include "rendering/TextureMap.h"

#include <amethyst.hpp>

static WorldVertex vertices[] = {{{-1, 0, -1}, {0, 0}, {255, 255, 255}, {0, 0, 0}},
                                 {{1, 0, -1}, {INT16_MAX, 0}, {255, 255, 255}, {0, 0, 0}},
                                 {{1, 0, 1}, {INT16_MAX, INT16_MAX}, {255, 255, 255}, {0, 0, 0}},
                                 {{1, 0, 1}, {INT16_MAX, INT16_MAX}, {255, 255, 255}, {0, 0, 0}},
                                 {{-1, 0, 1}, {0, INT16_MAX}, {255, 255, 255}, {0, 0, 0}},
                                 {{-1, 0, -1}, {0, 0}, {255, 255, 255}, {0, 0, 0}}};

Clouds::Clouds() {
    uint8_t* map = (uint8_t*)malloc(TEXTURE_SIZE * TEXTURE_SIZE);
    for (int i = 0; i < TEXTURE_SIZE; i++) {
        for (int j = 0; j < TEXTURE_SIZE; j++) {
            float noise = sino_2d(j * 0.2f, i * 0.3f);
            for (int k = 1; k < 3; k++) {
                noise += sino_2d(j * 0.15f / 1, i * 0.2f / 1);
            }
            map[j + i * TEXTURE_SIZE] = (noise / 3.f > 0.2f) * 15 | (15 << 4);
        }
    }
    C3D_TexInit(&m_texture, TEXTURE_SIZE, TEXTURE_SIZE, GPU_LA4);
    C3D_TexSetWrap(&m_texture, GPU_REPEAT, GPU_REPEAT);
    Texture_TileImage8(map, (uint8_t*)m_texture.data, TEXTURE_SIZE);

    free(map);

    m_cloudVBO = (WorldVertex*)linearAlloc(sizeof(vertices));
    memcpy(m_cloudVBO, vertices, sizeof(vertices));
}

Clouds::~Clouds() {
    C3D_TexDelete(&m_texture);
    linearFree(m_cloudVBO);
}

void Clouds::Draw(int projUniform, C3D_Mtx* projectionview, World* world, float tx, float tz) {
    C3D_Mtx model;
    Mtx_Identity(&model);
    Mtx_Translate(&model, tx, 90.f, tz, true);
    Mtx_Scale(&model, 90.f, 90.f, 90.f);

    C3D_CullFace(GPU_CULL_NONE);

    C3D_AlphaTest(true, GPU_GREATER, 0);

    C3D_TexBind(0, &m_texture);

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
