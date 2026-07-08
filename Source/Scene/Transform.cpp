#include "Transform.hpp"

namespace Luminary {

Transform::Transform() {
    UpdateMatrices();
}

Transform::~Transform() {
    if (m_Parent) {
        // Remove from parent's children
        auto& parentChildren = m_Parent->m_Children;
        parentChildren.erase(
            std::remove(parentChildren.begin(), parentChildren.end(), std::make_shared<Transform>(*this)),
            parentChildren.end()
        );
    }
}

void Transform::SetPosition(const Vector3& position) {
    m_Position = position;
    UpdateMatrices();
}

void Transform::SetRotation(const Quaternion& rotation) {
    m_Rotation = rotation;
    UpdateMatrices();
}

void Transform::SetScale(const Vector3& scale) {
    m_Scale = scale;
    UpdateMatrices();
}

void Transform::SetParent(std::shared_ptr<Transform> parent) {
    m_Parent = parent;
    if (parent) {
        parent->AddChild(std::make_shared<Transform>(*this));
    }
    UpdateMatrices();
}

void Transform::AddChild(std::shared_ptr<Transform> child) {
    m_Children.push_back(child);
    UpdateMatrices();
}

void Transform::UpdateMatrices() {
    Matrix4 translationMatrix = Matrix4::Translation(m_Position);
    Matrix4 rotationMatrix = Matrix4::FromQuaternion(m_Rotation);
    Matrix4 scaleMatrix = Matrix4::Scale(m_Scale);

    m_LocalMatrix = translationMatrix * rotationMatrix * scaleMatrix;

    if (m_Parent) {
        m_WorldMatrix = m_Parent->GetWorldMatrix() * m_LocalMatrix;
    } else {
        m_WorldMatrix = m_LocalMatrix;
    }

    for (auto& child : m_Children) {
        child->UpdateMatrices();
    }
}

} // namespace Luminary
