#include "client/renderer/ChestRenderer.hpp"
#include "misc/NumberUtils.hpp"

static constexpr float ATLAS = 64.0f;

static inline void texelUV(int tx, int ty, int tw, int th,
                           float& u0, float& v0, float& u1, float& v1)
{
    u0 = tx / ATLAS;
    u1 = (tx + tw) / ATLAS;

    v0 = 1.0f - (ty + th) / ATLAS;
    v1 = 1.0f - ty / ATLAS;
}

ChestRenderer::ChestRenderer()
{
    m_chestTexture.Load("romfs:/assets/minecraft/textures/entity/chest/normal.png");
}

ChestRenderer::~ChestRenderer()
{
    m_chestTexture.Unload();
}

void ChestRenderer::emitFace(Tessellator* t,
                             float x0, float y0, float z0,
                             float x1, float y1, float z1,
                             int axis, bool positiveDir,
                             float u0, float v0, float u1, float v1,
                             float r, float g, float b)
{
    t->color(r, g, b);

    float p[4][3];

    if (axis == 1) { // Y (top/bottom)
        float y = positiveDir ? y1 : y0;
        p[0][0]=x0; p[0][1]=y; p[0][2]=z0;
        p[1][0]=x0; p[1][1]=y; p[1][2]=z1;
        p[2][0]=x1; p[2][1]=y; p[2][2]=z1;
        p[3][0]=x1; p[3][1]=y; p[3][2]=z0;
    }
    else if (axis == 2) { // Z (front/back)
        float z = positiveDir ? z1 : z0;
        p[0][0]=x0; p[0][1]=y0; p[0][2]=z;
        p[1][0]=x0; p[1][1]=y1; p[1][2]=z;
        p[2][0]=x1; p[2][1]=y1; p[2][2]=z;
        p[3][0]=x1; p[3][1]=y0; p[3][2]=z;

        // Mirror back face (+Z) only
        if (positiveDir) {
            std::swap(u0, u1);
        }
    }
    else { // X (left/right)
        float x = positiveDir ? x1 : x0;
        p[0][0]=x; p[0][1]=y0; p[0][2]=z0;
        p[1][0]=x; p[1][1]=y1; p[1][2]=z0;
        p[2][0]=x; p[2][1]=y1; p[2][2]=z1;
        p[3][0]=x; p[3][1]=y0; p[3][2]=z1;

        // Flip left face
        if (!positiveDir) {
            std::swap(u0, u1);
        }
    }

    float uvs[4][2] = {
        {u0, v1},
        {u0, v0},
        {u1, v0},
        {u1, v1},
    };

    auto emitTri = [&](int a, int b, int c)
    {
        t->tex(uvs[a][0], uvs[a][1]); t->vertex(p[a][0], p[a][1], p[a][2]);
        t->tex(uvs[b][0], uvs[b][1]); t->vertex(p[b][0], p[b][1], p[b][2]);
        t->tex(uvs[c][0], uvs[c][1]); t->vertex(p[c][0], p[c][1], p[c][2]);
    };

    // Normal calculation
    float ux = p[1][0] - p[0][0];
    float uy = p[1][1] - p[0][1];
    float uz = p[1][2] - p[0][2];

    float vx = p[2][0] - p[0][0];
    float vy = p[2][1] - p[0][1];
    float vz = p[2][2] - p[0][2];

    float nx = uy * vz - uz * vy;
    float ny = uz * vx - ux * vz;
    float nz = ux * vy - uy * vx;

    float ex = 0, ey = 0, ez = 0;
    if (axis == 0) ex = positiveDir ? 1.f : -1.f;
    if (axis == 1) ey = positiveDir ? 1.f : -1.f;
    if (axis == 2) ez = positiveDir ? 1.f : -1.f;

    float dot = nx*ex + ny*ey + nz*ez;

    if (dot < 0.0f) {
        emitTri(0, 2, 1);
        emitTri(0, 3, 2);
    } else {
        emitTri(0, 1, 2);
        emitTri(0, 2, 3);
    }
}

