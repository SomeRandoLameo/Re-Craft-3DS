#include "Frustum.h"
#include "../TypeUtil.h"

namespace terra {
namespace math {
namespace volumes {

Frustum::Frustum(glm::vec3 position, glm::vec3 forward, float near, float far, float fov, float ratio, glm::vec3 up, glm::vec3 right) 
    : m_Position(position),
      m_Forward(forward),
      m_Near(near),
      m_Far(far)
{
    m_NearHeight = 2 * std::tan(fov / 2.0f) * m_Near;
    m_NearWidth = m_NearHeight * ratio;

    m_FarHeight = 2 * std::tan(fov / 2.0f) * m_Far;
    m_FarWidth = m_FarHeight * ratio;

    glm::vec3 nc = position + forward * near;
    glm::vec3 fc = position + forward * far;

    float hnh = m_NearHeight / 2.0f;
    float hnw = m_NearWidth / 2.0f;
    float hfh = m_FarHeight / 2.0f;
    float hfw = m_FarWidth / 2.0f;

    glm::vec3 ntl = nc + up * hnh - right * hnw;
    glm::vec3 ntr = nc + up * hnh + right * hnw;
    glm::vec3 nbl = nc - up * hnh - right * hnw;
    glm::vec3 nbr = nc - up * hnh + right * hnw;

    glm::vec3 ftl = fc + up * hfh - right * hfw;
    glm::vec3 ftr = fc + up * hfh + right * hfw;
    glm::vec3 fbl = fc - up * hfh - right * hfw;
    glm::vec3 fbr = fc - up * hfh + right * hfw;

    m_Planes[0] = Plane(ntr, ntl, ftl);
    m_Planes[1] = Plane(nbl, nbr, fbr);
    m_Planes[2] = Plane(ntl, nbl, fbl);
    m_Planes[3] = Plane(nbr, ntr, fbr);
    m_Planes[4] = Plane(ntl, ntr, nbr);
    m_Planes[5] = Plane(ftr, ftl, fbl);
}

bool Frustum::Intersects(glm::vec3 v) const {
    for (int i = 0; i < 6; ++i) {
        if (m_Planes[i].PointDistance(v) < 0)
            return false;
    }

    return true;
}

bool Frustum::Intersects(mc::Vector3i v) const {
    for (int i = 0; i < 6; ++i) {
        if (m_Planes[i].PointDistance(v) < 0)
            return false;
    }

    return true;
}

bool Frustum::Intersects(mc::Vector3d v) const {
    for (int i = 0; i < 6; ++i) {
        if (m_Planes[i].PointDistance(v) < 0)
            return false;
    }

    return true;
}

glm::vec3 GetVertex(const mc::AABB& aabb, int index) {
    mc::Vector3d v;
    mc::Vector3d diff = aabb.max - aabb.min;

    if (index == 0) {
        v = aabb.min;
    } else if (index == 1) {
        v = aabb.min + mc::Vector3d(diff.x, 0, 0);
    } else if (index == 2) {
        v = aabb.min + mc::Vector3d(diff.x, diff.y, 0);
    } else if (index == 3) {
        v = aabb.min + mc::Vector3d(0, diff.y, 0);
    } else if (index == 4) {
        v = aabb.min + mc::Vector3d(0, diff.y, diff.z);
    } else if (index == 5) {
        v = aabb.min + mc::Vector3d(0, 0, diff.z);
    } else if (index == 6) {
        v = aabb.min + mc::Vector3d(diff.x, 0, diff.z);
    } else if (index == 7) {
        v = aabb.max;
    }

    return VecToGLM(v);
}

bool Frustum::Intersects(const mc::AABB& aabb) const {
    for (int i = 0; i < 6; ++i) {
        int out = 0;
        int in = 0;

        for (int k = 0; k < 8 && (in == 0 || out == 0); ++k) {
            glm::vec3 v = GetVertex(aabb, k);

            if (m_Planes[i].PointDistance(v) < 0) {
                ++out;
            } else {
                ++in;
            }
        }
        
        if (in == 0) {
            return false;
        }
    }

    return true;
}

} // ns volumes
} // ns math
} // ns terra
