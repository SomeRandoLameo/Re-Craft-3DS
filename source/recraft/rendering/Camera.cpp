#include "rendering/Camera.hpp"

#include "world/CT_Chunk.hpp"

#include "gui/DebugUI.hpp"

Camera::Camera() {
	Mtx_Identity(&m_view);

	m_fov = C3D_AngleFromDegrees(60.f);
	m_near = 0.2f;
	m_far = 8.f * Chunk::Size;

	Mtx_PerspTilt(&m_projection, m_fov, (400.f / 240.f), m_near, m_far, false);
}

void Camera::Update(Player* player, float iod) {
	float currentFov = m_fov + C3D_AngleFromDegrees(12.f) * player->fovAdd;
	Mtx_PerspStereoTilt(&m_projection, currentFov, (400.f / 240.f), m_near, m_far, iod, 1.f, false);

	// View bobbing at player head
	mc::Vector3f playerHead(
			player->position.x,
			player->position.y + Player::EyeHeight + sinf(player->bobbing) * 0.1f + player->crouchAdd,
			player->position.z
	);

	Mtx_Identity(&m_view);
	Mtx_RotateX(&m_view, -player->xRot, true);
	Mtx_RotateY(&m_view, -player->yRot, true);
	Mtx_Translate(&m_view, -playerHead.x, -playerHead.y, -playerHead.z, true);

	C3D_Mtx vpTemp;
	Mtx_Multiply(&vpTemp, &m_projection, &m_view);
	Mtx_Copy(&m_vp, &vpTemp);

	m_frustum.UpdateFromMatrix(vpTemp);

	mc::Vector3f forward(player->view.x, player->view.y, player->view.z);
	mc::Vector3f right = Vector3f_crs(vecY, mc::Vector3f(sinf(player->yRot), 0.f, cosf(player->yRot)));
	mc::Vector3f up = Vector3f_crs(forward, right);

    m_frustum.UpdateCorners(playerHead, forward, right, up, m_fov, 400.f / 240.f, m_near, m_far);
}
