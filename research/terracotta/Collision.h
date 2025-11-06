#pragma once

#ifndef TERRACOTTA_COLLISION_H_
#define TERRACOTTA_COLLISION_H_

#include "Transform.h"
#include "World.h"

namespace terra {

class Collision {
private:
    mc::Vector3d m_Position;
    mc::Vector3d m_Normal;

public:
    Collision() noexcept { }
    Collision(mc::Vector3d position, mc::Vector3d normal) noexcept : m_Position(position), m_Normal(normal) { }

    mc::Vector3d GetPosition() const noexcept { return m_Position; }
    mc::Vector3d GetNormal() const noexcept { return m_Normal; }
};

class CollisionDetector {
private:
    terra::World* m_World;

    std::vector<mc::Vector3i> GetSurroundingLocations(mc::AABB bounds);

public:
    CollisionDetector(terra::World* world) noexcept : m_World(world) { }

    bool DetectCollision(mc::Vector3d from, mc::Vector3d rayVector, Collision* collision) const;

    void ResolveCollisions(Transform* transform, double dt, bool* onGround);
};

} // namespace terra

#endif
