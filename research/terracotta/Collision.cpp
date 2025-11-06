#include "Collision.h"
#include "Transform.h"

#include <iostream>

namespace terra {

using mc::Vector3d;
using mc::Vector3i;
using mc::AABB;
using mc::Ray;

template <typename T>
inline T Sign(T val) {
    return std::signbit(val) ? static_cast<T>(-1) : static_cast<T>(1);
}

inline Vector3d BasisAxis(int basisIndex) {
    static const Vector3d axes[3] = { Vector3d(1, 0, 0), Vector3d(0, 1, 0), Vector3d(0, 0, 1) };
    return axes[basisIndex];
}

Vector3d GetClosestFaceNormal(const Vector3d& pos, AABB bounds) {
    Vector3d center = bounds.min + (bounds.max - bounds.min) / 2;
    Vector3d dim = bounds.max - bounds.min;
    Vector3d offset = pos - center;

    double minDist = std::numeric_limits<double>::max();
    Vector3d normal;

    for (int i = 0; i < 3; ++i) {
        double dist = dim[i] - std::abs(offset[i]);
        if (dist < minDist) {
            minDist = dist;
            normal = BasisAxis(i) * Sign(offset[i]);
        }
    }

    return normal;
}


bool CollisionDetector::DetectCollision(Vector3d from, Vector3d rayVector, Collision* collision) const {
    static const std::vector<Vector3d> directions = {
        Vector3d(0, 0, 0), Vector3d(1, 0, 0), Vector3d(-1, 0, 0), Vector3d(0, 1, 0), Vector3d(0, -1, 0), Vector3d(0, 0, 1), Vector3d(0, 0, -1)
    };

    Vector3d direction = Vector3Normalize(rayVector);
    double length = rayVector.Length();

    Ray ray(from, direction);

    if (collision)
        * collision = Collision();

    for (double i = 0; i < length; ++i) {
        Vector3d position = from + direction * i;

        // Look for collisions in any blocks surrounding the ray
        for (Vector3d checkDirection : directions) {
            Vector3d checkPos = position + checkDirection;
            mc::block::BlockPtr block = m_World->GetBlock(mc::ToVector3i(checkPos));

            if (block && block->IsSolid()) {
                AABB bounds = block->GetBoundingBox(checkPos);
                double distance;

                if (bounds.Intersects(ray, &distance)) {
                    if (distance < 0 || distance > length) continue;

                    Vector3d collisionHit = from + direction * distance;
                    Vector3d normal = GetClosestFaceNormal(collisionHit, bounds);

                    if (collision)
                        * collision = Collision(collisionHit, normal);

                    return true;
                }
            }
        }
    }

    return false;
}

std::vector<Vector3i> CollisionDetector::GetSurroundingLocations(AABB bounds) {
    std::vector<Vector3i> locs;

    s32 radius = 2;
    for (s32 y = (s32)bounds.min.y - radius; y < (s32)bounds.max.y + radius; ++y) {
        for (s32 z = (s32)bounds.min.z - radius; z < (s32)bounds.max.z + radius; ++z) {
            for (s32 x = (s32)bounds.min.x - radius; x < (s32)bounds.max.x + radius; ++x) {
                locs.emplace_back(x, y, z);
            }
        }
    }

    return locs;
}

void CollisionDetector::ResolveCollisions(Transform* transform, double dt, bool* onGround) {
    const s32 MaxIterations = 10;
    bool collisions = true;

    Vector3d velocity = transform->velocity;
    Vector3d input_velocity = transform->input_velocity;

    for (s32 iteration = 0; iteration < MaxIterations && collisions; ++iteration) {
        Vector3d position = transform->position;

        collisions = false;

        for (std::size_t i = 0; i < 3; ++i) {
            AABB playerBounds = transform->bounding_box;

            if (iteration == 0)
                position[i] += velocity[i] * dt + input_velocity[i] * dt;

            playerBounds.min += position;
            playerBounds.max += position;

            std::vector<Vector3i> surrounding = GetSurroundingLocations(playerBounds);

            for (Vector3i checkPos : surrounding) {
                mc::block::BlockPtr block = m_World->GetBlock(checkPos);

                if (block && block->IsSolid()) {
                    AABB blockBounds = block->GetBoundingBox(checkPos);

                    if (playerBounds.Intersects(blockBounds)) {
                        if (onGround != nullptr && i == 1 && checkPos.y < transform->position.y) {
                            *onGround = true;
                        }

                        velocity[i] = 0;
                        input_velocity[i] = 0;

                        double penetrationDepth;

                        if (playerBounds.min[i] < blockBounds.min[i]) {
                            penetrationDepth = playerBounds.max[i] - blockBounds.min[i];
                        } else {
                            penetrationDepth = playerBounds.min[i] - blockBounds.max[i];
                        }

                        position[i] -= penetrationDepth;
                        collisions = true;
                        break;
                    }
                }
            }
        }

        transform->position = position;
    }

    transform->velocity = velocity;
    transform->input_velocity = input_velocity;
}

} // namespace terra
