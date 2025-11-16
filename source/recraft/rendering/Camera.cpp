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

	float3 playerHead = f3_new(
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

	float3 forward = f3_new(player->view.x,player->view.y,player->view.z);
	float3 right = f3_crs(f3_new(0, 1, 0), f3_new(sinf(player->yaw), 0.f, cosf(player->yaw)));
	float3 up = f3_crs(forward, right);

	float ar = 400.f / 240.f;
	float tan2halffov = 2.f * tanf(fov / 2.f);

	float hNear = tan2halffov * near;
	float wNear = hNear * ar;

	float hFar = tan2halffov * far;
	float wFar = hFar * ar;

	float3 cNear = f3_add(playerHead, f3_scl(forward, near));
	float3 cFar = f3_add(playerHead, f3_scl(forward, far));

	frustumCorners[Frustum_NearBottomLeft] = f3_sub(f3_sub(cNear, f3_scl(up, hNear * 0.5f)), f3_scl(right, wNear * 0.5f));
	frustumCorners[Frustum_NearBottomRight] = f3_add(f3_sub(cNear, f3_scl(up, hNear * 0.5f)), f3_scl(right, wNear * 0.5f));
	frustumCorners[Frustum_NearTopLeft] = f3_sub(f3_add(cNear, f3_scl(up, hNear * 0.5f)), f3_scl(right, wNear * 0.5f));
	frustumCorners[Frustum_NearTopRight] = f3_add(f3_add(cNear, f3_scl(up, hNear * 0.5f)), f3_scl(right, wNear * 0.5f));
	frustumCorners[Frustum_FarBottomLeft] = f3_sub(f3_sub(cFar, f3_scl(up, hFar * 0.5f)), f3_scl(right, wFar * 0.5f));
	frustumCorners[Frustum_FarBottomRight] = f3_add(f3_sub(cFar, f3_scl(up, hFar * 0.5f)), f3_scl(right, wFar * 0.5f));
	frustumCorners[Frustum_FarTopLeft] = f3_sub(f3_add(cFar, f3_scl(up, hFar * 0.5f)), f3_scl(right, wFar * 0.5f));
	frustumCorners[Frustum_FarTopRight] = f3_add(f3_add(cFar, f3_scl(up, hFar * 0.5f)), f3_scl(right, wFar * 0.5f));
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
	float3 min = f3_new(origin.x, origin.y, origin.z);
	float3 max = f3_new(origin.x + size.x, origin.y + size.y, origin.z + size.z);

	for (int i = 0; i < 6; i++) {
		int out = 0;
		out += ((FVec4_Dot(frustumPlanes[i], FVec4_New(min.x, min.y, min.z, 1.0f)) < 0.0));
		out += ((FVec4_Dot(frustumPlanes[i], FVec4_New(max.x, min.y, min.z, 1.0f)) < 0.0));
		out += ((FVec4_Dot(frustumPlanes[i], FVec4_New(min.x, max.y, min.z, 1.0f)) < 0.0));
		out += ((FVec4_Dot(frustumPlanes[i], FVec4_New(max.x, max.y, min.z, 1.0f)) < 0.0));
		out += ((FVec4_Dot(frustumPlanes[i], FVec4_New(min.x, min.y, max.z, 1.0f)) < 0.0));
		out += ((FVec4_Dot(frustumPlanes[i], FVec4_New(max.x, min.y, max.z, 1.0f)) < 0.0));
		out += ((FVec4_Dot(frustumPlanes[i], FVec4_New(min.x, max.y, max.z, 1.0f)) < 0.0));
		out += ((FVec4_Dot(frustumPlanes[i], FVec4_New(max.x, max.y, max.z, 1.0f)) < 0.0));
		if (out == 8) return false;
	}

	// http://iquilezles.org/www/articles/frustumcorrect/frustumcorrect.htm
	int out;
	out = 0;
	for (int i = 0; i < 8; i++) out += ((frustumCorners[i].x > max.x));
	if (out == 8) return false;
	out = 0;
	for (int i = 0; i < 8; i++) out += ((frustumCorners[i].x < min.x));
	if (out == 8) return false;
	out = 0;
	for (int i = 0; i < 8; i++) out += ((frustumCorners[i].y > max.y));
	if (out == 8) return false;
	out = 0;
	for (int i = 0; i < 8; i++) out += ((frustumCorners[i].y < min.y));
	if (out == 8) return false;
	out = 0;
	for (int i = 0; i < 8; i++) out += ((frustumCorners[i].z > max.z));
	if (out == 8) return false;
	out = 0;
	for (int i = 0; i < 8; i++) out += ((frustumCorners[i].z < min.z));
	if (out == 8) return false;

	return true;
}