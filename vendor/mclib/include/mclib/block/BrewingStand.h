#pragma once

#include "../common/Nameable.h"
#include "BlockEntity.h"
#include "InventoryBlock.h"

#include <unordered_map>

namespace mc {
    namespace block {

        class BrewingStand : public BlockEntity, public InventoryBlock, public Nameable {
        private:
            s32 m_BrewTime;
            u8 m_Fuel;

        public:
            BrewingStand(BlockEntityType type, Vector3i position) : BlockEntity(type, position) { }
            bool ImportNBT(nbt::NBT* nbt);

            inventory::Slot GetLeftSlot() const noexcept;
            inventory::Slot GetMiddleSlot() const noexcept;
            inventory::Slot GetRightSlot() const noexcept;
            inventory::Slot GetIngredientSlot() const noexcept;
            inventory::Slot GetFuelSlot() const noexcept;

            u8 GetBrewTime() const noexcept { return m_BrewTime; }
            u8 GetFuel() const noexcept { return m_Fuel; }
        };

    } // ns block
} // ns mc
