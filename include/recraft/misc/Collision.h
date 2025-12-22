#pragma once

#include <stdbool.h>

#include "VecMath.h"
//mclib
#include <mclib/common/Vector.h>
//TODO: Use MCLIB AABB
typedef struct { mc::Vector3d min, max; } Box;

Box Box_Create(float x, float y, float z, float w, float h, float d);

bool Box_Contains(Box box, float x, float y, float z);

bool Collision_BoxIntersect(Box a, Box b, int ignore_faces, mc::Vector3d* ncoll,float* dcoll,int* fcoll);
