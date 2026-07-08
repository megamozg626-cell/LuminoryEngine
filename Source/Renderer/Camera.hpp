#pragma once

#include "../Math/Vector3.hpp"
#include "../Math/Matrix4.hpp"
#include "../Math/Quaternion.hpp"
#include <memory>

namespace Luminary {

class Camera {
public:
    enum class ProjectionType {
        Perspective,
        Orthographic
    };

    Camera();
    ~Camera() = default;

    void SetPosition(const Vector3& position) { m_Position = position; UpdateViewMatrix(); }
    void SetRotation(const Quaternion& rotation) { m_Rotation = rotation; UpdateViewMatrix(); }
    void SetTarget(const Vector3& target) { m_Target = target; UpdateViewMatrix(); }
    void SetUpVector(const Vector3& up) { m_Up = up; UpdateViewMatrix(); }

    const Vector3& GetPosition() const { return m_Position; }
    const Quaternion& GetRotation() const { return m_Rotation; }
    const Vector3& GetTarget() const { return m_Target; }
    const Vector3& GetUpVector() const { return m_Up; }

    void SetProjectionType(ProjectionType type) { m_ProjectionType = type; UpdateProjectionMatrix(); }
    void SetFieldOfView(float fov) { m_FieldOfView = fov; UpdateProjectionMatrix(); }
    void SetAspectRatio(float aspect) { m_AspectRatio = aspect; UpdateProjectionMatrix(); }
    void SetNearPlane(float near) { m_NearPlane = near; UpdateProjectionMatrix(); }
    void SetFarPlane(float far) { m_FarPlane = far; UpdateProjectionMatrix(); }

    const Matrix4& GetViewMatrix() const { return m_ViewMatrix; }
    const Matrix4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
    Matrix4 GetViewProjectionMatrix() const { return m_ProjectionMatrix * m_ViewMatrix; }

    Vector3 GetForward() const { return m_Rotation * Vector3::Forward(); }
    Vector3 GetRight() const { return m_Rotation * Vector3::Right(); }
    Vector3 GetUp() const { return m_Rotation * Vector3::Up(); }

private:
    Vector3 m_Position = Vector3(0.0f, 0.0f, 10.0f);
    Quaternion m_Rotation = Quaternion::Identity();
    Vector3 m_Target = Vector3::Zero();
    Vector3 m_Up = Vector3::Up();

    ProjectionType m_ProjectionType = ProjectionType::Perspective;
    float m_FieldOfView = 45.0f;
    float m_AspectRatio = 16.0f / 9.0f;
    float m_NearPlane = 0.1f;
    float m_FarPlane = 10000.0f;

    Matrix4 m_ViewMatrix;
    Matrix4 m_ProjectionMatrix;

    void UpdateViewMatrix();
    void UpdateProjectionMatrix();
};

} // namespace Luminary
