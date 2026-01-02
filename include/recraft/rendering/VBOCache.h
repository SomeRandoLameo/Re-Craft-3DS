#pragma once

#include <stdio.h>

typedef struct {
    size_t size;
    void* memory;
} VBO_Block;

class VBOCache {
public:
    VBOCache();
    ~VBOCache() = default;

    VBO_Block Alloc(size_t size);
    void Free(VBO_Block block);
};
