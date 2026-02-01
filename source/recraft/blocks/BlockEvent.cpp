#include "blocks/BlockEvents.hpp"

void BlockEvent_RandomTick(World* world, ChunkColumnPtr column, int x[], int y[], int z[]) {
	int k = 0;
	for (int i = 0; i < ChunkColumn::ChunksPerColumn; i++) {
		for (int j = 0; j < RANDOMTICKS_PER_CLUSTER; j++) {

            auto defaultPosition = mc::Vector3i(x[k], y[k] + i * Chunk::Size, z[k]);

            BlockPtr block = column->GetBlock(defaultPosition);

            auto blockPosition = mc::Vector3i(
                    defaultPosition.x,
                    defaultPosition.y + 1,
                    defaultPosition.z
            );

            switch (block->GetID()) {
            case BlockID::Dirt:
					if (!column->GetBlock(blockPosition)->isOpaque()) {
                        column->SetBlockID(defaultPosition, BlockID::Grass);
					}
					break;
            case BlockID::Grass:
					if (column->GetBlock(blockPosition)->isOpaque()) {
                        column->SetBlockID(defaultPosition, BlockID::Dirt);
					}
					break;
            case BlockID::Snow_Grass:
					if (column->GetBlock(blockPosition)->isOpaque()) {
                        column->SetBlockID(defaultPosition, BlockID::Dirt);
					}
					break;
            case BlockID::Grass_Path:
					if (column->GetBlock(blockPosition)->isOpaque()) {
                        column->SetBlockID(defaultPosition, BlockID::Dirt);
					}
					break;
				default:
					break;
			}
			k++;
		}
	}
}