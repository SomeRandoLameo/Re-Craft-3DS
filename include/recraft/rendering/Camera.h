#pragma once

#include "../entity/Player.h"
#include "Frustum.h"
#include <stdbool.h> // if really needed
#include <citro3d.h>

//mclib
#include <mclib/common/Vector.h>

class Camera {
public:
    Camera();
    void Update(Player* player, float iod);

    bool IsPointVisible(C3D_FVec point) const { return m_frustum.IsPointVisible(point); }
    bool IsAABBVisible(C3D_FVec origin, C3D_FVec size) const { return m_frustum.IsAABBVisible(origin, size); }

    C3D_Mtx* GetProjection() { return &m_projection; }
    C3D_Mtx* GetView() { return &m_view; }
    C3D_Mtx* GetVP() { return &m_vp; }

    const Frustum& GetFrustum() const { return m_frustum; }

private:
    C3D_Mtx m_projection, m_view, m_vp;
    Frustum m_frustum;
    float m_near, m_far, m_fov;
};
