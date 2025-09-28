#pragma once

#include "../common/Nameable.h"
#include "BlockEntity.h"

#include <unordered_map>

namespace mc {
    namespace block {

        class EnchantmentTable : public BlockEntity, public Nameable {
        public:
            EnchantmentTable(BlockEntityType type, Vector3i position) : BlockEntity(type, position) { }
            bool ImportNBT(nbt::NBT* nbt);
        };

    } // ns block
} // ns mc
