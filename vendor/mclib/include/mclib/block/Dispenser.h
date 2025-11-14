#pragma once

#include "../common/Nameable.h"
#include "BlockEntity.h"
#include "InventoryBlock.h"

#include <unordered_map>

namespace mc {
    namespace block {

        class Dispenser : public BlockEntity, public InventoryBlock, public Nameable {
        public:
            Dispenser(BlockEntityType type, Vector3i position) : BlockEntity(type, position) { }
            bool ImportNBT(nbt::NBT* nbt);
        };

    } // ns block
} // ns mc
