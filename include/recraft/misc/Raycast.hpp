#pragma once

#include "world/Direction.hpp"

// mclib
#include <mclib/common/Vector.h>

class World;
struct Raycast_Result {
    mc::Vector3i hitPos;
    float        distSqr;
    Direction    direction;
    bool         entity;
};

bool Raycast_Cast(World* world, mc::Vector3f inpos, mc::Vector3f raydir, Raycast_Result* out);
