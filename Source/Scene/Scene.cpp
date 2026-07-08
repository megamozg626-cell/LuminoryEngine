#include "Scene.hpp"
#include "../Core/Logging/Logger.hpp"

namespace Luminary {

Scene::Scene(const std::string& name) : m_Name(name) {
    LOG_INFO("Scene created: {}", name);
}

Scene::~Scene() {
    m_Actors.clear();
    LOG_INFO("Scene destroyed: {}", m_Name);
}

std::shared_ptr<Actor> Scene::SpawnActor(const std::string& name) {
    auto actor = std::make_shared<Actor>(name);
    m_Actors.push_back(actor);
    LOG_DEBUG("Actor spawned: {} in scene {}", name, m_Name);
    return actor;
}

void Scene::DestroyActor(std::shared_ptr<Actor> actor) {
    if (!actor) return;
    m_DestroyQueue.push_back(actor);
}

std::shared_ptr<Actor> Scene::FindActorByName(const std::string& name) const {
    for (const auto& actor : m_Actors) {
        if (actor->GetName() == name) {
            return actor;
        }
    }
    return nullptr;
}

void Scene::Update(float deltaTime) {
    for (auto& actor : m_Actors) {
        actor->Update(deltaTime);
    }

    if (m_Physics) {
        m_Physics->Update(deltaTime);
    }

    ProcessDestroyQueue();
}

void Scene::Render() {
    for (auto& actor : m_Actors) {
        actor->Render();
    }
}

void Scene::ProcessDestroyQueue() {
    for (const auto& actor : m_DestroyQueue) {
        auto it = std::find(m_Actors.begin(), m_Actors.end(), actor);
        if (it != m_Actors.end()) {
            m_Actors.erase(it);
            LOG_DEBUG("Actor destroyed: {}", actor->GetName());
        }
    }
    m_DestroyQueue.clear();
}

} // namespace Luminary
