#include "client/model/ModelX.hpp"
#include "block/Block.hpp"
#include <cmath>
static constexpr float kAngle0 =  M_PI / 4.f;
static constexpr float kAngle1 = -M_PI / 4.f;

static constexpr float kHalfW  = 0.499f;
static constexpr float kHeight = 1.0f;

void ModelX::emitPlane(Tessellator* t,
                       float cx, float cy, float cz,
                       float angleY,
                       float u0, float v0,
                       float u1, float v1,
                       float r, float g, float b)
{
    float dx = cosf(angleY) * kHalfW;
    float dz = sinf(angleY) * kHalfW;

    float p[4][3] = {
        { cx - dx, cy,           cz - dz }, // 0 bottom-left
        { cx - dx, cy + kHeight, cz - dz }, // 1 top-left
        { cx + dx, cy + kHeight, cz + dz }, // 2 top-right
        { cx + dx, cy,           cz + dz }  // 3 bottom-right
    };

    float uvs[4][2] = {
        {u1, v1},
        {u1, v0},
        {u0, v0},
        {u0, v1},
    };

    t->color(r, g, b);

    auto emitTri = [&](int a, int b, int c) {
        t->tex(uvs[a][0], uvs[a][1]);
        t->vertex(p[a][0], p[a][1], p[a][2]);

        t->tex(uvs[b][0], uvs[b][1]);
        t->vertex(p[b][0], p[b][1], p[b][2]);

        t->tex(uvs[c][0], uvs[c][1]);
        t->vertex(p[c][0], p[c][1], p[c][2]);
    };


    emitTri(0, 1, 2);
    emitTri(0, 2, 3);

    emitTri(3, 2, 1);
    emitTri(3, 1, 0);
}


void ModelX::Render(int projUniform, C3D_Mtx* vp,
                    float worldX, float worldY, float worldZ,
                    BlockID blockId, uint8_t metadata,
                    float r, float g, float b)
{
    C3D_Mtx model;
    Mtx_Identity(&model);
    Mtx_Translate(&model, worldX, worldY, worldZ, true);

    C3D_Mtx mvp;
    Mtx_Multiply(&mvp, vp, &model);
    C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, projUniform, &mvp);

    TextureMap* tm = BlockRegistry::GetTextureMapEx();
    C3D_TexBind(0, tm->GetTexture());

    C3D_AlphaTest(true, GPU_GEQUAL, 128);

    C3D_CullFace(GPU_CULL_NONE);

    //TODO: Flip UVs
    int16_t iconUV[2];
    BlockRegistry::GetInstance().GetTextureUV(blockId, metadata, Direction::Top, iconUV);

    const float inv = 1.0f / (float)TextureMap::UvPrecision;
    const float tileSize = (float)(TextureMap::UvPrecision / tm->GetMapTiles());

    float u1 = (float)iconUV[0] * inv;
    float v0 = (float)iconUV[1] * inv;
    float u0 = u1 + tileSize * inv;
    float v1 = v0 + tileSize * inv;

    uint8_t col[3];
    BlockRegistry::GetBlock(blockId)->getColor(metadata, Direction::Top, col);

    float fr = r * (col[0] / 255.0f);
    float fg = g * (col[1] / 255.0f);
    float fb = b * (col[2] / 255.0f);

    Tessellator* t = Tessellator::getInstance();
    t->begin(TESS_TRIANGLES);

    emitPlane(t, 0.5f, 0.0f, 0.5f, kAngle0, u0, v0, u1, v1, fr, fg, fb);
    emitPlane(t, 0.5f, 0.0f, 0.5f, kAngle1, u0, v0, u1, v1, fr, fg, fb);

    t->flush();

    C3D_CullFace(GPU_CULL_BACK_CCW);
    C3D_AlphaTest(false, GPU_GREATER, 0);
}