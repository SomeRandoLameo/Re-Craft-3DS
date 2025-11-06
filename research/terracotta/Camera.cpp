#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

namespace terra {

Camera::Camera(glm::vec3 position, float fov, float ratio, float near, float far, glm::vec3 worldUp, float yaw, float pitch)
    : m_Position(position),
      m_WorldUp(worldUp),
      m_Yaw(yaw),
      m_Pitch(pitch),
      m_Near(near),
      m_Far(far),
      m_Fov(fov),
      m_AspectRatio(ratio),
      m_MovementSpeed(5.0f),
      m_MouseSensitivity(0.005f),
      m_Zoom(45.0f)
{
    UpdateVectors();
}

math::volumes::Frustum Camera::GetFrustum() const {
    return math::volumes::Frustum(m_Position, m_Front, m_Near, m_Far, m_Fov, m_AspectRatio, m_Up, m_Right);
}

void Camera::ProcessMovement(CameraMovement direction, float dt) {
    float speed = m_MovementSpeed * dt;

    if (direction == CameraMovement::Forward) {
        m_Position += m_Front * speed;
    } else if (direction == CameraMovement::Backward) {
        m_Position -= m_Front * speed;
    } else if (direction == CameraMovement::Left) {
        m_Position -= m_Right * speed;
    } else if (direction == CameraMovement::Right) {
        m_Position += m_Right * speed;
    } else if (direction == CameraMovement::Raise) {
        m_Position += m_WorldUp * speed;
    } else if (direction == CameraMovement::Lower) {
        m_Position -= m_WorldUp * speed;
    }
}

void Camera::ProcessRotation(float xoffset, float yoffset) {
    m_Yaw += xoffset * m_MouseSensitivity;
    m_Pitch += yoffset * m_MouseSensitivity;

    const float kMaxPitch = (float)glm::radians(89.0);

    m_Pitch = std::min(std::max(m_Pitch, -kMaxPitch), kMaxPitch);
    
    UpdateVectors();
}

void Camera::ProcessZoom(float yoffset) {
    const float kMinZoom = 1.0f;
    const float kMaxZoom = 60.0f;

    if (m_Zoom >= kMinZoom && m_Zoom <= kMaxZoom) {
        m_Zoom -= yoffset;
    }

    m_Zoom = std::min(std::max(m_Zoom, kMinZoom), kMaxZoom);
}

glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

glm::mat4 Camera::GetPerspectiveMatrix() const {
    return glm::perspective(m_Fov, m_AspectRatio, m_Near, m_Far);
}

void Camera::UpdateVectors() {
    m_Front.x = std::cos(m_Yaw) * std::cos(m_Pitch);
    m_Front.y = std::sin(m_Pitch);
    m_Front.z = std::sin(m_Yaw) * std::cos(m_Pitch);
    m_Front = glm::normalize(m_Front);

    m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
    m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}

} // ns terra
