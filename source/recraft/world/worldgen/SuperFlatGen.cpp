#include "world/worldgen/SuperFlatGen.h"

void SuperFlatGen::Init(World* world) {
    m_world = world;
}

void SuperFlatGen::Generate(WorkQueue* queue, WorkerItem item, void* context) {
	for (int y = 0; y < 17; y++) {
		Block block = Block_Air;
		switch (y) {
			case 0:
				block = Block_Bedrock;
				break;
			case 1 ... 10:
				block = Block_Stone;
				break;
			case 11 ... 15:
				block = Block_Dirt;
				break;
			case 16:
				block = Block_Grass;
				break;
			default:
				block = Block_Air;
				break;
		}
		for (int x = 0; x < CHUNK_SIZE; x++) {
			for (int z = 0; z < CHUNK_SIZE; z++) {
                item.column->SetBlock(mc::Vector3i(x, y, z), block);
            }
        }
	}
}