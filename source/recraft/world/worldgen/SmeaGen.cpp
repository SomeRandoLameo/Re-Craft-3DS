#include "world/worldgen/SmeaGen.hpp"

extern "C" {
#include <sino/sino.h>

}

void SmeaGen::Init(World* world) {
    m_world = world;
}

// based of https://github.com/smealum/3dscraft/blob/master/source/generation.c
void SmeaGen::Generate(WorkQueue* queue, WorkerItem item, void* context) {
	for (int x = 0; x < Chunk::Size; x++) {
		for (int z = 0; z < Chunk::Size; z++) {
			float px = (float)(x + item.column->x * Chunk::Size);
			float pz = (float)(z + item.column->z * Chunk::Size);

			const int smeasClusterSize = 8;
			const int smeasChunkHeight = 16;
			int height = (int)(sino_2d((px) / (smeasClusterSize * 4), (pz) / (smeasClusterSize * 4)) * smeasClusterSize) +
				     (smeasChunkHeight * smeasClusterSize / 2);

			for (int y = 0; y < height - 3; y++) {
                item.column->SetBlock(mc::Vector3i(x, y, z), Block::Stone);
			}

			for (int y = height - 3; y < height; y++) {
                item.column->SetBlock(mc::Vector3i(x, y, z), Block::Dirt);
			}

            item.column->SetBlock(mc::Vector3i(x, height, z), Block::Grass);
		}
	}
}