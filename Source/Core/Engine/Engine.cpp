#include "Engine.hpp"
#include "../Logging/Logger.hpp"
#include "../../Renderer/RenderCore.hpp"
#include "../../Physics/Physics.hpp"
#include "../../Input/InputSystem.hpp"
#include "../../Audio/AudioSystem.hpp"
#include "../../AssetSystem/AssetManager.hpp"
#include "../../Scene/Scene.hpp"
#include "../Time/Clock.hpp"

namespace Luminary {

Engine::Engine() {
    LOG_INFO("Initializing Luminary Engine");
}

Engine::~Engine() {
    Shutdown();
}

bool Engine::Initialize(uint32_t windowWidth, uint32_t windowHeight, const std::string& windowTitle) {
    LOG_INFO("Engine::Initialize - Width: {}, Height: {}, Title: {}", windowWidth, windowHeight, windowTitle);

    try {
        InitializeSystems();
        
        m_CurrentScene = std::make_shared<Scene>("DefaultScene");
        m_IsRunning = true;

        LOG_INFO("Engine initialized successfully");
        return true;
    }
    catch (const std::exception& e) {
        LOG_ERROR("Engine initialization failed: {}", e.what());
        return false;
    }
}

void Engine::InitializeSystems() {
    LOG_INFO("Initializing core systems...");

    m_Renderer = std::make_shared<Renderer>();
    if (!m_Renderer->Initialize()) {
        throw std::runtime_error("Failed to initialize renderer");
    }

    m_Physics = std::make_shared<Physics>();
    if (!m_Physics->Initialize()) {
        throw std::runtime_error("Failed to initialize physics");
    }

    m_InputSystem = std::make_shared<InputSystem>();
    if (!m_InputSystem->Initialize()) {
        throw std::runtime_error("Failed to initialize input system");
    }

    m_AudioSystem = std::make_shared<AudioSystem>();
    if (!m_AudioSystem->Initialize()) {
        throw std::runtime_error("Failed to initialize audio system");
    }

    m_AssetManager = std::make_shared<AssetManager>();
    if (!m_AssetManager->Initialize()) {
        throw std::runtime_error("Failed to initialize asset manager");
    }

    LOG_INFO("All core systems initialized");
}

void Engine::Run() {
    Clock clock;
    clock.Start();

    while (m_IsRunning) {
        m_DeltaTime = static_cast<float>(clock.Tick());
        m_TotalTime += m_DeltaTime;
        m_FrameCount++;
        m_FrameTimeAccumulator += m_DeltaTime;

        // Calculate FPS every 100ms
        if (m_FrameTimeAccumulator >= 0.1f) {
            m_FPS = m_FrameCount / m_FrameTimeAccumulator;
            m_FrameTimeAccumulator = 0.0f;
            m_FrameCount = 0;
        }

        Tick(m_DeltaTime);
        Update(m_DeltaTime);
        Render();
    }
}

void Engine::Tick(float deltaTime) {
    m_InputSystem->ProcessInput();
    UpdateSystems(deltaTime);
}

void Engine::Update(float deltaTime) {
    if (m_CurrentScene) {
        m_CurrentScene->Update(deltaTime);
    }
    m_Physics->Update(deltaTime);
}

void Engine::Render() {
    if (m_Renderer && m_CurrentScene) {
        m_Renderer->BeginFrame();
        m_Renderer->RenderScene(m_CurrentScene);
        m_Renderer->EndFrame();
    }
}

void Engine::UpdateSystems(float deltaTime) {
    m_AudioSystem->Update(deltaTime);
}

void Engine::Shutdown() {
    LOG_INFO("Shutting down Luminary Engine");
    ShutdownSystems();
    m_IsRunning = false;
}

void Engine::ShutdownSystems() {
    if (m_AssetManager) m_AssetManager->Shutdown();
    if (m_AudioSystem) m_AudioSystem->Shutdown();
    if (m_InputSystem) m_InputSystem->Shutdown();
    if (m_Physics) m_Physics->Shutdown();
    if (m_Renderer) m_Renderer->Shutdown();
    LOG_INFO("All systems shut down");
}

} // namespace Luminary
