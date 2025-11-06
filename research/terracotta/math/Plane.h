#ifndef TERRACOTTA_MATH_PLANE_H_
#define TERRACOTTA_MATH_PLANE_H_

#include <glm/glm.hpp>
#include <mclib/common/Vector.h>

namespace terra {
namespace math {

class Plane {
public:
    Plane() { }
    Plane(glm::vec3 normal, float distance);
    // Construct plane from three noncollinear points
    Plane(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);

    const glm::vec3& GetNormal() const { return m_Normal; }
    float GetDistance() const { return m_Distance; }

    float PointDistance(const glm::vec3& q) const;
    float PointDistance(mc::Vector3i q) const;
    float PointDistance(mc::Vector3d q) const;

private:
    glm::vec3 m_Normal;
    float m_Distance;
};

} // ns math
} // ns terra

#endif
