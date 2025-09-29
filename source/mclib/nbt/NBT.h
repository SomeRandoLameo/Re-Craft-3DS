#pragma once

#include "../mclib.h"
#include "../common/Types.h"
#include "Tag.h"

#include <string>
#include <vector>
#include <memory>
#include <map>

namespace mc {

    class DataBuffer;

    namespace nbt {

        class NBT {
        private:
            TagCompound m_Root;

        public:
             NBT() { }

            NBT(const NBT& rhs) = default;
            NBT& operator=(const NBT& rhs) = default;
            NBT(NBT&& rhs) = default;
            NBT& operator=(NBT&& rhs) = default;

            TagCompound& GetRoot() noexcept { return m_Root; }
            const TagCompound& GetRoot() const noexcept { return m_Root; }
            bool HasData() const { return m_Root.begin() != m_Root.end(); }

            void SetRoot(const TagCompound& compound) { m_Root = compound; }

            template <typename T>
            T* GetTag(const std::wstring& tagName) const {
                return m_Root.GetTag<T>(tagName);
            }

            friend  DataBuffer& operator>>(DataBuffer& out, NBT& nbt);
        };

         DataBuffer& operator<<(DataBuffer& out, const NBT& nbt);
         DataBuffer& operator>>(DataBuffer& in, NBT& nbt);

    } // ns nbt
} // ns mc
