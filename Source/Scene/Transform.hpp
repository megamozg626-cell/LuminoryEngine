#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>
#include <vector>

namespace Luminary {

class Component;

class Transform {
public:
    Transform();
    ~Transform();

    void SetPosition(const Vector3& position);
    void SetRotation(const Quaternion& rotation);
    void SetScale(const Vector3& scale);

    const Vector3& GetPosition() const { return m_Position; }
    const Quaternion& GetRotation() const { return m_Rotation; }
    const Vector3& GetScale() const { return m_Scale; }

    const Matrix4& GetLocalMatrix() const { return m_LocalMatrix; }
    const Matrix4& GetWorldMatrix() const { return m_WorldMatrix; }

    Vector3 GetForward() const { return m_Rotation * Vector3::Forward(); }
    Vector3 GetRight() const { return m_Rotation * Vector3::Right(); }
    Vector3 GetUp() const { return m_Rotation * Vector3::Up(); }

    void SetParent(std::shared_ptr<Transform> parent);
    std::shared_ptr<Transform> GetParent() const { return m_Parent; }

    void AddChild(std::shared_ptr<Transform> child);
    const std::vector<std::shared_ptr<Transform>>& GetChildren() const { return m_Children; }

    void UpdateMatrices();

private:
    Vector3 m_Position = Vector3::Zero();
    Quaternion m_Rotation = Quaternion::Identity();
    Vector3 m_Scale = Vector3::One();

    Matrix4 m_LocalMatrix;
    Matrix4 m_WorldMatrix;

    std::shared_ptr<Transform> m_Parent;
    std::vector<std::shared_ptr<Transform>> m_Children;
};

} // namespace Luminary
