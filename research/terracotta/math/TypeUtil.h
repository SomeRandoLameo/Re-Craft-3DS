#ifndef TERRACOTTA_MATH_TYPEUTIL_H_
#define TERRACOTTA_MATH_TYPEUTIL_H_

#include <glm/glm.hpp>
#include <mclib/common/Vector.h>

namespace terra {
namespace math {

inline glm::vec3 VecToGLM(mc::Vector3d v) { return glm::vec3(v.x, v.y, v.z); }
inline glm::vec3 VecToGLM(mc::Vector3i v) { return glm::vec3(v.x, v.y, v.z); }

} // ns math
} // ns terra

#endif
