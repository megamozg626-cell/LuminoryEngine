#include <iostream>
#include "Engine/EngineCore.hpp"
#include "Scene/Scene.hpp"
#include "Scene/Actor.hpp"
#include "Renderer/Mesh.hpp"
#include "Renderer/Material.hpp"
#include "Input/InputSystem.hpp"
#include "Core/Logging/Logger.hpp"

using namespace Luminary;

int main(int argc, char* argv[]) {
    try {
        // Initialize logger
        Logger::Initialize("LuminoryEngine.log");
        LOG_INFO("=== Luminary Engine Starting ===");

        // Create engine core
        auto engine = std::make_shared<EngineCore>();
        if (!engine->Initialize()) {
            LOG_ERROR("Failed to initialize engine");
            return 1;
        }

        // Create scene
        auto scene = std::make_shared<Scene>("MainScene");
        LOG_INFO("Created scene: {}", scene->GetName());

        // Spawn some actors
        auto cubeActor = scene->SpawnActor("Cube");
        auto transform = cubeActor->GetTransform();
        transform->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
        LOG_INFO("Spawned actor: {}", cubeActor->GetName());

        auto sphereActor = scene->SpawnActor("Sphere");
        auto sphereTransform = sphereActor->GetTransform();
        sphereTransform->SetPosition(Vector3(3.0f, 0.0f, 0.0f));
        LOG_INFO("Spawned actor: {}", sphereActor->GetName());

        auto groundActor = scene->SpawnActor("Ground");
        auto groundTransform = groundActor->GetTransform();
        groundTransform->SetPosition(Vector3(0.0f, -5.0f, 0.0f));
        groundTransform->SetScale(Vector3(10.0f, 0.5f, 10.0f));
        LOG_INFO("Spawned actor: {}", groundActor->GetName());

        // Create camera
        auto cameraActor = scene->SpawnActor("Camera");
        auto cameraTransform = cameraActor->GetTransform();
        cameraTransform->SetPosition(Vector3(5.0f, 5.0f, 5.0f));
        LOG_INFO("Created camera at position: ({}, {}, {})", 5.0f, 5.0f, 5.0f);

        // Game loop
        LOG_INFO("=== Starting game loop ===");
        uint32_t frameCount = 0;
        double elapsedTime = 0.0;

        while (engine->IsRunning() && frameCount < 600) { // Run for ~10 seconds at 60 FPS
            engine->Update();
            scene->Update(engine->GetGameTime()->GetDeltaTime());

            // Simple input handling
            auto inputSystem = engine->GetInputSystem();
            if (inputSystem->IsKeyPressed(InputSystem::KeyCode::Escape)) {
                LOG_INFO("Escape key pressed, exiting");
                engine->Exit();
            }

            // Update actor positions
            if (auto cubeTransform = cubeActor->GetTransform()) {
                Vector3 pos = cubeTransform->GetPosition();
                pos.y = std::sin(static_cast<float>(elapsedTime)) * 2.0f;
                cubeTransform->SetPosition(pos);
            }

            // Render
            engine->GetRenderer()->BeginFrame();
            scene->Render();
            engine->GetRenderer()->EndFrame();

            frameCount++;
            elapsedTime += engine->GetGameTime()->GetDeltaTime();

            if (frameCount % 60 == 0) {
                LOG_INFO("Frame: {}, FPS: {:.1f}, Time: {:.2f}s",
                    frameCount,
                    engine->GetGameTime()->GetFrameRate(),
                    elapsedTime);
            }
        }

        LOG_INFO("=== Game loop ended ===");
        LOG_INFO("Total frames: {}, Total time: {:.2f}s", frameCount, elapsedTime);

        // Shutdown
        engine->Shutdown();
        Logger::Shutdown();

        LOG_INFO("=== Luminary Engine Shutdown ===");
        return 0;
    }
    catch (const std::exception& e) {
        LOG_ERROR("Unhandled exception: {}", e.what());
        return 1;
    }
}
