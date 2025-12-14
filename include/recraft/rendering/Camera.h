#pragma once

#include "../entity/Player.h"
#include <stdbool.h> // if really needed
#include <citro3d.h>

//mclib
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

class Camera {
public:
    Camera();
    void Update(Player* player, float iod);

    bool IsPointVisible(C3D_FVec point) const;
    bool IsAABBVisible(C3D_FVec origin, C3D_FVec size) const;

    // Getters for matrices if really needed
    C3D_Mtx* GetProjection() { return &m_projection; }
    C3D_Mtx* GetView() { return &m_view; }
    C3D_Mtx* GetVP() { return &m_vp; }

private:
    C3D_Mtx m_projection, m_view, m_vp;
    C3D_FVec m_frustumPlanes[FrustumPlanes_Count];
    mc::Vector3d m_frustumCorners[FrustumCorners_Count];

    float m_near, m_far, m_fov;
};