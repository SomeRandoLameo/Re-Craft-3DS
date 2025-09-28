#pragma once

#include "BlockEntity.h"

#include <unordered_map>

namespace mc {
    namespace block {

        class NoteBlock : public BlockEntity {
        private:
            u8 m_Note;
            u8 m_Powered;

        public:
            NoteBlock(BlockEntityType type, Vector3i position) : BlockEntity(type, position) { }
            bool ImportNBT(nbt::NBT* nbt);

            inline u8 GetNode() const noexcept { return m_Note; }
            inline bool IsPowered() const noexcept { return m_Powered != 0; }
        };

    } // ns block
} // ns mc
