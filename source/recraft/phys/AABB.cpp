#include "phys/AABB.hpp"

AABB::AABB(mc::Vector3f min, mc::Vector3f max)
    : min(min), max(max) {}

AABB::AABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
    : min(minX, minY, minZ), max(maxX, maxY, maxZ) {}

float AABB::ClipXCollide(const AABB& other, float xPos) const {
    if (other.max.y > min.y && other.min.y < max.y && other.max.z > min.z && other.min.z < max.z) {
        if (xPos > 0.0f) {
            if (other.max.x <= min.x) {
                xPos = std::min(min.x - other.max.x, xPos);
            }
        }

        if (xPos < 0.0f) {
            if (other.min.x >= max.x) {
                xPos = std::min(max.x - other.min.x, xPos);
            }
        }
    }

    return xPos;
}

float AABB::ClipYCollide(const AABB& other, float yPos) const {
    if (other.max.x > min.x && other.min.x < max.x && other.max.z > min.z && other.min.z < max.z){
        if (yPos > 0.0f){
            if (other.max.y <= min.y)
                yPos = std::min(min.y - other.max.y, yPos);
        }

        if (yPos < 0.0f){
            if (other.min.y >= max.y)
                yPos = std::max(max.y - other.min.y, yPos);
        }
    }

    return yPos;
}

float AABB::ClipZCollide(const AABB& other, float zPos) const {
    if (other.max.x > min.x && other.min.x < max.x && other.max.y > min.y && other.min.y < max.y){
        if (zPos > 0.0f){
            if (other.max.z <= min.z)
                zPos = std::min(min.z - other.max.z, zPos);
        }

        if (zPos < 0.0f){
            if (other.min.z >= max.z)
                zPos = std::max(max.z - other.min.z, zPos);
        }
    }

    return zPos;
}


bool AABB::containsX(mc::Vector3f position) {
    return position.y >= min.y && position.y <= max.y && position.z >= min.z && position.z <= max.z;
}

bool AABB::containsY(mc::Vector3f position) {
    return position.x >= min.x && position.x <= max.x && position.z >= min.z && position.z <= max.z;
}

bool AABB::containsZ(mc::Vector3f position) {
    return position.x >= min.x && position.x <= max.x && position.y >= min.y && position.y <= max.y;
}


bool AABB::Intersect(const AABB &other) const {
    return max.x > other.min.x
        && min.x < other.max.x
        && max.y > other.min.y
        && min.y < other.max.y
        && max.z > other.min.z
        && min.z < other.max.z;
}
