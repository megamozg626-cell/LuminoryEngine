#include "EngineCore.hpp"
#include "../Core/Logging/Logger.hpp"
#include "../Renderer/RenderCore.hpp"
#include "../Input/InputSystem.hpp"
#include "../Audio/AudioSystem.hpp"
#include "../AssetSystem/AssetManager.hpp"
#include "../Editor/Editor.hpp"
#include <chrono>

namespace Luminary {

GameTime::GameTime() {
    m_LastTime = std::chrono::high_resolution_clock::now();
}

GameTime::~GameTime() = default;

void GameTime::Update() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> elapsed = currentTime - m_LastTime;
    m_LastTime = currentTime;

    m_UnscaledDeltaTime = elapsed.count();
    m_DeltaTime = m_UnscaledDeltaTime * m_TimeScale;
    m_TotalTime += m_DeltaTime;
    m_FrameCount++;

    // Calculate frame rate
    m_FrameRateAccumulator += m_UnscaledDeltaTime;
    m_FramesSinceLastUpdate++;

    if (m_FrameRateAccumulator >= 1.0f) {
        m_FrameRate = static_cast<float>(m_FramesSinceLastUpdate);
        m_FrameRateAccumulator = 0.0f;
        m_FramesSinceLastUpdate = 0;
    }
}

EngineCore::EngineCore() {
    LOG_INFO("EngineCore created");
}

EngineCore::~EngineCore() {
    Shutdown();
}

bool EngineCore::Initialize() {
    LOG_INFO("=== Initializing Luminary Engine ===");

    try {
        // Initialize core systems
        m_GameTime = std::make_shared<GameTime>();
        m_Renderer = std::make_shared<Renderer>();
        m_InputSystem = std::make_shared<InputSystem>();
        m_AudioSystem = std::make_shared<AudioSystem>();
        m_AssetManager = std::make_shared<AssetManager>();
        m_Editor = std::make_shared<Editor>();

        if (!m_Renderer->Initialize()) {
            LOG_ERROR("Failed to initialize Renderer");
            return false;
        }

        if (!m_InputSystem->Initialize()) {
            LOG_ERROR("Failed to initialize InputSystem");
            return false;
        }

        if (!m_AudioSystem->Initialize()) {
            LOG_ERROR("Failed to initialize AudioSystem");
            return false;
        }

        if (!m_AssetManager->Initialize()) {
            LOG_ERROR("Failed to initialize AssetManager");
            return false;
        }

        if (!m_Editor->Initialize()) {
            LOG_ERROR("Failed to initialize Editor");
            return false;
        }

        m_IsRunning = true;
        LOG_INFO("=== Engine initialized successfully ===");
        return true;
    }
    catch (const std::exception& e) {
        LOG_ERROR("Engine initialization failed: {}", e.what());
        return false;
    }
}

void EngineCore::Shutdown() {
    LOG_INFO("=== Shutting down Luminary Engine ===");

    if (m_Editor) m_Editor->Shutdown();
    if (m_AssetManager) m_AssetManager->Shutdown();
    if (m_AudioSystem) m_AudioSystem->Shutdown();
    if (m_InputSystem) m_InputSystem->Shutdown();
    if (m_Renderer) m_Renderer->Shutdown();

    m_IsRunning = false;
    LOG_INFO("=== Engine shut down ===");
}

void EngineCore::Update() {
    if (!m_IsRunning) return;

    m_GameTime->Update();
    m_InputSystem->ProcessInput();
    m_AudioSystem->Update(m_GameTime->GetDeltaTime());
    m_Editor->Update(m_GameTime->GetDeltaTime());
}

void EngineCore::Render() {
    if (!m_IsRunning) return;

    m_Renderer->BeginFrame();
    m_Editor->OnGUI();
    m_Renderer->EndFrame();
}

} // namespace Luminary
