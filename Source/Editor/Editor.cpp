#include "Editor.hpp"
#include "../Core/Logging/Logger.hpp"
#include "../Scene/Actor.hpp"
#include "../Scene/Scene.hpp"

namespace Luminary {

EditorWindow::EditorWindow(const std::string& name) : m_Name(name) {
    LOG_DEBUG("EditorWindow created: {}", name);
}

EditorWindow::~EditorWindow() = default;

SceneViewWindow::SceneViewWindow() : EditorWindow("Scene") {
    LOG_DEBUG("SceneViewWindow created");
}

SceneViewWindow::~SceneViewWindow() = default;

void SceneViewWindow::OnGUI() {
    LOG_DEBUG("SceneViewWindow::OnGUI");
}

void SceneViewWindow::Update(float deltaTime) {
    // Update scene view
}

InspectorWindow::InspectorWindow() : EditorWindow("Inspector") {
    LOG_DEBUG("InspectorWindow created");
}

InspectorWindow::~InspectorWindow() = default;

void InspectorWindow::OnGUI() {
    if (m_SelectedActor) {
        LOG_DEBUG("Inspector showing: {}", m_SelectedActor->GetName());
    }
}

HierarchyWindow::HierarchyWindow() : EditorWindow("Hierarchy") {
    LOG_DEBUG("HierarchyWindow created");
}

HierarchyWindow::~HierarchyWindow() = default;

void HierarchyWindow::OnGUI() {
    if (m_Scene) {
        const auto& actors = m_Scene->GetActors();
        for (const auto& actor : actors) {
            LOG_DEBUG("Hierarchy: {}", actor->GetName());
        }
    }
}

PropertiesWindow::PropertiesWindow() : EditorWindow("Properties") {
    LOG_DEBUG("PropertiesWindow created");
}

PropertiesWindow::~PropertiesWindow() = default;

void PropertiesWindow::OnGUI() {
    LOG_DEBUG("PropertiesWindow::OnGUI");
}

ConsoleWindow::ConsoleWindow() : EditorWindow("Console") {
    LOG_DEBUG("ConsoleWindow created");
}

ConsoleWindow::~ConsoleWindow() = default;

void ConsoleWindow::AddLog(const std::string& message) {
    m_Logs.push_back(message);
    if (m_Logs.size() > 1000) {
        m_Logs.erase(m_Logs.begin());
    }
}

void ConsoleWindow::OnGUI() {
    LOG_DEBUG("ConsoleWindow showing {} logs", m_Logs.size());
}

Editor::Editor() {
    LOG_INFO("Editor created");
}

Editor::~Editor() {
    Shutdown();
}

bool Editor::Initialize() {
    LOG_INFO("Editor initialized");

    // Register default windows
    RegisterWindow(std::make_shared<SceneViewWindow>());
    RegisterWindow(std::make_shared<HierarchyWindow>());
    RegisterWindow(std::make_shared<InspectorWindow>());
    RegisterWindow(std::make_shared<PropertiesWindow>());
    RegisterWindow(std::make_shared<ConsoleWindow>());

    return true;
}

void Editor::Shutdown() {
    m_Windows.clear();
    LOG_INFO("Editor shut down");
}

void Editor::Update(float deltaTime) {
    if (!m_IsEnabled) return;

    for (auto& [name, window] : m_Windows) {
        if (window->IsOpen()) {
            window->Update(deltaTime);
        }
    }
}

void Editor::OnGUI() {
    if (!m_IsEnabled) return;

    for (auto& [name, window] : m_Windows) {
        if (window->IsOpen()) {
            window->OnGUI();
        }
    }
}

void Editor::RegisterWindow(std::shared_ptr<EditorWindow> window) {
    if (window) {
        m_Windows[window->GetName()] = window;
    }
}

std::shared_ptr<EditorWindow> Editor::GetWindow(const std::string& name) const {
    auto it = m_Windows.find(name);
    return it != m_Windows.end() ? it->second : nullptr;
}

} // namespace Luminary
