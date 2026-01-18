#pragma once

#include <stdio.h>

typedef struct {
    size_t size;
    void* memory;
} VBOBlock;

typedef struct {
    VBOBlock vbo, transparentVBO;
    int x, y, z;
    size_t vertices, transparentVertices;
    uint8_t delay;
    uint16_t visibility;
} VBOUpdate;

class VBOCache {
public:
    VBOCache();
    ~VBOCache() = default;

    VBOBlock Alloc(size_t size);
    void Free(VBOBlock block);
};
