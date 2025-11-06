#include "Plane.h"

namespace terra {
namespace math {

Plane::Plane(glm::vec3 normal, float distance) 
    : m_Normal(normal),
      m_Distance(distance)
{

}

Plane::Plane(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
    m_Normal = glm::normalize(glm::cross(p2 - p1, p3 - p1));
    m_Distance = glm::dot(m_Normal, p1);
}

float Plane::PointDistance(const glm::vec3& q) const {
    return (glm::dot(m_Normal, q) - m_Distance) / glm::dot(m_Normal, m_Normal);
}

float Plane::PointDistance(mc::Vector3i q) const {
    using T = mc::Vector3i::value_type;

    mc::Vector3i n(static_cast<T>(m_Normal.x), static_cast<T>(m_Normal.y), static_cast<T>(m_Normal.z));

    return static_cast<float>((n.Dot(q) - m_Distance) / n.Dot(n));
}

float Plane::PointDistance(mc::Vector3d q) const {
    mc::Vector3d n(m_Normal.x, m_Normal.y, m_Normal.z);

    return static_cast<float>((n.Dot(q) - m_Distance) / n.Dot(n));
}

} // ns math
} // ns terra
