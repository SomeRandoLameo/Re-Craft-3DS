#include "entity/Entity.hpp"

#include "world/CT_World.hpp"

Entity::Entity(World* world) : m_world(world) {}

void Entity::MoveTo(mc::Vector3f pos, float yaw, float pitch)
{
    pos.y += heightOffset;

    position = pos;
    oldPosition = pos;
    deltaPosition = pos;

    yRot = yaw;

    xRot = pitch;

    SetPos(pos);
}

void Entity::MoveRelative(float x, float z, float speed)
{
    float len = x * x + z * z;
    if (len >= 0.01f)
    {
        len = speed / std::sqrt(len);
        x *= len;
        z *= len;

        // Zyklische yRot verwenden (zwischen 0 und 360 Grad)
        float yRotRad = (this->yRot * M_PI / 180.0f);

        float sinYaw = std::sin(yRotRad);
        float cosYaw = std::cos(yRotRad);

        deltaPosition.x += x * cosYaw - z * sinYaw;
        deltaPosition.z += z * cosYaw + x * sinYaw;
    }
}

void Entity::SetRot(float yaw, float pitch) {
    yRot = yaw;
    xRot = pitch;
}

void Entity::SetPos(mc::Vector3f pos) {
    position = pos;

    float halfSize = bbWidth / 2;
    float lowY = pos.y - heightOffset + ySlideOffset;

    bb = AABB(
        pos.x - halfSize,
        lowY,
        pos.z - halfSize,
        pos.x + halfSize,
        lowY + bbHeight,
        pos.z + halfSize
    );
}

void Entity::ResetPos() {}

void Entity::SetSize(float bbWidth, float bbHeight){
    this->bbWidth = bbWidth;
    this->bbHeight = bbHeight;
}

void Entity::Turn(float yRotDelta, float xRotDelta) {

    yRot += yRotDelta * 0.15f;
    xRot = std::clamp(xRot - xRotDelta * 0.15f, -90.0f, 90.0f);

    yRot = std::remainder(yRot, 360.0f);
    if (yRot < 0.0f) yRot += 360.0f;
}

void Entity::CheckFallDamage(float deltaY, bool HitGround) {
    if (HitGround) {
        if (m_fallDistance > 0.0f) {
            CauseFallDamage(m_fallDistance);
            m_fallDistance = 0.0f;
        }
    } else if (deltaY < 0.0f) {
        m_fallDistance -= deltaY;
    }
}

void Entity::CauseFallDamage(float distance) {}

void Entity::Tick() {
    deltaPosition = position;
}
