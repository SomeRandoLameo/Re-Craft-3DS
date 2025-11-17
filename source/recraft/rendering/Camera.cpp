#include "rendering/Camera.h"

#include "world/CT_Chunk.h"

#include "gui/DebugUI.h"

Camera::Camera() {
	Mtx_Identity(&view);

	fov = C3D_AngleFromDegrees(60.f);
	near = 0.2f;
	far = 8.f * CHUNK_SIZE;

	Mtx_PerspTilt(&projection, fov, (400.f / 240.f), near, far, false);
}

void Camera::Update(Player* player, float iod) {
	float currentFov = fov + C3D_AngleFromDegrees(12.f) * player->fovAdd;
	Mtx_PerspStereoTilt(&projection, currentFov, (400.f / 240.f), near, far, iod, 1.f, false);

    // View bobbing at player head
	mc::Vector3d playerHead(
			player->position.x,
			player->position.y + PLAYER_EYEHEIGHT + sinf(player->bobbing) * 0.1f + player->crouchAdd,
			player->position.z
	);

	Mtx_Identity(&view);
	Mtx_RotateX(&view, -player->pitch, true);
	Mtx_RotateY(&view, -player->yaw, true);
	Mtx_Translate(&view, -playerHead.x, -playerHead.y, -playerHead.z, true);

	C3D_Mtx vpTemp;
	Mtx_Multiply(&vpTemp, &projection, &view);
	Mtx_Copy(&vp, &vpTemp);

	C3D_FVec rowX = vpTemp.r[0];
	C3D_FVec rowY = vpTemp.r[1];
	C3D_FVec rowZ = vpTemp.r[2];
	C3D_FVec rowW = vpTemp.r[3];

	frustumPlanes[Frustum_Near] = FVec4_Normalize(FVec4_Subtract(rowW, rowZ));
	frustumPlanes[Frustum_Right] = FVec4_Normalize(FVec4_Add(rowW, rowX));
	frustumPlanes[Frustum_Left] = FVec4_Normalize(FVec4_Subtract(rowW, rowX));
	frustumPlanes[Frustum_Top] = FVec4_Normalize(FVec4_Add(rowW, rowY));
	frustumPlanes[Frustum_Bottom] = FVec4_Normalize(FVec4_Subtract(rowW, rowY));
	frustumPlanes[Frustum_Far] = FVec4_Normalize(FVec4_Add(rowW, rowZ));

    mc::Vector3d forward(player->view.x,player->view.y,player->view.z);
    mc::Vector3d right = Vector3d_crs(mc::Vector3d(0, 1, 0), mc::Vector3d(sinf(player->yaw), 0.f, cosf(player->yaw)));
    mc::Vector3d up = Vector3d_crs(forward, right);

	float ar = 400.f / 240.f;
	float tan2halffov = 2.f * tanf(fov / 2.f);

	float hNear = tan2halffov * near;
	float wNear = hNear * ar;

	float hFar = tan2halffov * far;
	float wFar = hFar * ar;

    mc::Vector3d cNear = playerHead + Vector3d_Scale(forward, near);
    mc::Vector3d cFar = playerHead + Vector3d_Scale(forward, far);

	frustumCorners[Frustum_NearBottomLeft] = ((cNear - Vector3d_Scale(up, hNear * 0.5f)) - Vector3d_Scale(right, wNear * 0.5f));
	frustumCorners[Frustum_NearBottomRight] = ((cNear - Vector3d_Scale(up, hNear * 0.5f)) + Vector3d_Scale(right, wNear * 0.5f));
	frustumCorners[Frustum_NearTopLeft] = ((cNear + Vector3d_Scale(up, hNear * 0.5f)) - Vector3d_Scale(right, wNear * 0.5f));
	frustumCorners[Frustum_NearTopRight] = ((cNear + Vector3d_Scale(up, hNear * 0.5f)) + Vector3d_Scale(right, wNear * 0.5f));
	frustumCorners[Frustum_FarBottomLeft] = ((cFar - Vector3d_Scale(up, hFar * 0.5f)) - Vector3d_Scale(right, wFar * 0.5f));
	frustumCorners[Frustum_FarBottomRight] = ((cFar - Vector3d_Scale(up, hFar * 0.5f)) + Vector3d_Scale(right, wFar * 0.5f));
	frustumCorners[Frustum_FarTopLeft] = ((cFar + Vector3d_Scale(up, hFar * 0.5f)) - Vector3d_Scale(right, wFar * 0.5f));
	frustumCorners[Frustum_FarTopRight] = ((cFar + Vector3d_Scale(up, hFar * 0.5f)) + Vector3d_Scale(right, wFar * 0.5f));
}

bool Camera::IsPointVisible(C3D_FVec point) const {
	point.w = 1.f;
	for (int i = 0; i < FrustumPlanes_Count; i++) {
		if (FVec4_Dot(point, frustumPlanes[i]) < 0.f) {
			return false;
		}
	}
	return true;
}

bool Camera::IsAABBVisible(C3D_FVec origin, C3D_FVec size) const {
    mc::Vector3d min(origin.x, origin.y, origin.z);
    mc::Vector3d max(origin.x + size.x, origin.y + size.y, origin.z + size.z);

    // Check if AABB is outside any frustum plane
    for (int i = 0; i < 6; i++) {
        int out = 0;
        for (int z = 0; z < 2; z++) {
            for (int y = 0; y < 2; y++) {
                for (int x = 0; x < 2; x++) {
                    C3D_FVec corner = FVec4_New(
                            x ? max.x : min.x,
                            y ? max.y : min.y,
                            z ? max.z : min.z,
                            1.0f
                    );
                    out += (FVec4_Dot(frustumPlanes[i], corner) < 0.0);
                }
            }
        }
        if (out == 8) return false;
    }

    // Check if frustum is outside any AABB plane
    for (int axis = 0; axis < 3; axis++) {
        int countAbove = 0, countBelow = 0;

        for (int i = 0; i < 8; i++) {
            float coord = (axis == 0) ? frustumCorners[i].x :
                          (axis == 1) ? frustumCorners[i].y :
                          frustumCorners[i].z;
            float axisMin = (axis == 0) ? min.x : (axis == 1) ? min.y : min.z;
            float axisMax = (axis == 0) ? max.x : (axis == 1) ? max.y : max.z;

            if (coord > axisMax) countAbove++;
            if (coord < axisMin) countBelow++;
        }

        if (countAbove == 8 || countBelow == 8) return false;
    }

    return true;
}