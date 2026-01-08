#include "blocks/BlockEvents.h"

void BlockEvent_RandomTick(World* world, ChunkColumnPtr column, int x[], int y[], int z[]) {
	int k = 0;
	for (int i = 0; i < ChunkColumn::ChunksPerColumn; i++) {
		for (int j = 0; j < RANDOMTICKS_PER_CLUSTER; j++) {

            auto defaultPosition = mc::Vector3i(x[k], y[k] + i * Chunk::Size, z[k]);

            Block block = column->GetBlock(defaultPosition);

            auto blockPosition = mc::Vector3i(
                    defaultPosition.x,
                    defaultPosition.y + 1,
                    defaultPosition.z
            );

            switch (block) {
            case Block::Dirt:
					if (!Block_Opaque(column->GetBlock(blockPosition),column->GetMetadata(blockPosition))) {
                        column->SetBlock(defaultPosition, Block::Grass);
					}
					break;
            case Block::Grass:
					if (Block_Opaque(column->GetBlock(blockPosition), column->GetMetadata(blockPosition))) {
                        column->SetBlock(defaultPosition, Block::Dirt);
					}
					break;
            case Block::Snow_Grass:
					if (Block_Opaque(column->GetBlock(blockPosition), column->GetMetadata(blockPosition))) {
                        column->SetBlock(defaultPosition, Block::Dirt);
					}
					break;
            case Block::Grass_Path:
					if (Block_Opaque(column->GetBlock(blockPosition), column->GetMetadata(blockPosition))) {
                        column->SetBlock(defaultPosition, Block::Dirt);
					}
					break;
				default:
					break;
			}
			k++;
		}
	}
}