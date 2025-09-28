#pragma once

#include "BlockEntity.h"

#include <unordered_map>

namespace mc {
    namespace block {

        class EndGateway : public BlockEntity {
        private:
            s64 m_Age;
            bool m_ExactTeleport;
            Vector3i m_Exit;

        public:
            EndGateway(BlockEntityType type, Vector3i position) : BlockEntity(type, position) { }
            bool ImportNBT(nbt::NBT* nbt);

            inline s64 GetAge() const noexcept { return m_Age; }
            inline bool IsExactTeleport() const noexcept { return m_ExactTeleport; }
            inline Vector3i GetExit() const noexcept { return m_Exit; }
        };

    } // ns block
} // ns mc
