#pragma once

#include <stdio.h>

typedef struct {
    size_t size;
    void* memory;
} VBOBlock;

class VBOCache {
public:
    VBOCache();
    ~VBOCache() = default;

    VBOBlock Alloc(size_t size);
    void Free(VBOBlock block);
};
