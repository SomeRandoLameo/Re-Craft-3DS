#pragma once

#include "BlockEntity.h"

#include <unordered_map>

namespace mc {
    namespace block {

        class Bed : public BlockEntity {
        private:
            s32 m_Color;

        public:
            Bed(BlockEntityType type, Vector3i position) : BlockEntity(type, position) { }
            bool ImportNBT(nbt::NBT* nbt);

            inline s32 GetColor() const noexcept { return m_Color; }
        };

    } // ns block
} // ns mc
