#include "rendering/VBOCache.h"
#include <3ds.h>
#include <vector>
#include <algorithm>

static LightLock lock;

VBOCache::VBOCache() {
    LightLock_Init(&lock);
}

VBO_Block VBOCache::Alloc(size_t size) {
    LightLock_Lock(&lock);
    VBO_Block block;
    block.memory = linearAlloc(size);
    block.size = size;
    LightLock_Unlock(&lock);
    return block;
}

void VBOCache::Free(VBO_Block block) {
    if (block.size > 0 && block.memory != NULL) {
        LightLock_Lock(&lock);

        linearFree(block.memory);

        LightLock_Unlock(&lock);
    }
}
