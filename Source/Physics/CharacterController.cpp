#include "CharacterController.hpp"

namespace Luminary {

CharacterController::CharacterController() = default;
CharacterController::~CharacterController() = default;

void CharacterController::Move(const Vector3& velocity) {
    m_Velocity = velocity;
}

} // namespace Luminary
