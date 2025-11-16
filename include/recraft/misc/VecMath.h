#pragma once

#include <math.h>

#include "NumberUtils.h"

#include "../../mclib/common/Vector.h"
//TODO: Use MCLIB Vector stuff
typedef union {
	float v[3];
	struct {
		float x, y, z;
	};
} float3;

static inline float3 f3_new(float x, float y, float z) { return (float3){{x, y, z}}; }

static inline float3 f3_add(float3 a, float3 b) { return f3_new(a.x + b.x, a.y + b.y, a.z + b.z); }
static inline float3 f3_sub(float3 a, float3 b) { return f3_new(a.x - b.x, a.y - b.y, a.z - b.z); }

//TODO: BIG FAT TODO HERE
static inline float3 f3_scl(float3 a, float b) { return f3_new(a.x * b, a.y * b, a.z * b); }

static inline mc::Vector3d Vector3d_Scale(mc::Vector3d a, float b) { return mc::Vector3d(a.x * b, a.y * b, a.z * b); }

static inline float f3_dot(float3 a, float3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static inline float3 f3_crs(float3 a, float3 b) { return f3_new(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }

static inline float f3_mag(float3 vec) { return sqrtf(f3_dot(vec, vec)); }
static inline float f3_magSqr(float3 vec) { return f3_dot(vec, vec); }
static inline float3 f3_nrm(float3 vec) {
	float m = f3_mag(vec);
	return f3_new(vec.x / m, vec.y / m, vec.z / m);
}


#define f3_unpack(v) (v).x, (v).y, (v).z