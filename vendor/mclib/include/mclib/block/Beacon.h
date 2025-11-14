#pragma once

#include "BlockEntity.h"

#include <unordered_map>

namespace mc {
    namespace block {
        class Beacon : public BlockEntity {
        private:
            std::wstring m_Lock;
            s32 m_Levels;
            s32 m_Primary;
            s32 m_Secondary;

        public:
            Beacon(BlockEntityType type, Vector3i position) : BlockEntity(type, position), m_Levels(0), m_Primary(0), m_Secondary(0) { }
            bool ImportNBT(nbt::NBT* nbt);

            s32 GetLevels() const noexcept { return m_Levels; }
            s32 GetPrimary() const noexcept { return m_Primary; }
            s32 GetSecondary() const noexcept { return m_Secondary; }
        };

    } // ns block
} // ns mc
