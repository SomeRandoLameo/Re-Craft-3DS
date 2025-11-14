#pragma once

#include "BlockEntity.h"

namespace mc {
    namespace block {

        class Sign : public BlockEntity {
        private:
            std::array<std::wstring, 4> m_Text;

        public:
             Sign(BlockEntityType type, Vector3i position) : BlockEntity(type, position) { }
             bool ImportNBT(nbt::NBT* nbt);

             const std::wstring& GetText(std::size_t index) const;
        };

    } // ns block
} // ns mc