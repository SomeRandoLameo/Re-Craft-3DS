#pragma once

#include <citro3d.h>
#include <mclib/common/Vector.h>

enum FrustumPlanes {
    Frustum_Near = 0,
    Frustum_Right,
    Frustum_Left,
    Frustum_Top,
    Frustum_Bottom,
    Frustum_Far,
    FrustumPlanes_Count
};

enum FrustumCorners {
    Frustum_NearBottomLeft = 0,
    Frustum_NearBottomRight,
    Frustum_NearTopLeft,
    Frustum_NearTopRight,
    Frustum_FarBottomLeft,
    Frustum_FarBottomRight,
    Frustum_FarTopLeft,
    Frustum_FarTopRight,
    FrustumCorners_Count
};

class Frustum {
public:
    Frustum();

    void UpdateFromMatrix(const C3D_Mtx& vp);
    void UpdateCorners(const mc::Vector3d& position, const mc::Vector3d& forward,
                       const mc::Vector3d& right, const mc::Vector3d& up,
                       float fov, float aspectRatio, float nearPlane, float farPlane);

    bool IsPointVisible(C3D_FVec point) const;
    bool IsAABBVisible(C3D_FVec origin, C3D_FVec size) const;

    const C3D_FVec* GetPlanes() const { return m_planes; }
    const mc::Vector3d* GetCorners() const { return m_corners; }

private:
    C3D_FVec m_planes[FrustumPlanes_Count];
    mc::Vector3d m_corners[FrustumCorners_Count];
};