#pragma once

#include <memory>
#include <vector>
#include <string>
#include <map>
#include <functional>

namespace Luminary {

class Scene;
class Renderer;
class Physics;
class InputSystem;
class AudioSystem;
class AssetManager;

class Engine {
public:
    Engine();
    ~Engine();

    bool Initialize(uint32_t windowWidth, uint32_t windowHeight, const std::string& windowTitle);
    void Run();
    void Shutdown();

    // Main loop control
    void Tick(float deltaTime);
    void Render();
    void Update(float deltaTime);

    // Getters
    std::shared_ptr<Scene> GetCurrentScene() const { return m_CurrentScene; }
    std::shared_ptr<Renderer> GetRenderer() const { return m_Renderer; }
    std::shared_ptr<Physics> GetPhysics() const { return m_Physics; }
    std::shared_ptr<InputSystem> GetInputSystem() const { return m_InputSystem; }
    std::shared_ptr<AudioSystem> GetAudioSystem() const { return m_AudioSystem; }
    std::shared_ptr<AssetManager> GetAssetManager() const { return m_AssetManager; }

    float GetDeltaTime() const { return m_DeltaTime; }
    float GetTotalTime() const { return m_TotalTime; }
    uint32_t GetFrameCount() const { return m_FrameCount; }
    float GetFPS() const { return m_FPS; }

    bool IsRunning() const { return m_IsRunning; }
    void RequestShutdown() { m_IsRunning = false; }

private:
    bool m_IsRunning = false;
    float m_DeltaTime = 0.0f;
    float m_TotalTime = 0.0f;
    uint32_t m_FrameCount = 0;
    float m_FPS = 0.0f;
    float m_FrameTimeAccumulator = 0.0f;

    std::shared_ptr<Scene> m_CurrentScene;
    std::shared_ptr<Renderer> m_Renderer;
    std::shared_ptr<Physics> m_Physics;
    std::shared_ptr<InputSystem> m_InputSystem;
    std::shared_ptr<AudioSystem> m_AudioSystem;
    std::shared_ptr<AssetManager> m_AssetManager;

    void InitializeSystems();
    void UpdateSystems(float deltaTime);
    void ShutdownSystems();
};

} // namespace Luminary
