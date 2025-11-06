#ifndef TERRACOTTA_CAMERA_H_
#define TERRACOTTA_CAMERA_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "math/volumes/Frustum.h"

#undef near
#undef far

namespace terra {

enum class CameraMovement {
    Forward,
    Backward,
    Left,
    Right,
    Raise,
    Lower
};

class Camera {
public:
    Camera(glm::vec3 position, float fov, float ratio, float near, float far, glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -1.5708, float pitch = 0.0f);

    void ProcessMovement(CameraMovement direction, float dt);
    void ProcessRotation(float xoffset, float yoffset);
    void ProcessZoom(float yoffset);

    const glm::vec3& GetPosition() const { return m_Position; }
    glm::vec3 GetFront() const { return m_Front; }
    glm::vec3 GetRight() const { return m_Right; }
    float GetZoom() const { return m_Zoom; }
    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetPerspectiveMatrix() const;

    void SetPosition(glm::vec3 pos) { m_Position = pos; }

    float GetYaw() const { return m_Yaw; }
    float GetPitch() const { return m_Pitch; }

    math::volumes::Frustum GetFrustum() const;

    void SetMouseSensitivity(float sensitivity) { m_MouseSensitivity = sensitivity; }

    void SetFov(float fov) { m_Fov = fov; }
private:
    glm::vec3 m_Position;
    glm::vec3 m_Front;
    glm::vec3 m_WorldUp;
    glm::vec3 m_Up;
    glm::vec3 m_Right;

    float m_Yaw;
    float m_Pitch;
    float m_Near;
    float m_Far;
    float m_Fov;
    float m_AspectRatio;

    float m_MovementSpeed;
    float m_MouseSensitivity;
    float m_Zoom;

    void UpdateVectors();
};

} // ns terra

#endif
