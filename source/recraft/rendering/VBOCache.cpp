#include "rendering/VBOCache.h"
#include <3ds.h>

static LightLock lock;

VBOCache::VBOCache() {
    LightLock_Init(&lock);
}

VBO_Block VBOCache::Alloc(size_t size) {
    VBO_Block block;
    block.memory = linearAlloc(size);
    block.size = size;
    return block;
}

void VBOCache::Free(VBO_Block block) {
    if (block.size > 0 && block.memory != NULL) {
        linearFree(block.memory);
    }
}
