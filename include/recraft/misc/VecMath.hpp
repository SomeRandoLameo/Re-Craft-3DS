#pragma once

#include <math.h>
#include <mclib/common/Vector.h>

#include "misc/NumberUtils.hpp"

// TODO: MINOR TODO HERE

static inline mc::Vector3f Vector3f_Scale(mc::Vector3f a, float b) {
    return {a.x * b, a.y * b, a.z * b};
}

static inline float Vector3f_MagSqr(mc::Vector3f vec) {
    return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
}

static inline float Vector3f_dot(mc::Vector3f a, mc::Vector3f b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline mc::Vector3f Vector3f_crs(mc::Vector3f a, mc::Vector3f b) {
    return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

static inline float Vector3f_mag(mc::Vector3f vec) {
    return sqrtf(Vector3f_dot(vec, vec));
}

static inline mc::Vector3i ToVector3i(const mc::Vector3f& d) {
    return mc::Vector3i(FastFloor(d.x), FastFloor(d.y), FastFloor(d.z));
}
