#pragma once

#include <math.h>

#include "../../mclib/common/Vector.h"

//TODO: MINOR TODO HERE

static inline mc::Vector3d Vector3d_Scale(mc::Vector3d a, float b) { return {a.x * b, a.y * b, a.z * b}; }

static inline double Vector3d_MagSqr(mc::Vector3d vec) { return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z; }

static inline float Vector3d_dot(mc::Vector3d a, mc::Vector3d b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

static inline mc::Vector3d Vector3d_crs(mc::Vector3d a, mc::Vector3d b) { return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x}; }

static inline float Vector3d_mag(mc::Vector3d vec) { return sqrtf(Vector3d_dot(vec, vec)); }