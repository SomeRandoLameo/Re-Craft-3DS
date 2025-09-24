#pragma once

#include "../mclib.h"
#include "../common/Vector.h"
#include "../nbt/NBT.h"
#include "../inventory/Slot.h"

#include <string>
#include <memory>
#include <array>
#include <vector>

namespace mc {
    namespace block {

        enum class BlockEntityType {
            Banner,
            Beacon,
            Bed,
            Cauldron,
            BrewingStand,
            Chest,
            Comparator,
            CommandBlock,
            DaylightSensor,
            Dispenser,
            Dropper,
            EnchantingTable,
            EnderChest,
            EndGateway,
            EndPortal,
            FlowerPot,
            Furnace,
            Hopper,
            Jukebox,
            MonsterSpawner,
            Noteblock,
            Piston,
            ShulkerBox,
            Sign,
            Skull,
            StructureBlock,
            TrappedChest,

            Unknown
        };

        class BlockEntity {
        private:
            BlockEntityType m_Type;
            Vector3i m_Position;
            nbt::NBT m_NBT;

        protected:
            virtual MCLIB_API bool ImportNBT(nbt::NBT* nbt) { return true; }

        public:
            MCLIB_API BlockEntity(BlockEntityType type, Vector3i position) noexcept;

            MCLIB_API BlockEntityType GetType() const noexcept { return m_Type; }
            MCLIB_API Vector3i GetPosition() const noexcept { return m_Position; }
            MCLIB_API nbt::NBT* GetNBT() noexcept { return &m_NBT; }

            MCLIB_API static std::unique_ptr<BlockEntity> CreateFromNBT(nbt::NBT* nbt);
        };
        using BlockEntityPtr = std::shared_ptr<BlockEntity>;

    } // ns block
} // ns mc