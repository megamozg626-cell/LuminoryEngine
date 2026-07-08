#pragma once

#include "Actor.hpp"
#include "../Physics/Physics.hpp"
#include <memory>
#include <vector>
#include <string>
#include <map>

namespace Luminary {

class Scene {
public:
    explicit Scene(const std::string& name = "Scene");
    ~Scene();

    const std::string& GetName() const { return m_Name; }

    std::shared_ptr<Actor> SpawnActor(const std::string& name = "Actor");
    void DestroyActor(std::shared_ptr<Actor> actor);

    const std::vector<std::shared_ptr<Actor>>& GetActors() const { return m_Actors; }
    std::shared_ptr<Actor> FindActorByName(const std::string& name) const;

    void Update(float deltaTime);
    void Render();

    void SetPhysics(std::shared_ptr<Physics> physics) { m_Physics = physics; }
    std::shared_ptr<Physics> GetPhysics() const { return m_Physics; }

private:
    std::string m_Name;
    std::vector<std::shared_ptr<Actor>> m_Actors;
    std::vector<std::shared_ptr<Actor>> m_DestroyQueue;
    std::shared_ptr<Physics> m_Physics;

    void ProcessDestroyQueue();
};

} // namespace Luminary
