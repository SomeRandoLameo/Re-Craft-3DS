#include "rendering/Frustum.hpp"
#include "misc/VecMath.hpp"

Frustum::Frustum() {
    for (auto& m_plane : m_planes) {
        m_plane = FVec4_New(0.f, 0.f, 0.f, 0.f);
    }
    for (auto& m_corner : m_corners) {
        m_corner = mc::Vector3f(0.f, 0.f, 0.f);
    }
}

void Frustum::UpdateFromMatrix(const C3D_Mtx& vp) {
    C3D_FVec rowX = vp.r[0];
    C3D_FVec rowY = vp.r[1];
    C3D_FVec rowZ = vp.r[2];
    C3D_FVec rowW = vp.r[3];

    m_planes[Frustum_Near]   = FVec4_Normalize(FVec4_Subtract(rowW, rowZ));
    m_planes[Frustum_Right]  = FVec4_Normalize(FVec4_Add(rowW, rowX));
    m_planes[Frustum_Left]   = FVec4_Normalize(FVec4_Subtract(rowW, rowX));
    m_planes[Frustum_Top]    = FVec4_Normalize(FVec4_Add(rowW, rowY));
    m_planes[Frustum_Bottom] = FVec4_Normalize(FVec4_Subtract(rowW, rowY));
    m_planes[Frustum_Far]    = FVec4_Normalize(FVec4_Add(rowW, rowZ));
}

void Frustum::UpdateCorners(const mc::Vector3f& position, const mc::Vector3f& forward, const mc::Vector3f& right,
                            const mc::Vector3f& up, float fov, float aspectRatio, float nearPlane, float farPlane) {
    float tan2halffov = 2.f * tanf(fov / 2.f);

    float hNear = tan2halffov * nearPlane;
    float wNear = hNear * aspectRatio;

    float hFar = tan2halffov * farPlane;
    float wFar = hFar * aspectRatio;

    mc::Vector3f cNear = position + Vector3f_Scale(forward, nearPlane);
    mc::Vector3f cFar  = position + Vector3f_Scale(forward, farPlane);

    m_corners[Frustum_NearBottomLeft] =
        ((cNear - Vector3f_Scale(up, hNear * 0.5f)) - Vector3f_Scale(right, wNear * 0.5f));
    m_corners[Frustum_NearBottomRight] =
        ((cNear - Vector3f_Scale(up, hNear * 0.5f)) + Vector3f_Scale(right, wNear * 0.5f));
    m_corners[Frustum_NearTopLeft] = ((cNear + Vector3f_Scale(up, hNear * 0.5f)) - Vector3f_Scale(right, wNear * 0.5f));
    m_corners[Frustum_NearTopRight] =
        ((cNear + Vector3f_Scale(up, hNear * 0.5f)) + Vector3f_Scale(right, wNear * 0.5f));
    m_corners[Frustum_FarBottomLeft]  = ((cFar - Vector3f_Scale(up, hFar * 0.5f)) - Vector3f_Scale(right, wFar * 0.5f));
    m_corners[Frustum_FarBottomRight] = ((cFar - Vector3f_Scale(up, hFar * 0.5f)) + Vector3f_Scale(right, wFar * 0.5f));
    m_corners[Frustum_FarTopLeft]     = ((cFar + Vector3f_Scale(up, hFar * 0.5f)) - Vector3f_Scale(right, wFar * 0.5f));
    m_corners[Frustum_FarTopRight]    = ((cFar + Vector3f_Scale(up, hFar * 0.5f)) + Vector3f_Scale(right, wFar * 0.5f));
}

bool Frustum::IsPointVisible(C3D_FVec point) const {
    point.w = 1.f;
    for (auto m_plane : m_planes) {
        if (FVec4_Dot(point, m_plane) < 0.f) {
            return false;
        }
    }
    return true;
}
// TODO: Use actual AABB from MCLib
bool Frustum::IsAABBVisible(C3D_FVec origin, C3D_FVec size) const {
    mc::Vector3f min(origin.x, origin.y, origin.z);
    mc::Vector3f max(origin.x + size.x, origin.y + size.y, origin.z + size.z);

    // Check if AABB is outside any frustum plane
    for (auto m_plane : m_planes) {
        int out = 0;
        for (int z = 0; z < 2; z++) {
            for (int y = 0; y < 2; y++) {
                for (int x = 0; x < 2; x++) {
                    C3D_FVec corner = FVec4_New(x ? max.x : min.x, y ? max.y : min.y, z ? max.z : min.z, 1.0f);
                    out += (FVec4_Dot(m_plane, corner) < 0.0);
                }
            }
        }
        if (out == 8) {
            return false;
        }
    }

    for (int axis = 0; axis < 3; axis++) {
        int countAbove = 0, countBelow = 0;

        for (const auto& m_corner : m_corners) {
            float coord   = (axis == 0) ? m_corner.x : (axis == 1) ? m_corner.y : m_corner.z;
            float axisMin = (axis == 0) ? min.x : (axis == 1) ? min.y : min.z;
            float axisMax = (axis == 0) ? max.x : (axis == 1) ? max.y : max.z;

            if (coord > axisMax) {
                countAbove++;
            }
            if (coord < axisMin) {
                countBelow++;
            }
        }

        if (countAbove == 8 || countBelow == 8) {
            return false;
        }
    }

    return true;
}
