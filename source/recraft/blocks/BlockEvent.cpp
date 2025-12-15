#include "blocks/BlockEvents.h"

void BlockEvent_RandomTick(World* world, ChunkColumn* chunk, int x[], int y[], int z[]) {
	int k = 0;
	for (int i = 0; i < CLUSTER_PER_CHUNK; i++) {
		for (int j = 0; j < RANDOMTICKS_PER_CLUSTER; j++) {

            auto defaultPosition = mc::Vector3i(x[k], y[k] + i * CHUNK_SIZE, z[k]);

            Block block = chunk->GetBlock(defaultPosition);

            auto blockPosition = mc::Vector3i(
                    defaultPosition.x,
                    defaultPosition.y + 1,
                    defaultPosition.z
            );

            switch (block) {
				case Block_Dirt:
					if (!Block_Opaque(chunk->GetBlock(blockPosition),chunk->GetMetadata(blockPosition))) {
                        chunk->SetBlock(defaultPosition, Block_Grass);
					}
					break;
				case Block_Grass:
					if (Block_Opaque(chunk->GetBlock(blockPosition), chunk->GetMetadata(blockPosition))) {
                        chunk->SetBlock(defaultPosition, Block_Dirt);
					}
					break;
				case Block_Snow_Grass:
					if (Block_Opaque(chunk->GetBlock(blockPosition), chunk->GetMetadata(blockPosition))) {
                        chunk->SetBlock(defaultPosition, Block_Dirt);
					}
					break;
				case Block_Grass_Path:

					if (Block_Opaque(chunk->GetBlock(blockPosition), chunk->GetMetadata(blockPosition))) {
                        chunk->SetBlock(defaultPosition, Block_Dirt);
					}
					break;
				default:
					break;
			}
			k++;
		}
	}
}