#pragma once

#include "../world/CT_World.h"

//mclib
#include <mclib/common/Vector.h>

typedef struct {
	int x, y, z;
	float distSqr;
	Direction direction;
} Raycast_Result;

bool Raycast_Cast(World* world, mc::Vector3d inpos, mc::Vector3d raydir, Raycast_Result* out);