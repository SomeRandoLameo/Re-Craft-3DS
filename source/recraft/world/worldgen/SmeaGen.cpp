#include "world/worldgen/SmeaGen.h"

extern "C" {
#include <sino/sino.h>

}

void SmeaGen::Init(World* world) {
    m_world = world;
}

// based of https://github.com/smealum/3dscraft/blob/master/source/generation.c
void SmeaGen::Generate(WorkQueue* queue, WorkerItem item, void* context) {
	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int z = 0; z < CHUNK_SIZE; z++) {
			float px = (float)(x + item.chunk->x * CHUNK_SIZE);
			float pz = (float)(z + item.chunk->z * CHUNK_SIZE);

			const int smeasClusterSize = 8;
			const int smeasChunkHeight = 16;
			int height = (int)(sino_2d((px) / (smeasClusterSize * 4), (pz) / (smeasClusterSize * 4)) * smeasClusterSize) +
				     (smeasChunkHeight * smeasClusterSize / 2);

			for (int y = 0; y < height - 3; y++) {
                item.chunk->SetBlock(mc::Vector3i(x, y, z), mc::block::BlockRegistry::GetInstance()->GetBlock("minecraft:stone"));
			}
			for (int y = height - 3; y < height; y++) {
                item.chunk->SetBlock(mc::Vector3i(x, y, z), mc::block::BlockRegistry::GetInstance()->GetBlock("minecraft:dirt"));
			}
            item.chunk->SetBlock(mc::Vector3i(x, height, z), mc::block::BlockRegistry::GetInstance()->GetBlock("minecraft:grass_block"));
		}
	}
}