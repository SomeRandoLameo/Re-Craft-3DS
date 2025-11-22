#include "misc/Raycast.h"
#include "misc/VecMath.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <citro3d.h>

#define INF (CHUNKCACHE_SIZE / 2 * CHUNK_SIZE)

bool Raycast_Cast(World* world, mc::Vector3d inpos, mc::Vector3d raydir, Raycast_Result* out) {
    mc::Vector3d map = mc::Vector3d(
            FastFloor(inpos.x),
            FastFloor(inpos.y),
            FastFloor(inpos.z)
            );

    mc::Vector3d square = mc::Vector3d(
            raydir.x * raydir.x,
            raydir.y * raydir.y,
            raydir.z * raydir.z
            );

    mc::Vector3d deltaDist = mc::Vector3d(
            sqrtf(1.f + (square.y + square.z) / square.x),
            sqrtf(1.f + (square.x + square.z) / square.y),
            sqrtf(1.f + (square.x + square.y) / square.z)
            );

    mc::Vector3d step = mc::Vector3d(0,0,0);
    mc::Vector3d sideDist = mc::Vector3d(0,0,0);
	if (raydir.x < 0) {
		step.x = -1;
		sideDist.x = (inpos.x - map.x) * deltaDist.x;
	} else {
		step.x = 1;
		sideDist.x = (map.x + 1.f - inpos.x) * deltaDist.x;
	}
	if (raydir.y < 0) {
		step.y = -1;
		sideDist.y = (inpos.y - map.y) * deltaDist.y;
	} else {
		step.y = 1;
		sideDist.y = (map.y + 1.f - inpos.y) * deltaDist.y;
	}
	if (raydir.z < 0) {
		step.z = -1;
		sideDist.z = (inpos.z - map.z) * deltaDist.z;
	} else {
		step.z = 1;
		sideDist.z = (map.z + 1.f - inpos.z) * deltaDist.z;
	}

	int hit = 0, side = 0, steps = 0;
	while (hit == 0) {
		if (sideDist.x < sideDist.y && sideDist.x < sideDist.z) {
			sideDist.x += deltaDist.x;
			map.x += step.x;
			side = 0;
		} else if (sideDist.y < sideDist.z) {
			sideDist.y += deltaDist.y;
			map.y += step.y;
			side = 1;
		} else {
			sideDist.z += deltaDist.z;
			map.z += step.z;
			side = 2;
		}

		if (world->GetBlock(mc::ToVector3i(map)) != Block_Air || world->GetBlock(mc::ToVector3i(map)) == Block_Lava) hit = 1;
		// if (world->errFlags & World_ErrUnloadedBlockRequested) break;

		if (steps++ > INF) break;
	}

	switch (side) {
		case 0:  // X Achse
			if (raydir.x > 0.f)
				out->direction = Direction_West;
			else
				out->direction = Direction_East;
			break;
		case 1:  // Y Achse
			if (raydir.y > 0.f)
				out->direction = Direction_Bottom;
			else
				out->direction = Direction_Top;
			break;
		case 2:  // Z Achse
			if (raydir.z > 0.f)
				out->direction = Direction_North;
			else
				out->direction = Direction_South;
			break;
		default:
			printf("Unknown axis! %d\n", side);
			break;
	}

	mc::Vector3d dist = map - inpos;
	out->distSqr = Vector3d_MagSqr(dist);
	out->x = map.x;
	out->y = map.y;
	out->z = map.z;

	return hit;
}