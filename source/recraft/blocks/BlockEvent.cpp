#include "blocks/BlockEvents.h"

void BlockEvent_RandomTick(World* world, ChunkColumn* chunk, int x[], int y[], int z[]) {
	int k = 0;
	for (int i = 0; i < CLUSTER_PER_CHUNK; i++) {
		for (int j = 0; j < RANDOMTICKS_PER_CLUSTER; j++) {

            auto defaultPosition = mc::Vector3i(x[k], y[k] + i * CHUNK_SIZE, z[k]);

            mc::block::BlockPtr block = chunk->GetBlock(defaultPosition);

            auto blockPosition = mc::Vector3i(
                    defaultPosition.x,
                    defaultPosition.y + 1,
                    defaultPosition.z
            );

            if(block->GetName() == "minecraft:dirt"){
                if (!chunk->GetBlock(blockPosition)->IsOpaque()) {
                    chunk->SetBlock(defaultPosition, mc::block::BlockRegistry::GetInstance()->GetBlock("minecraft:grass_block"));
                }
            }
            if(block->GetName() == "minecraft:grass_block"){
                if (!chunk->GetBlock(blockPosition)->IsOpaque()) {
                    chunk->SetBlock(defaultPosition, mc::block::BlockRegistry::GetInstance()->GetBlock("minecraft:dirt"));
                }
            }
            if(block->GetName() == "minecraft:grass_path"){
                if (!chunk->GetBlock(blockPosition)->IsOpaque()) {
                    chunk->SetBlock(defaultPosition, mc::block::BlockRegistry::GetInstance()->GetBlock("minecraft:dirt"));
                }
            }

			k++;
		}
	}
}