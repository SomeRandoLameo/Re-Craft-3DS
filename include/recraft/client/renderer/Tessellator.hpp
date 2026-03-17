#pragma once

#include <3ds.h>
#include <citro3d.h>
#include <stdint.h>
#include <float.h>

// ── Vertex layout (18 bytes) ──────────────────────────────────────────────────
//  offset  0   int16×3   v0 in_xyz   position integer part
//  offset  6   int16×2   v1 in_uv    UV × 32768
//  offset 10   uint8×4   v2 in_rgb   color 0-255
//  offset 14   uint8×4   v3 in_fxyz  position fraction×255 + pad
struct TessVertex
{
    int16_t  x,  y,  z;
    int16_t  u,  v;
    uint8_t  r,  g,  b,  a;
    uint8_t  fx, fy, fz, fw;
};
static_assert(sizeof(TessVertex) == 18, "TessVertex must be 18 bytes");

#define TESS_QUADS     0
#define TESS_TRIANGLES 1

class Tessellator
{
public:
    class Bounds {
    public:
        void reset() {
            bb[0] = bb[1] = bb[2] =  FLT_MAX;
            bb[3] = bb[4] = bb[5] = -FLT_MAX;
        }
        void addVert(float x, float y, float z) {
            if (x < bb[0]) bb[0] = x;
            if (y < bb[1]) bb[1] = y;
            if (z < bb[2]) bb[2] = z;
            if (x > bb[3]) bb[3] = x;
            if (y > bb[4]) bb[4] = y;
            if (z > bb[5]) bb[5] = z;
        }
        void addBounds(const Bounds& o) {
            if (o.bb[0] < bb[0]) bb[0] = o.bb[0];
            if (o.bb[1] < bb[1]) bb[1] = o.bb[1];
            if (o.bb[2] < bb[2]) bb[2] = o.bb[2];
            if (o.bb[3] > bb[3]) bb[3] = o.bb[3];
            if (o.bb[4] > bb[4]) bb[4] = o.bb[4];
            if (o.bb[5] > bb[5]) bb[5] = o.bb[5];
        }
        float bb[6];
    };

    static Tessellator* getInstance();
    static void           createInstance(int maxVertices = 8192);
    static void           destroyInstance();
    static Tessellator* getUniqueInstance(int maxVertices = 8192);

    // Call once after C3D_BindProgram.
    // For vshader.v.pica: configureAttribs(0, 1, 2, 3)
    static void configureAttribs(int posReg, int texReg, int colReg, int nrmReg);

    // begin() – start a new primitive batch (does NOT reset the whole buffer)
    void begin();
    void begin(int mode);

    // end() – finalise the current primitive into the buffer (NO draw call)
    void end();

    // flush() – draw everything accumulated since the last flush(), then clear
    //           Call once per frame after all begin()/end() pairs.
    void flush();

    void tex   (float u, float v);
    void color (float r, float g, float b);
    void color (float r, float g, float b, float a);
    void color (int r,   int g,   int b);
    void color (int r,   int g,   int b,   int a);
    void color (int packed);
    void color (int packed, int alpha);
    void noColor();
    void normal(float x, float y, float z);

    void vertex  (float x, float y, float z);
    void vertexUV(float x, float y, float z, float u, float v);

    void offset   (float xo, float yo, float zo);
    void addOffset(float x,  float y,  float z);

    bool hasMaxVertices() const;

    const TessVertex* bufPtr() const { return _buf; }
    int               bufPos() const { return _bufPos; }

    int    count;
    Bounds bounds;

    ~Tessellator();

private:
    explicit Tessellator(int maxVertices);
    Tessellator(const Tessellator&)            = delete;
    Tessellator& operator=(const Tessellator&) = delete;

    void resetState();   // resets per-primitive state, NOT the buffer
    void clearBuffer();  // resets the vertex buffer (called by flush)

    TessVertex* _buf;
    int         _bufCap;
    int         _bufPos;    // next free slot in _buf

    TessVertex  _quad[4];
    int         _quadVerts;

    float   _u, _v;
    int     _col;
    bool    _hasColor, _hasTexture, _noColor;
    bool    _anyColor;  // true if ANY vertex in the current buffer has color set
    int     _mode;
    float   _xo, _yo, _zo;

    static Tessellator* s_instance;
    static int  s_posIdx, s_texIdx, s_colIdx, s_nrmIdx;
    static bool s_attribsConfigured;
};
