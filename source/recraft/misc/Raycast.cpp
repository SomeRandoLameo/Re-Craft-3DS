#include "misc/Raycast.hpp"
#include "misc/VecMath.hpp"

#include "world/CT_World.hpp"

#include <stdio.h>
#include <stdlib.h>

#include <citro3d.h>

static constexpr int INF = (World::ChunkCacheSize / 2 * Chunk::Size);

bool Raycast_Cast(World* world, mc::Vector3f inpos, mc::Vector3f raydir, Raycast_Result* out) {
    mc::Vector3f map = mc::Vector3f(FastFloor(inpos.x), //
                                    FastFloor(inpos.y), //
                                    FastFloor(inpos.z));

    mc::Vector3f square = mc::Vector3f(raydir.x * raydir.x, //
                                       raydir.y * raydir.y, //
                                       raydir.z * raydir.z);

    mc::Vector3f deltaDist = mc::Vector3f(sqrtf(1.f + (square.y + square.z) / square.x), //
                                          sqrtf(1.f + (square.x + square.z) / square.y), //
                                          sqrtf(1.f + (square.x + square.y) / square.z));

    mc::Vector3f step     = mc::Vector3f(0, 0, 0);
    mc::Vector3f sideDist = mc::Vector3f(0, 0, 0);
    if (raydir.x < 0) {
        step.x     = -1;
        sideDist.x = (inpos.x - map.x) * deltaDist.x;
    } else {
        step.x     = 1;
        sideDist.x = (map.x + 1.f - inpos.x) * deltaDist.x;
    }
    if (raydir.y < 0) {
        step.y     = -1;
        sideDist.y = (inpos.y - map.y) * deltaDist.y;
    } else {
        step.y     = 1;
        sideDist.y = (map.y + 1.f - inpos.y) * deltaDist.y;
    }
    if (raydir.z < 0) {
        step.z     = -1;
        sideDist.z = (inpos.z - map.z) * deltaDist.z;
    } else {
        step.z     = 1;
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

        if (world->GetBlock(ToVector3i(map)) != Block::Air || world->GetBlock(ToVector3i(map)) == Block::Lava) {
            hit = 1;
        }
        // if (world->errFlags & World_ErrUnloadedBlockRequested) break;

        if (steps++ > INF) {
            break;
        }
    }

    switch (side) {
    case 0: // X Achse
        if (raydir.x > 0.f) {
            out->direction = Direction::West;
        } else {
            out->direction = Direction::East;
        }
        break;
    case 1: // Y Achse
        if (raydir.y > 0.f) {
            out->direction = Direction::Bottom;
        } else {
            out->direction = Direction::Top;
        }
        break;
    case 2: // Z Achse
        if (raydir.z > 0.f) {
            out->direction = Direction::North;
        } else {
            out->direction = Direction::South;
        }
        break;
    default:
        printf("Unknown axis! %d\n", side);
        break;
    }

    mc::Vector3f dist = map - inpos;
    out->distSqr      = Vector3f_MagSqr(dist);
    out->hitPos       = ToVector3i(map);

    // TODO: Add Entities to damage
    out->entity = false;

    return hit;
}
