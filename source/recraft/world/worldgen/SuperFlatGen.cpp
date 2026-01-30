#include "world/worldgen/SuperFlatGen.hpp"

void SuperFlatGen::Init(World* world) {
    m_world = world;
}

void SuperFlatGen::Generate(WorkQueue* queue, WorkerItem item, void* context) {
	for (int y = 0; y < 17; y++) {
        BlockID block = BlockID::Air;
		switch (y) {
			case 0:
				block = BlockID::Bedrock;
				break;
			case 1 ... 10:
				block = BlockID::Stone;
				break;
			case 11 ... 15:
				block = BlockID::Dirt;
				break;
			case 16:
				block = BlockID::Grass;
				break;
			default:
				block = BlockID::Air;
				break;
		}
		for (int x = 0; x < Chunk::Size; x++) {
			for (int z = 0; z < Chunk::Size; z++) {
                item.column->SetBlock(mc::Vector3i(x, y, z), block);
            }
        }
	}
}