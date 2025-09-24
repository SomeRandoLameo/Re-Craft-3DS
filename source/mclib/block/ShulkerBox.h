#pragma once

#include "../common/Nameable.h"
#include "BlockEntity.h"
#include "InventoryBlock.h"

#include <unordered_map>

namespace mc {
    namespace block {

        class ShulkerBox : public BlockEntity, public InventoryBlock, public Nameable {
        public:
            MCLIB_API ShulkerBox(BlockEntityType type, Vector3i position) : BlockEntity(type, position) { }
            MCLIB_API bool ImportNBT(nbt::NBT* nbt);
        };

    } // ns block
} // ns mc
