#pragma once

#include "Entity.h"

namespace mc {
    namespace entity {

        class EntityFactory {
        public:
            static Entity* CreateEntity(EntityId id, u8 type);
        };

    } // ns entity
} // ns mc
