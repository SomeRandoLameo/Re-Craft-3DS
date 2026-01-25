#pragma once

struct VBOBlock {
    size_t size;
    void*  memory;
};

struct VBOUpdate {
    VBOBlock vbo, transparentVBO;
    int      x, y, z;
    size_t   vertices, transparentVertices;
    u8       delay;
    u16      visibility;
};

class VBOCache {
public:
    VBOCache();
    ~VBOCache() = default;

    VBOBlock Alloc(size_t size);
    void     Free(VBOBlock block);
};
