#pragma once

#include "../Scene/Component.hpp"
#include "../Math/Vector3.hpp"
#include "../Math/Quaternion.hpp"
#include <memory>

namespace Luminary {

class Character : public Component {
public:
    Character();
    ~Character();

    void SetWalkSpeed(float speed) { m_WalkSpeed = speed; }
    void SetRunSpeed(float speed) { m_RunSpeed = speed; }
    void SetSprintSpeed(float speed) { m_SprintSpeed = speed; }
    void SetJumpHeight(float height) { m_JumpHeight = height; }

    float GetWalkSpeed() const { return m_WalkSpeed; }
    float GetRunSpeed() const { return m_RunSpeed; }
    float GetSprintSpeed() const { return m_SprintSpeed; }
    float GetJumpHeight() const { return m_JumpHeight; }

    void Move(const Vector3& direction, bool bSprint = false);
    void Jump();
    void Crouch(bool bCrouch);
    void Rotate(const Vector3& rotation);

    void Update(float deltaTime) override;

    bool IsMoving() const { return m_CurrentVelocity.Length() > 0.1f; }
    bool IsJumping() const { return m_IsJumping; }
    bool IsCrouching() const { return m_IsCrouching; }

private:
    float m_WalkSpeed = 5.0f;
    float m_RunSpeed = 10.0f;
    float m_SprintSpeed = 15.0f;
    float m_JumpHeight = 2.0f;

    Vector3 m_CurrentVelocity = Vector3::Zero();
    Vector3 m_InputDirection = Vector3::Zero();
    bool m_IsJumping = false;
    bool m_IsCrouching = false;
    float m_CrouchHeight = 1.0f;
    float m_NormalHeight = 2.0f;
};

class PlayerCharacter : public Character {
public:
    PlayerCharacter();
    ~PlayerCharacter();

    void SetCameraOffset(const Vector3& offset) { m_CameraOffset = offset; }
    const Vector3& GetCameraOffset() const { return m_CameraOffset; }

    void Update(float deltaTime) override;

private:
    Vector3 m_CameraOffset = Vector3(0.0f, 1.6f, 0.0f);
    Vector3 m_CameraRotation = Vector3::Zero();
};

} // namespace Luminary
