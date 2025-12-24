#include "world/worldgen/SuperFlatGen.h"

void SuperFlatGen::Init(World* world) {
    m_world = world;
}

void SuperFlatGen::Generate(WorkQueue* queue, WorkerItem item, void* context) {
	for (int y = 0; y < 17; y++) {
		mc::block::BlockPtr block;
		switch (y) {
			case 0:
				block = mc::block::BlockRegistry::GetInstance()->GetBlock("minecraft:bedrock");
				break;
			case 1 ... 10:
				block = mc::block::BlockRegistry::GetInstance()->GetBlock("minecraft:stone");
				break;
			case 11 ... 15:
                block = mc::block::BlockRegistry::GetInstance()->GetBlock("minecraft:dirt");
				break;
			case 16:
				block = mc::block::BlockRegistry::GetInstance()->GetBlock("minecraft:grass_block");
				break;
			default:
				block = mc::block::BlockRegistry::GetInstance()->GetBlock("minecraft:air");
				break;
		}
		for (int x = 0; x < CHUNK_SIZE; x++) {
			for (int z = 0; z < CHUNK_SIZE; z++) {
                item.chunk->SetBlock(mc::Vector3i(x, y, z), block);
            }
        }
	}
}