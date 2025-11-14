#pragma once

#include "Entity.h"

namespace mc {
    namespace entity {

        class LivingEntity : public Entity {
        private:
            float m_Health;

        public:
            LivingEntity(EntityId id, protocol::Version protocolVersion) : Entity(id, protocolVersion) { }

            float GetHealth() const { return m_Health; }
        };

    } // ns entity
} // ns mc
