#pragma once

#include "../common/Nameable.h"
#include "BlockEntity.h"

#include <unordered_map>

namespace mc {
    namespace block {

        class EnchantmentTable : public BlockEntity, public Nameable {
        public:
            MCLIB_API EnchantmentTable(BlockEntityType type, Vector3i position) : BlockEntity(type, position) { }
            MCLIB_API bool ImportNBT(nbt::NBT* nbt);
        };

    } // ns block
} // ns mc
