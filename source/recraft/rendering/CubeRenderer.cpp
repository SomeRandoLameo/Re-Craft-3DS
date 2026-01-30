#include "rendering/CubeRenderer.hpp"

#include <cstdint>
#include <cstring>
#include "rendering/VertexFmt.hpp"

#include "rendering/TextureMap.hpp"

// Cube vertices (6 faces, 2 triangles each = 36 vertices)
static WorldVertex cubeVertices[] = {
        // Front face (Z+)
        {{-1, -1, 1}, {0, 0}, {255, 255, 255}, {0, 0, 0}},
        {{1, -1, 1}, {INT16_MAX, 0}, {255, 255, 255}, {0, 0, 0}},
        {{1, 1, 1}, {INT16_MAX, INT16_MAX}, {255, 255, 255}, {0, 0, 0}},
        {{1, 1, 1}, {INT16_MAX, INT16_MAX}, {255, 255, 255}, {0, 0, 0}},
        {{-1, 1, 1}, {0, INT16_MAX}, {255, 255, 255}, {0, 0, 0}},
        {{-1, -1, 1}, {0, 0}, {255, 255, 255}, {0, 0, 0}},

        // Back face (Z-)
        {{1, -1, -1}, {0, 0}, {255, 255, 255}, {0, 0, 0}},
        {{-1, -1, -1}, {INT16_MAX, 0}, {255, 255, 255}, {0, 0, 0}},
        {{-1, 1, -1}, {INT16_MAX, INT16_MAX}, {255, 255, 255}, {0, 0, 0}},
        {{-1, 1, -1}, {INT16_MAX, INT16_MAX}, {255, 255, 255}, {0, 0, 0}},
        {{1, 1, -1}, {0, INT16_MAX}, {255, 255, 255}, {0, 0, 0}},
        {{1, -1, -1}, {0, 0}, {255, 255, 255}, {0, 0, 0}},

        // Top face (Y+)
        {{-1, 1, 1}, {0, 0}, {255, 255, 255}, {0, 0, 0}},
        {{1, 1, 1}, {INT16_MAX, 0}, {255, 255, 255}, {0, 0, 0}},
        {{1, 1, -1}, {INT16_MAX, INT16_MAX}, {255, 255, 255}, {0, 0, 0}},
        {{1, 1, -1}, {INT16_MAX, INT16_MAX}, {255, 255, 255}, {0, 0, 0}},
        {{-1, 1, -1}, {0, INT16_MAX}, {255, 255, 255}, {0, 0, 0}},
        {{-1, 1, 1}, {0, 0}, {255, 255, 255}, {0, 0, 0}},

        // Bottom face (Y-)
        {{-1, -1, -1}, {0, 0}, {255, 255, 255}, {0, 0, 0}},
        {{1, -1, -1}, {INT16_MAX, 0}, {255, 255, 255}, {0, 0, 0}},
        {{1, -1, 1}, {INT16_MAX, INT16_MAX}, {255, 255, 255}, {0, 0, 0}},
        {{1, -1, 1}, {INT16_MAX, INT16_MAX}, {255, 255, 255}, {0, 0, 0}},
        {{-1, -1, 1}, {0, INT16_MAX}, {255, 255, 255}, {0, 0, 0}},
        {{-1, -1, -1}, {0, 0}, {255, 255, 255}, {0, 0, 0}},

        // Right face (X+)
        {{1, -1, 1}, {0, 0}, {255, 255, 255}, {0, 0, 0}},
        {{1, -1, -1}, {INT16_MAX, 0}, {255, 255, 255}, {0, 0, 0}},
        {{1, 1, -1}, {INT16_MAX, INT16_MAX}, {255, 255, 255}, {0, 0, 0}},
        {{1, 1, -1}, {INT16_MAX, INT16_MAX}, {255, 255, 255}, {0, 0, 0}},
        {{1, 1, 1}, {0, INT16_MAX}, {255, 255, 255}, {0, 0, 0}},
        {{1, -1, 1}, {0, 0}, {255, 255, 255}, {0, 0, 0}},

        // Left face (X-)
        {{-1, -1, -1}, {0, 0}, {255, 255, 255}, {0, 0, 0}},
        {{-1, -1, 1}, {INT16_MAX, 0}, {255, 255, 255}, {0, 0, 0}},
        {{-1, 1, 1}, {INT16_MAX, INT16_MAX}, {255, 255, 255}, {0, 0, 0}},
        {{-1, 1, 1}, {INT16_MAX, INT16_MAX}, {255, 255, 255}, {0, 0, 0}},
        {{-1, 1, -1}, {0, INT16_MAX}, {255, 255, 255}, {0, 0, 0}},
        {{-1, -1, -1}, {0, 0}, {255, 255, 255}, {0, 0, 0}}
};

CubeRenderer::CubeRenderer() {
    m_cloudVBO = (WorldVertex*)linearAlloc(sizeof(cubeVertices));
    memcpy(m_cloudVBO, cubeVertices, sizeof(cubeVertices));
}

CubeRenderer::~CubeRenderer() {
    linearFree(m_cloudVBO);
}

void CubeRenderer::Draw(int projUniform, C3D_Mtx* projectionview, World* world, mc::Vector3f position) {
    C3D_Mtx model;
    Mtx_Identity(&model);

    m_posX = position.x + 10 ;
    m_posY = position.y;
    m_posZ = position.z;

    Mtx_Translate(&model, m_posX, m_posY, m_posZ, true);
    Mtx_Scale(&model, 1.f, 1.f, 1.f);

    C3D_CullFace(GPU_CULL_BACK_CCW);

    C3D_AlphaTest(false, GPU_GREATER, 0);

    // Disable all texture units
    C3D_TexEnv* env = C3D_GetTexEnv(0);
    C3D_TexEnvInit(env);
    C3D_TexEnvSrc(env, C3D_Both, GPU_PRIMARY_COLOR, GPU_PRIMARY_COLOR, GPU_PRIMARY_COLOR);
    C3D_TexEnvFunc(env, C3D_Both, GPU_REPLACE);

    GSPGPU_FlushDataCache(m_cloudVBO, sizeof(cubeVertices));

    C3D_Mtx mvp;
    Mtx_Multiply(&mvp, projectionview, &model);

    C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, projUniform, &mvp);

    C3D_BufInfo* bufInfo = C3D_GetBufInfo();
    BufInfo_Init(bufInfo);
    BufInfo_Add(bufInfo, m_cloudVBO, sizeof(WorldVertex), 4, 0x3210);

    C3D_DrawArrays(GPU_TRIANGLES, 0, 36);
}