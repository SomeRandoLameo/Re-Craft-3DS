#include "rendering/VBOCache.hpp"
#include <3ds.h>

static LightLock lock;

VBOCache::VBOCache() {
    LightLock_Init(&lock);
}

VBOBlock VBOCache::Alloc(size_t size) {
    VBOBlock block;
    block.memory = linearAlloc(size);
    block.size   = size;
    return block;
}

void VBOCache::Free(VBOBlock block) {
    if (block.size > 0 && block.memory != NULL) {
        linearFree(block.memory);
    }
}
