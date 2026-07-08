#include "Actor.hpp"
#include "Component.hpp"

namespace Luminary {

Actor::Actor(const std::string& name) : m_Name(name) {
    m_Transform = std::make_shared<Transform>();
}

Actor::~Actor() = default;

void Actor::Update(float deltaTime) {
    if (!m_Active) return;

    for (auto& component : m_Components) {
        component->Update(deltaTime);
    }
}

void Actor::Render() {
    if (!m_Active) return;

    for (auto& component : m_Components) {
        component->Render();
    }
}

} // namespace Luminary
