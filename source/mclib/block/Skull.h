#pragma once

#include "BlockEntity.h"
#include "../common/UUID.h"

#include <vector>

namespace mc {
    namespace block {

        enum class SkullType {
            Skeleton,
            WitherSkeleton,
            Zombie,
            Head,
            Creeper,
            Dragon
        };

        std::string to_string(SkullType type);
        std::wstring to_wstring(SkullType type);

        class Skull : public BlockEntity {
        public:
            struct Texture {
                std::wstring signature;
                std::wstring value;
            };
        private:
            SkullType m_Type;
            u8 m_Rotation;
            UUID m_OwnerUUID;
            std::wstring m_OwnerName;
            std::vector<Texture> m_Textures;

        public:
            Skull(BlockEntityType type, Vector3i position) : BlockEntity(type, position) { }
            bool ImportNBT(nbt::NBT* nbt);

            inline SkullType GetType() const noexcept { return m_Type; }
            inline u8 GetRotation() const noexcept { return m_Rotation; }
            inline UUID GetOwnerUUID() const noexcept { return m_OwnerUUID; }
            inline const std::wstring& GetOwnerName() const noexcept { return m_OwnerName; }
            inline const std::vector<Texture>& GetTextures() const noexcept { return m_Textures; }
        };

    } // ns block
} // ns mc