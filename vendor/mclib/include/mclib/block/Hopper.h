#pragma once

#include "../common/Nameable.h"
#include "BlockEntity.h"
#include "InventoryBlock.h"

#include <unordered_map>

namespace mc {
    namespace block {

        class Hopper : public BlockEntity, public InventoryBlock, public Nameable {
        private:
            s32 m_TransferCooldown;

        public:
            Hopper(BlockEntityType type, Vector3i position) : BlockEntity(type, position) { }
            bool ImportNBT(nbt::NBT* nbt);

            s32 GetTransferCooldown() const noexcept { return m_TransferCooldown; }
        };

    } // ns block
} // ns mc
