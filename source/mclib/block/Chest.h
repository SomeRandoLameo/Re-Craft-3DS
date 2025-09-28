#pragma once

#include "../common/Nameable.h"
#include "BlockEntity.h"
#include "InventoryBlock.h"

#include <unordered_map>

namespace mc {
    namespace block {

        class Chest : public BlockEntity, public InventoryBlock, public Nameable {
        public:
            Chest(BlockEntityType type, Vector3i position) : BlockEntity(type, position) { }
            bool ImportNBT(nbt::NBT* nbt);
        };

    } // ns block
} // ns mc
