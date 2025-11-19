#include "rendering/VBOCache.h"
#include <3ds.h>
#include <vector>
#include <algorithm>

static std::vector<VBO_Block> freedBlocks;

static LightLock lock;

VBOCache::VBOCache() {
	freedBlocks.clear();
	LightLock_Init(&lock);
}
VBOCache::~VBOCache() {
	for (auto& block : freedBlocks) {
		linearFree(block.memory);
	}
	freedBlocks.clear();
}

VBO_Block VBOCache::Alloc(size_t size) {
	LightLock_Lock(&lock);

	for (size_t i = 0; i < freedBlocks.size(); ++i) {
		VBO_Block& block = freedBlocks[i];

		if (size <= block.size && block.size - size <= 2048) {
			VBO_Block result = block;

			freedBlocks.erase(freedBlocks.begin() + i);

			LightLock_Unlock(&lock);
			return result;
		}
	}

	VBO_Block block;
	block.memory = linearAlloc(size);
	block.size = size;
	LightLock_Unlock(&lock);
	return block;
}

int VBOCache::SortBySize(const void* a, const void* b) {
    return ((VBO_Block*)b)->size - ((VBO_Block*)a)->size;
}

void VBOCache::Free(VBO_Block block) {
	if (block.size > 0 && block.memory != NULL) {
		LightLock_Lock(&lock);

		freedBlocks.push_back(block);

		std::sort(
				freedBlocks.begin(),
				freedBlocks.end(),
				[](const VBO_Block& a, const VBO_Block& b) {
					return b.size < a.size;
				}
		);

		LightLock_Unlock(&lock);
	}
}
