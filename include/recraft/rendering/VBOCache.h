#pragma once

#include <cstdio>

typedef struct {
    size_t size;
    void* memory;
} VBO_Block;

class VBOCache {
public:
    VBOCache();
    ~VBOCache();


    VBO_Block Alloc(size_t size);
    void Free(VBO_Block block);
private:

    static int SortBySize(const void* a, const void* b);
};
