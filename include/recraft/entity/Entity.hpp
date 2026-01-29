#pragma once

#include <mclib/common/Vector.h>
#include "misc/VecMath.hpp"
#include "phys/AABB.hpp"
#include "world/CT_World.hpp"

class Entity {
public:
    Entity(World* world);
    ~Entity() = default;

    mc::Vector3f position       = mc::Vector3f(0.0f, 0.0f, 0.0f);
    mc::Vector3f oldPosition    = mc::Vector3f(0.0f, 0.0f, 0.0f);
    mc::Vector3f deltaPosition  = mc::Vector3f(0.0f, 0.0f, 0.0f);

    bool onGround = false;
    bool removed = false;

    float xRot = 0.f;
    float yRot = 0.f;

    float heightOffset = 0.0f;
    float ySlideOffset = 0.0f;

    float bbWidth = 0.6f;
    float bbHeight = 1.8f;
    AABB bb;

    virtual void MoveTo(mc::Vector3f pos, float yaw, float pitch);
    virtual void MoveRelative(float x, float z, float speed);

    virtual void SetRot(float yaw, float pitch);
    virtual void SetSize(float bbWidth, float bbHeight);
    virtual void SetPos(mc::Vector3f pos);
    virtual void ResetPos();

    virtual void Turn(float yRotDelta, float xRotDelta);

    virtual void CheckFallDamage(float deltaY, bool HitGround);
    virtual void CauseFallDamage(float distance);

    virtual void Tick();
protected:
    World* m_world = nullptr;
    float m_fallDistance = 0.0f;


private:
};
