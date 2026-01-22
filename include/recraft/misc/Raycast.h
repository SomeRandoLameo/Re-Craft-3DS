#pragma once

#include "../world/CT_World.h"

//mclib
#include <mclib/common/Vector.h>

typedef struct {
	mc::Vector3i hitPos;
	float distSqr;
	Direction direction;
    bool entity;
} Raycast_Result;

bool Raycast_Cast(World* world, mc::Vector3d inpos, mc::Vector3d raydir, Raycast_Result* out);