void ChestRenderer::emitBox(Tessellator* t,
                            float ox, float oy, float oz,
                            float w,  float h,  float d,
                            int texX, int texY,
                            float r, float g, float b)
{
    float x0 = ox, x1 = ox + w;
    float y0 = oy, y1 = oy + h;
    float z0 = oz, z1 = oz + d;

    int iw = (int)w, ih = (int)h, id_ = (int)d;

    float u0, v0, u1, v1;

    // Top
    texelUV(texX + id_, texY, iw, id_, u0, v0, u1, v1);
    emitFace(t, x0,y0,z0, x1,y1,z1, 1, true,  u0,v0,u1,v1, r,g,b);

    // Bottom
    texelUV(texX + id_ + iw, texY, iw, id_, u0, v0, u1, v1);
    emitFace(t, x0,y0,z0, x1,y1,z1, 1, false, u0,v0,u1,v1, r*0.5f,g*0.5f,b*0.5f);

    // Left (-X)
    texelUV(texX, texY + id_, id_, ih, u0, v0, u1, v1);
    emitFace(t, x0,y0,z0, x1,y1,z1, 0, false, u0,v0,u1,v1, r*0.9f,g*0.9f,b*0.9f);

    // Right (+X)
    texelUV(texX + id_ + iw, texY + id_, id_, ih, u0, v0, u1, v1);
    emitFace(t, x0,y0,z0, x1,y1,z1, 0, true,  u0,v0,u1,v1, r*0.9f,g*0.9f,b*0.9f);

    // Front (-Z)
    texelUV(texX + id_, texY + id_, iw, ih, u0, v0, u1, v1);
    emitFace(t, x0,y0,z0, x1,y1,z1, 2, false, u0,v0,u1,v1, r,g,b);

    // Back (+Z)
    texelUV(texX + id_ + iw + id_, texY + id_, iw, ih, u0, v0, u1, v1);
    emitFace(t, x0,y0,z0, x1,y1,z1, 2, true,  u0,v0,u1,v1, r*0.8f,g*0.8f,b*0.8f);
}

// ---------------------------------------------------------------------------
void ChestRenderer::Draw(int projUniform, C3D_Mtx* vp,
                         float worldX, float worldY, float worldZ)
{
    C3D_Mtx model;
    Mtx_Identity(&model);

    Mtx_Translate(&model,
                  worldX + 1.0f/16.0f,
                  worldY,
                  worldZ + 1.0f/16.0f,
                  true);

    Mtx_Scale(&model, 1.0f/16.0f, 1.0f/16.0f, 1.0f/16.0f);

    C3D_Mtx mvp;
    Mtx_Multiply(&mvp, vp, &model);
    C3D_FVUnifMtx4x4(GPU_VERTEX_SHADER, projUniform, &mvp);

    m_chestTexture.Bind();

    C3D_TexEnv* env = C3D_GetTexEnv(0);
    C3D_TexEnvInit(env);
    C3D_TexEnvSrc(env, C3D_Both, GPU_TEXTURE0, GPU_PRIMARY_COLOR, (GPU_TEVSRC)0);
    C3D_TexEnvFunc(env, C3D_Both, GPU_MODULATE);

    C3D_AlphaTest(true, GPU_GEQUAL, 128);
    C3D_CullFace(GPU_CULL_BACK_CCW);

    Tessellator* t = Tessellator::getInstance();
    t->begin(TESS_TRIANGLES);

    //chest
    emitBox(t, 0,0,0, 14,10,14, 0,19, 1,1,1);
    emitBox(t, 0,10,0, 14,5,14, 0,0, 1,1,1);

    // lock
    emitBox(t, 6.0f, 8.0f, 14.0f, 2.0f, 4.0f, 1.0f, 0,0, 1,1,1);

    t->flush();

    C3D_AlphaTest(false, GPU_GREATER, 0);
}