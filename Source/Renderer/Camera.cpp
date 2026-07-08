#include "Camera.hpp"

namespace Luminary {

Camera::Camera() {
    UpdateViewMatrix();
    UpdateProjectionMatrix();
}

void Camera::UpdateViewMatrix() {
    m_ViewMatrix = Matrix4::LookAt(m_Position, m_Target, m_Up);
}

void Camera::UpdateProjectionMatrix() {
    if (m_ProjectionType == ProjectionType::Perspective) {
        m_ProjectionMatrix = Matrix4::Perspective(m_FieldOfView, m_AspectRatio, m_NearPlane, m_FarPlane);
    } else {
        float width = 10.0f;
        float height = width / m_AspectRatio;
        m_ProjectionMatrix = Matrix4::Orthographic(-width / 2.0f, width / 2.0f, -height / 2.0f, height / 2.0f, m_NearPlane, m_FarPlane);
    }
}

} // namespace Luminary
