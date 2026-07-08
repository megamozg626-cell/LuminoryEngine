cmake_minimum_required(VERSION 3.16)
project(LuminoryEngine VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Engine source files
set(ENGINE_SOURCES
    # Core
    Source/Core/Logging/Logger.cpp
    Source/Core/Logging/Logger.hpp
    
    # Math
    Source/Math/Vector2.hpp
    Source/Math/Vector3.hpp
    Source/Math/Vector4.hpp
    Source/Math/Matrix4.hpp
    Source/Math/Quaternion.hpp
    Source/Math/Math.hpp
    
    # Renderer
    Source/Renderer/RenderCore.hpp
    Source/Renderer/RenderCore.cpp
    Source/Renderer/Camera.hpp
    Source/Renderer/Camera.cpp
    Source/Renderer/Mesh.hpp
    Source/Renderer/Mesh.cpp
    Source/Renderer/Material.hpp
    Source/Renderer/Material.cpp
    
    # Scene
    Source/Scene/Transform.hpp
    Source/Scene/Transform.cpp
    Source/Scene/Actor.hpp
    Source/Scene/Actor.cpp
    Source/Scene/Component.hpp
    Source/Scene/Component.cpp
    Source/Scene/Scene.hpp
    Source/Scene/Scene.cpp
    
    # Physics
    Source/Physics/Physics.hpp
    Source/Physics/Physics.cpp
    Source/Physics/CharacterController.hpp
    Source/Physics/CharacterController.cpp
    
    # Input
    Source/Input/InputSystem.hpp
    Source/Input/InputSystem.cpp
    
    # Animation
    Source/Animation/Skeleton.hpp
    Source/Animation/Skeleton.cpp
    Source/Animation/AnimationBlueprint.hpp
    Source/Animation/AnimationBlueprint.cpp
    
    # Audio
    Source/Audio/AudioSystem.hpp
    Source/Audio/AudioSystem.cpp
    
    # Asset System
    Source/AssetSystem/AssetManager.hpp
    Source/AssetSystem/AssetManager.cpp
    
    # Blueprint
    Source/Blueprint/BlueprintNode.hpp
    Source/Blueprint/BlueprintNode.cpp
    
    # Material Editor
    Source/MaterialEditor/MaterialNode.hpp
    Source/MaterialEditor/MaterialGraph.cpp
    
    # Character
    Source/Character/Character.hpp
    Source/Character/Character.cpp
    
    # AI
    Source/AI/AI.hpp
    Source/AI/AI.cpp
    
    # Particles
    Source/Particles/Particles.hpp
    Source/Particles/Particles.cpp
    
    # Editor
    Source/Editor/Editor.hpp
    Source/Editor/Editor.cpp
    
    # UI
    Source/UI/UISystem.hpp
    Source/UI/UISystem.cpp
    
    # Engine
    Source/Engine/EngineCore.hpp
    Source/Engine/EngineCore.cpp
)

# Create static library
add_library(LuminoryEngine STATIC ${ENGINE_SOURCES})

# Include directories
target_include_directories(LuminoryEngine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Source
)

# Link DirectX 12 libraries (Windows)
if(WIN32)
    target_link_libraries(LuminoryEngine PUBLIC
        d3d12.lib
        dxgi.lib
        d3dcompiler.lib
        winmm.lib
    )
endif()

# Create executable
add_executable(LuminoryEngineApp
    Source/Main.cpp
)

# Link engine library
target_link_libraries(LuminoryEngineApp PRIVATE LuminoryEngine)

# Include directories for app
target_include_directories(LuminoryEngineApp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Source
)

# Set as startup project (MSVC)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT LuminoryEngineApp)
