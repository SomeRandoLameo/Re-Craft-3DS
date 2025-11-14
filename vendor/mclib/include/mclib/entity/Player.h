#pragma once

#include "LivingEntity.h"

namespace mc {
    namespace entity {
        class PlayerEntity : public LivingEntity {
        public:
            PlayerEntity(EntityId id, protocol::Version protocolVersion) : LivingEntity(id, protocolVersion) { }
        };

        typedef std::weak_ptr<PlayerEntity> PlayerEntityPtr;

    } // ns entity
} // ns mc