#pragma once
#include <mclib/common/Vector.h>

class AABB
{
public:

    AABB() = default;

    AABB(mc::Vector3f min, mc::Vector3f max);

    AABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);

    mc::Vector3f min, max;

    void Move(mc::Vector3f position) {
        min += position;
        max += position;
    }

    void Grow(mc::Vector3f dimensions) {
        min -= dimensions;
        max += dimensions;
    };

    void Grow(float size){
        min -= mc::Vector3f(size, size, size);
        max += mc::Vector3f(size, size, size);
    }

    void Expand(mc::Vector3f dimensions) {
        if (dimensions.x < 0) min.x += dimensions.x;
        if (dimensions.x > 0) max.x += dimensions.x;
        if (dimensions.y < 0) min.y += dimensions.y;
        if (dimensions.y > 0) max.y += dimensions.y;
        if (dimensions.z < 0) min.z += dimensions.z;
        if (dimensions.z > 0) max.z += dimensions.z;
    }

    bool Intersect(const AABB &other) const;

    bool Contains(mc::Vector3f position) const
    {
        if (position.x <= min.x || position.x >= max.x)
            return false;
        if (position.y > min.y && position.y < max.y)
            return position.z > min.z && position.z < max.z;
        return false;
    }

    bool containsX(mc::Vector3f position);
    bool containsY(mc::Vector3f position);
    bool containsZ(mc::Vector3f position);

    float ClipXCollide(const AABB &other, float xPos) const;
    float ClipYCollide(const AABB &other, float yPos) const;
    float ClipZCollide(const AABB &other, float zPos) const;

};
