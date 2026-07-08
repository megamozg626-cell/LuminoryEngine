#pragma once

namespace Luminary {

class CharacterController {
public:
    CharacterController();
    ~CharacterController();

    void SetHeight(float height) { m_Height = height; }
    void SetRadius(float radius) { m_Radius = radius; }
    void SetStepOffset(float offset) { m_StepOffset = offset; }

    float GetHeight() const { return m_Height; }
    float GetRadius() const { return m_Radius; }
    float GetStepOffset() const { return m_StepOffset; }

    void Move(const Vector3& velocity);

private:
    float m_Height = 2.0f;
    float m_Radius = 0.4f;
    float m_StepOffset = 0.5f;
    Vector3 m_Velocity = Vector3::Zero();
};

} // namespace Luminary
