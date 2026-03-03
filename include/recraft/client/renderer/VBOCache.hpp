#pragma once

#include <stdio.h>

struct VBOBlock {
    size_t size;
    void* memory;
};

struct VBOUpdate {
    VBOBlock vbo, transparentVBO;
    int x, y, z;
    size_t vertices, transparentVertices;
    uint8_t delay;
    uint16_t visibility;
};

class VBOCache {
public:
    VBOCache();
    ~VBOCache() = default;

    VBOBlock Alloc(size_t size);
    void Free(VBOBlock block);
};
