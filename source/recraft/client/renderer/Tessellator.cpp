#include "client/renderer/Tessellator.hpp"
#include <cstring>
#include <cmath>
#include <algorithm>

static inline int16_t encodeUV(float uv) {
    int v = (int)(uv * 32768.0f);
    if (v >  32767) v =  32767;
    if (v < -32768) v = -32768;
    return (int16_t)v;
}

static inline void encodePos(float pos, int16_t& ipart, uint8_t& fpart) {
    float fl = floorf(pos);
    ipart    = (int16_t)(int)fl;
    fpart    = (uint8_t)(int)((pos - fl) * 255.0f + 0.5f);
}

Tessellator* Tessellator::s_instance         = nullptr;
int            Tessellator::s_posIdx            = 0;
int            Tessellator::s_texIdx            = 1;
int            Tessellator::s_colIdx            = 2;
int            Tessellator::s_nrmIdx            = 3;
bool           Tessellator::s_attribsConfigured = false;

Tessellator* Tessellator::getInstance() {
    return s_instance;
}

Tessellator* Tessellator::getUniqueInstance(int n) {
    return new Tessellator(n);
}

void Tessellator::createInstance(int n) {
    if (!s_instance) s_instance = new Tessellator(n);
}

void Tessellator::destroyInstance() {
    delete s_instance;
    s_instance = nullptr;
}

void Tessellator::configureAttribs(int p, int t, int c, int n) {
    s_posIdx=p;
    s_texIdx=t;
    s_colIdx=c;
    s_nrmIdx=n;
    s_attribsConfigured=true;
}

// ── ctor/dtor ─────────────────────────────────────────────────────────────────
Tessellator::Tessellator(int maxVerts)
        : count(0), _bufCap(maxVerts), _bufPos(0), _quadVerts(0)
        , _u(0), _v(0), _col(0xFFFFFFFF)
        , _hasColor(false), _hasTexture(false), _noColor(false), _anyColor(false)
        , _mode(TESS_QUADS)
        , _xo(0), _yo(0), _zo(0)
{
    bounds.reset();
    _buf = (TessVertex*)linearAlloc(sizeof(TessVertex) * maxVerts);
    if (!_buf) {
        _bufCap = maxVerts / 2;
        _buf = (TessVertex*)linearAlloc(sizeof(TessVertex) * _bufCap);
    }
}

Tessellator::~Tessellator() {
    if (_buf) {
        linearFree(_buf);
        _buf = nullptr;
    }
}

// ── internal ──────────────────────────────────────────────────────────────────
void Tessellator::resetState() {
    _hasColor   = false;
    _hasTexture = false;
    _noColor    = false;
    _quadVerts  = 0;
    // _mode is NOT reset here - set by begin(mode) after this call
}

void Tessellator::clearBuffer() {
    _bufPos    = 0;
    _quadVerts = 0;
    _anyColor  = false;
    count      = 0;
    bounds.reset();
}

// ── begin / end / flush ───────────────────────────────────────────────────────

void Tessellator::begin() {
    begin(TESS_QUADS);
}

void Tessellator::begin(int mode) {
    resetState();  // resets color/texture/noColor/quadVerts
    _mode = mode;  // set AFTER resetState so it isn't clobbered
}

// end() – closes the current primitive, writes triangles into _buf.
// Does NOT draw anything. Call flush() once per frame to actually draw.
void Tessellator::end() {
    // Fill white for any verts in this primitive that had no color
    // (We track _anyColor per-frame and fix up in flush if needed)
}

