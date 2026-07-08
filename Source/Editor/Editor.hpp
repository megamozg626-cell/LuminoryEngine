#pragma once

#include <memory>
#include <string>
#include <vector>
#include <map>

namespace Luminary {

class EditorWindow {
public:
    EditorWindow(const std::string& name);
    virtual ~EditorWindow();

    const std::string& GetName() const { return m_Name; }
    bool IsOpen() const { return m_IsOpen; }
    void SetOpen(bool open) { m_IsOpen = open; }

    virtual void OnGUI() {}
    virtual void Update(float deltaTime) {}

protected:
    std::string m_Name;
    bool m_IsOpen = true;
};

class SceneViewWindow : public EditorWindow {
public:
    SceneViewWindow();
    ~SceneViewWindow();

    void OnGUI() override;
    void Update(float deltaTime) override;

private:
    bool m_ShowGrid = true;
    bool m_ShowGizmos = true;
    float m_GridSize = 1.0f;
};

class InspectorWindow : public EditorWindow {
public:
    InspectorWindow();
    ~InspectorWindow();

    void SetSelectedActor(std::shared_ptr<class Actor> actor) { m_SelectedActor = actor; }
    void OnGUI() override;

private:
    std::shared_ptr<class Actor> m_SelectedActor;
};

class HierarchyWindow : public EditorWindow {
public:
    HierarchyWindow();
    ~HierarchyWindow();

    void SetScene(std::shared_ptr<class Scene> scene) { m_Scene = scene; }
    void OnGUI() override;
    std::shared_ptr<class Actor> GetSelectedActor() const { return m_SelectedActor; }

private:
    std::shared_ptr<class Scene> m_Scene;
    std::shared_ptr<class Actor> m_SelectedActor;
};

class PropertiesWindow : public EditorWindow {
public:
    PropertiesWindow();
    ~PropertiesWindow();

    void OnGUI() override;

private:
};

class ConsoleWindow : public EditorWindow {
public:
    ConsoleWindow();
    ~ConsoleWindow();

    void AddLog(const std::string& message);
    void OnGUI() override;

private:
    std::vector<std::string> m_Logs;
    bool m_AutoScroll = true;
};

class Editor {
public:
    Editor();
    ~Editor();

    bool Initialize();
    void Shutdown();
    void Update(float deltaTime);
    void OnGUI();

    void RegisterWindow(std::shared_ptr<EditorWindow> window);
    std::shared_ptr<EditorWindow> GetWindow(const std::string& name) const;

    bool IsEnabled() const { return m_IsEnabled; }
    void SetEnabled(bool enabled) { m_IsEnabled = enabled; }

private:
    bool m_IsEnabled = true;
    std::map<std::string, std::shared_ptr<EditorWindow>> m_Windows;
};

} // namespace Luminary
