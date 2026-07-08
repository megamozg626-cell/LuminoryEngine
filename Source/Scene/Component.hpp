#pragma once

#include <memory>
#include <functional>

namespace Luminary {

class Actor;

class Component : public std::enable_shared_from_this<Component> {
public:
    Component();
    virtual ~Component();

    void SetEnabled(bool enabled) { m_Enabled = enabled; }
    bool IsEnabled() const { return m_Enabled; }

    void SetOwner(std::weak_ptr<Actor> owner) { m_Owner = owner; }
    std::weak_ptr<Actor> GetOwner() const { return m_Owner; }

    virtual void Update(float deltaTime) {}
    virtual void Render() {}
    virtual void OnEnable() {}
    virtual void OnDisable() {}

private:
    bool m_Enabled = true;
    std::weak_ptr<Actor> m_Owner;
};

} // namespace Luminary