// flush() – one draw call for everything accumulated this frame
void Tessellator::flush() {
    if (_bufPos == 0) return;

    // If color was never set for any vertex, fill opaque white
    if (!_anyColor)
        for (int i = 0; i < _bufPos; i++)
            _buf[i].r = _buf[i].g = _buf[i].b = _buf[i].a = 255;

    // BufInfo
    C3D_BufInfo* bi = C3D_GetBufInfo();
    BufInfo_Init(bi);
    BufInfo_Add(bi, _buf, sizeof(TessVertex), 4, 0x3210);

    // AttrInfo – order matches TessVertex, citro3d accumulates offsets:
    //   GPU_SHORT         ×3 → offset  0  (6 bytes)
    //   GPU_SHORT         ×2 → offset  6  (4 bytes)
    //   GPU_UNSIGNED_BYTE ×4 → offset 10  (4 bytes)
    //   GPU_UNSIGNED_BYTE ×4 → offset 14  (4 bytes)
    C3D_AttrInfo* ai = C3D_GetAttrInfo();
    AttrInfo_Init(ai);
    AttrInfo_AddLoader(ai, s_posIdx, GPU_SHORT,         3);
    AttrInfo_AddLoader(ai, s_texIdx, GPU_SHORT,         2);
    AttrInfo_AddLoader(ai, s_colIdx, GPU_UNSIGNED_BYTE, 4);
    AttrInfo_AddLoader(ai, s_nrmIdx, GPU_UNSIGNED_BYTE, 4);

    C3D_DrawArrays(GPU_TRIANGLES, 0, _bufPos);

    clearBuffer();
}

// ── state setters ─────────────────────────────────────────────────────────────
void Tessellator::tex(float u, float v) {
    _hasTexture=true;
    _u=u;
    _v=v;
}

void Tessellator::color(float r,float g,float b) {
    color(
            (int)(r*255),
            (int)(g*255),
            (int)(b*255),
            255
    );
}

void Tessellator::color(float r,float g,float b,float a) {
    color(
            (int)(r*255),
            (int)(g*255),
            (int)(b*255),
            (int)(a*255)
    );
}

void Tessellator::color(int r,int g,int b) {
    color(r, g, b, 255);
}

void Tessellator::color(int r,int g,int b,int a) {
    if (_noColor) return;
    r=std::max(0,std::min(255,r));
    g=std::max(0,std::min(255,g));
    b=std::max(0,std::min(255,b));
    a=std::max(0,std::min(255,a));
    _hasColor = true;
    _anyColor = true;
    _col = (r<<24)|(g<<16)|(b<<8)|a;
}
void Tessellator::color(int p) {
    color(
            (p>>16)&0xFF,
            (p>>8)&0xFF,
            p&0xFF,
            255
    );
}

void Tessellator::color(int p,int a) {
    color((p>>16)&0xFF,(p>>8)&0xFF,p&0xFF,a);
}

void Tessellator::noColor(){
    _noColor=true;
}

void Tessellator::normal(float,float,float) {

}

void Tessellator::offset(float x,float y,float z) {
    _xo=x;
    _yo=y;
    _zo=z;
}

void Tessellator::addOffset(float x,float y,float z) {
    _xo+=x;
    _yo+=y;
    _zo+=z;
}

void Tessellator::vertex(float x, float y, float z) {
    x += _xo; y += _yo; z += _zo;
    bounds.addVert(x, y, z);
    count++;

    TessVertex tv;
    encodePos(x, tv.x, tv.fx);
    encodePos(y, tv.y, tv.fy);
    encodePos(z, tv.z, tv.fz);
    tv.fw = 0;
    tv.u  = encodeUV(_hasTexture ? _u : 0.0f);
    tv.v  = encodeUV(_hasTexture ? _v : 0.0f);

    if (_hasColor) {
        tv.r = (uint8_t)((_col>>24)&0xFF);
        tv.g = (uint8_t)((_col>>16)&0xFF);
        tv.b = (uint8_t)((_col>> 8)&0xFF);
        tv.a = (uint8_t)( _col     &0xFF);
    } else {
        tv.r = tv.g = tv.b = tv.a = 255;
    }

    if (_mode == TESS_QUADS) {
        _quad[_quadVerts++] = tv;
        if (_quadVerts == 4) {
            if (_bufPos + 6 <= _bufCap) {
                _buf[_bufPos++]=_quad[0]; _buf[_bufPos++]=_quad[1]; _buf[_bufPos++]=_quad[2];
                _buf[_bufPos++]=_quad[0]; _buf[_bufPos++]=_quad[2]; _buf[_bufPos++]=_quad[3];
            }
            _quadVerts = 0;
        }
    } else {
        if (_bufPos + 1 <= _bufCap)
            _buf[_bufPos++] = tv;
    }
}

void Tessellator::vertexUV(float x,float y,float z,float u,float v) {
    tex(u,v);
    vertex(x,y,z);
}

bool Tessellator::hasMaxVertices() const {
    return (_bufPos*(int)sizeof(TessVertex)) > 60*1024;
}