cmake_minimum_required(VERSION 3.20)
project(LuminoryEngine CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX /EHsc /fp:fast /arch:AVX2")

# Find required packages
find_package(DirectXMath REQUIRED)
find_package(d3d12 REQUIRED)
find_package(dxgi REQUIRED)

# Options
option(BUILD_EDITOR "Build with Editor" ON)
option(BUILD_TESTS "Build tests" ON)
option(ENABLE_PROFILING "Enable profiling" ON)

# Main engine library
add_library(LuminoryCore STATIC)

# Core module
target_sources(LuminoryCore PRIVATE
    Source/Core/Engine/Engine.cpp
    Source/Core/Engine/Engine.hpp
    Source/Core/Application/Application.cpp
    Source/Core/Application/Application.hpp
    Source/Core/Memory/MemoryPool.cpp
    Source/Core/Memory/MemoryPool.hpp
    Source/Core/Logging/Logger.cpp
    Source/Core/Logging/Logger.hpp
    Source/Core/Threading/TaskSystem.cpp
    Source/Core/Threading/TaskSystem.hpp
    Source/Core/Time/Clock.cpp
    Source/Core/Time/Clock.hpp
)

# Math module
target_sources(LuminoryCore PRIVATE
    Source/Math/Vector2.hpp
    Source/Math/Vector3.hpp
    Source/Math/Vector4.hpp
    Source/Math/Matrix4.hpp
    Source/Math/Quaternion.hpp
    Source/Math/Math.hpp
)

# Renderer module
target_sources(LuminoryCore PRIVATE
    Source/Renderer/RenderCore.hpp
    Source/Renderer/RenderCore.cpp
    Source/Renderer/DirectXRenderer.hpp
    Source/Renderer/DirectXRenderer.cpp
    Source/Renderer/Camera.hpp
    Source/Renderer/Camera.cpp
    Source/Renderer/Shader.hpp
    Source/Renderer/Shader.cpp
    Source/Renderer/Material.hpp
    Source/Renderer/Material.cpp
    Source/Renderer/Mesh.hpp
    Source/Renderer/Mesh.cpp
    Source/Renderer/Light.hpp
    Source/Renderer/Light.cpp
    Source/Renderer/PostProcess.hpp
    Source/Renderer/PostProcess.cpp
)

# Scene module
target_sources(LuminoryCore PRIVATE
    Source/Scene/Actor.hpp
    Source/Scene/Actor.cpp
    Source/Scene/Component.hpp
    Source/Scene/Component.cpp
    Source/Scene/Scene.hpp
    Source/Scene/Scene.cpp
    Source/Scene/Transform.hpp
    Source/Scene/Transform.cpp
)

# Physics module
target_sources(LuminoryCore PRIVATE
    Source/Physics/Physics.hpp
    Source/Physics/Physics.cpp
    Source/Physics/RigidBody.hpp
    Source/Physics/RigidBody.cpp
    Source/Physics/Collision.hpp
    Source/Physics/Collision.cpp
    Source/Physics/CharacterController.hpp
    Source/Physics/CharacterController.cpp
)

# Input module
target_sources(LuminoryCore PRIVATE
    Source/Input/InputSystem.hpp
    Source/Input/InputSystem.cpp
    Source/Input/InputMapping.hpp
    Source/Input/InputMapping.cpp
)

# Animation module
target_sources(LuminoryCore PRIVATE
    Source/Animation/Skeleton.hpp
    Source/Animation/Skeleton.cpp
    Source/Animation/Animation.hpp
    Source/Animation/Animation.cpp
    Source/Animation/AnimationBlueprint.hpp
    Source/Animation/AnimationBlueprint.cpp
    Source/Animation/AnimationStateMachine.hpp
    Source/Animation/AnimationStateMachine.cpp
)

# Character module
target_sources(LuminoryCore PRIVATE
    Source/Character/Character.hpp
    Source/Character/Character.cpp
    Source/Character/PlayerCharacter.hpp
    Source/Character/PlayerCharacter.cpp
)

# Audio module
target_sources(LuminoryCore PRIVATE
    Source/Audio/AudioSystem.hpp
    Source/Audio/AudioSystem.cpp
    Source/Audio/AudioSource.hpp
    Source/Audio/AudioSource.cpp
)

# Asset system
target_sources(LuminoryCore PRIVATE
    Source/AssetSystem/AssetManager.hpp
    Source/AssetSystem/AssetManager.cpp
    Source/AssetSystem/ModelLoader.hpp
    Source/AssetSystem/ModelLoader.cpp
)

# Blueprint system
target_sources(LuminoryCore PRIVATE
    Source/Blueprint/BlueprintVM.hpp
    Source/Blueprint/BlueprintVM.cpp
    Source/Blueprint/BlueprintNode.hpp
    Source/Blueprint/BlueprintNode.cpp
    Source/Blueprint/BlueprintCompiler.hpp
    Source/Blueprint/BlueprintCompiler.cpp
)

# Material Editor
target_sources(LuminoryCore PRIVATE
    Source/MaterialEditor/MaterialNode.hpp
    Source/MaterialEditor/MaterialNode.cpp
    Source/MaterialEditor/MaterialGraph.hpp
    Source/MaterialEditor/MaterialGraph.cpp
)

# AI module
target_sources(LuminoryCore PRIVATE
    Source/AI/NavMesh.hpp
    Source/AI/NavMesh.cpp
    Source/AI/PathFinding.hpp
    Source/AI/PathFinding.cpp
    Source/AI/BehaviorTree.hpp
    Source/AI/BehaviorTree.cpp
)

# Particle system
target_sources(LuminoryCore PRIVATE
    Source/Particles/ParticleSystem.hpp
    Source/Particles/ParticleSystem.cpp
    Source/Particles/ParticleEmitter.hpp
    Source/Particles/ParticleEmitter.cpp
)

# Terrain system
target_sources(LuminoryCore PRIVATE
    Source/Terrain/Terrain.hpp
    Source/Terrain/Terrain.cpp
    Source/Terrain/TerrainLayering.hpp
    Source/Terrain/TerrainLayering.cpp
)

# Networking
target_sources(LuminoryCore PRIVATE
    Source/Networking/NetworkManager.hpp
    Source/Networking/NetworkManager.cpp
    Source/Networking/Replication.hpp
    Source/Networking/Replication.cpp
)

# UI System
target_sources(LuminoryCore PRIVATE
    Source/UI/UISystem.hpp
    Source/UI/UISystem.cpp
    Source/UI/UIWidget.hpp
    Source/UI/UIWidget.cpp
)

# Utilities
target_sources(LuminoryCore PRIVATE
    Source/Utilities/String.hpp
    Source/Utilities/String.cpp
    Source/Utilities/FileSystem.hpp
    Source/Utilities/FileSystem.cpp
    Source/Utilities/JSON.hpp
    Source/Utilities/JSON.cpp
)

target_include_directories(LuminoryCore PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/Source
    ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/nlohmann_json/include
)

target_link_libraries(LuminoryCore PUBLIC
    d3d12
    dxgi
    dxguid
)

# ImGui library
add_library(ImGui STATIC
    ThirdParty/imgui/imgui.cpp
    ThirdParty/imgui/imgui_demo.cpp
    ThirdParty/imgui/imgui_draw.cpp
    ThirdParty/imgui/imgui_widgets.cpp
    ThirdParty/imgui/imgui_tables.cpp
    ThirdParty/imgui/backends/imgui_impl_win32.cpp
    ThirdParty/imgui/backends/imgui_impl_dx12.cpp
)

target_include_directories(ImGui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/imgui/backends
)

# Editor executable
if(BUILD_EDITOR)
    add_executable(LuminoryEditor WIN32
        Source/Editor/Editor.cpp
        Source/Editor/Editor.hpp
        Source/Editor/EditorWindow.cpp
        Source/Editor/EditorWindow.hpp
        Source/Editor/Viewport.cpp
        Source/Editor/Viewport.hpp
        Source/Editor/SceneHierarchy.cpp
        Source/Editor/SceneHierarchy.hpp
        Source/Editor/Properties.cpp
        Source/Editor/Properties.hpp
        Source/Editor/ContentBrowser.cpp
        Source/Editor/ContentBrowser.hpp
        Source/Editor/MaterialEditor.cpp
        Source/Editor/MaterialEditor.hpp
        Source/Editor/BlueprintEditor.cpp
        Source/Editor/BlueprintEditor.hpp
        Source/Editor/AnimationEditor.cpp
        Source/Editor/AnimationEditor.hpp
        Source/Editor/MenuBar.cpp
        Source/Editor/MenuBar.hpp
        Source/Editor/Toolbar.cpp
        Source/Editor/Toolbar.hpp
        Source/Editor/OutputLog.cpp
        Source/Editor/OutputLog.hpp
        Source/Editor/AssetBrowser.cpp
        Source/Editor/AssetBrowser.hpp
    )

    target_link_libraries(LuminoryEditor PRIVATE
        LuminoryCore
        ImGui
    )

    target_include_directories(LuminoryEditor PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Source
    )
endif()

# Game executable
add_executable(LuminoryGame WIN32
    Source/Game/Main.cpp
)

target_link_libraries(LuminoryGame PRIVATE
    LuminoryCore
)

# Tests
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(Tests)
endif()

# Set startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT LuminoryEditor)
