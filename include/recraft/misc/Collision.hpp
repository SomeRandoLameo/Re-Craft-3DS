#pragma once

#include "misc/VecMath.hpp"

//TODO: Use MCLIB AABB
struct Box {
    mc::Vector3f min;
    mc::Vector3f max;
};

Box Box_Create(float x, float y, float z, float w, float h, float d);

bool Box_Contains(Box box, float x, float y, float z);

bool Collision_BoxIntersect(Box a, Box b, int ignore_faces, mc::Vector3f* ncoll,float* dcoll,int* fcoll);
