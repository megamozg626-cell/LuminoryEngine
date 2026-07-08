#pragma once

#include <memory>
#include <string>
#include <vector>
#include <map>
#include "Transform.hpp"

namespace Luminary {

class Component;
class Camera;

class Actor {
public:
    Actor(const std::string& name = "Actor");
    virtual ~Actor();

    void SetName(const std::string& name) { m_Name = name; }
    const std::string& GetName() const { return m_Name; }

    void SetActive(bool active) { m_Active = active; }
    bool IsActive() const { return m_Active; }

    std::shared_ptr<Transform> GetTransform() const { return m_Transform; }

    template<typename T, typename... Args>
    std::shared_ptr<T> AddComponent(Args&&... args) {
        static_assert(std::is_base_of_v<Component, T>, "T must be derived from Component");
        auto component = std::make_shared<T>(std::forward<Args>(args)...);
        m_Components.push_back(component);
        return component;
    }

    template<typename T>
    std::shared_ptr<T> GetComponent() const {
        for (const auto& component : m_Components) {
            auto casted = std::dynamic_pointer_cast<T>(component);
            if (casted) return casted;
        }
        return nullptr;
    }

    virtual void Update(float deltaTime);
    virtual void Render();

    const std::vector<std::shared_ptr<Component>>& GetComponents() const { return m_Components; }

private:
    std::string m_Name;
    bool m_Active = true;
    std::shared_ptr<Transform> m_Transform;
    std::vector<std::shared_ptr<Component>> m_Components;
};

} // namespace Luminary
