#include "Character.hpp"
#include "../Scene/Actor.hpp"
#include "../Scene/Transform.hpp"
#include "../Core/Logging/Logger.hpp"

namespace Luminary {

Character::Character() {
    LOG_DEBUG("Character created");
}

Character::~Character() = default;

void Character::Move(const Vector3& direction, bool bSprint) {
    m_InputDirection = direction.Normalize();
    float speed = bSprint ? m_SprintSpeed : (direction.Length() > 0.5f ? m_RunSpeed : m_WalkSpeed);
    m_CurrentVelocity = m_InputDirection * speed;
}

void Character::Jump() {
    if (!m_IsJumping) {
        m_IsJumping = true;
        m_CurrentVelocity.y = std::sqrt(2.0f * 9.81f * m_JumpHeight);
    }
}

void Character::Crouch(bool bCrouch) {
    m_IsCrouching = bCrouch;
}

void Character::Rotate(const Vector3& rotation) {
    if (auto owner = GetOwner().lock()) {
        auto transform = owner->GetTransform();
        transform->SetRotation(Quaternion::FromEulerAngles(rotation.x, rotation.y, rotation.z));
    }
}

void Character::Update(float deltaTime) {
    if (auto owner = GetOwner().lock()) {
        auto transform = owner->GetTransform();
        Vector3 newPos = transform->GetPosition() + m_CurrentVelocity * deltaTime;
        transform->SetPosition(newPos);
    }
}

PlayerCharacter::PlayerCharacter() {
    LOG_DEBUG("PlayerCharacter created");
}

PlayerCharacter::~PlayerCharacter() = default;

void PlayerCharacter::Update(float deltaTime) {
    Character::Update(deltaTime);
    // Update camera position based on character position
}

} // namespace Luminary
