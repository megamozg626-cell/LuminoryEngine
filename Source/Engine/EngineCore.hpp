#pragma once

#include <memory>
#include <chrono>
#include <string>

namespace Luminary {

class GameTime {
public:
    GameTime();
    ~GameTime();

    void Update();
    void SetTimeScale(float scale) { m_TimeScale = scale; }

    float GetDeltaTime() const { return m_DeltaTime; }
    float GetUnscaledDeltaTime() const { return m_UnscaledDeltaTime; }
    float GetTimeScale() const { return m_TimeScale; }
    double GetTotalTime() const { return m_TotalTime; }
    uint64_t GetFrameCount() const { return m_FrameCount; }
    float GetFrameRate() const { return m_FrameRate; }

private:
    float m_DeltaTime = 0.0f;
    float m_UnscaledDeltaTime = 0.0f;
    float m_TimeScale = 1.0f;
    double m_TotalTime = 0.0;
    uint64_t m_FrameCount = 0;
    float m_FrameRate = 60.0f;
    float m_FrameRateAccumulator = 0.0f;
    uint32_t m_FramesSinceLastUpdate = 0;

    std::chrono::high_resolution_clock::time_point m_LastTime;
};

class EngineCore {
public:
    EngineCore();
    ~EngineCore();

    bool Initialize();
    void Shutdown();
    void Update();
    void Render();
    bool IsRunning() const { return m_IsRunning; }
    void Exit() { m_IsRunning = false; }

    std::shared_ptr<GameTime> GetGameTime() const { return m_GameTime; }
    std::shared_ptr<class Renderer> GetRenderer() const { return m_Renderer; }
    std::shared_ptr<class InputSystem> GetInputSystem() const { return m_InputSystem; }
    std::shared_ptr<class AudioSystem> GetAudioSystem() const { return m_AudioSystem; }
    std::shared_ptr<class AssetManager> GetAssetManager() const { return m_AssetManager; }
    std::shared_ptr<class Editor> GetEditor() const { return m_Editor; }

private:
    bool m_IsRunning = false;

    std::shared_ptr<GameTime> m_GameTime;
    std::shared_ptr<class Renderer> m_Renderer;
    std::shared_ptr<class InputSystem> m_InputSystem;
    std::shared_ptr<class AudioSystem> m_AudioSystem;
    std::shared_ptr<class AssetManager> m_AssetManager;
    std::shared_ptr<class Editor> m_Editor;
};

} // namespace Luminary
