# Luminary Engine

## Overview

Luminary Engine is a modern, high-performance game engine written in C++ with a focus on DirectX 12 rendering, advanced physics, and comprehensive editor tools.

## Features

### Core Systems
- **Rendering**: DirectX 12 graphics with support for complex materials and lighting
- **Physics**: Full 3D physics engine with rigid bodies and collision detection
- **Input**: Complete input handling system for keyboard, mouse, and controllers
- **Audio**: Audio system for sound effects and background music
- **Asset Management**: Comprehensive asset loading and caching system

### Advanced Features
- **Animation System**: Skeleton-based animation with smooth blending
- **Blueprint System**: Visual scripting for game logic
- **Material Editor**: Node-based material creation and editing
- **Character System**: Ready-to-use character controllers and player mechanics
- **AI System**: Navigation mesh, pathfinding, and behavior trees
- **Particle System**: Dynamic particle effects and emitters
- **UI System**: Complete UI framework with buttons, labels, text inputs, and panels

### Editor Tools
- **Scene Editor**: Full scene editing with hierarchy visualization
- **Inspector**: Property editing for actors and components
- **Console**: Real-time logging and debugging
- **Properties Panel**: Advanced property management
- **Blueprint Graph Editor**: Visual programming interface
- **Material Graph Editor**: Node-based material creation

## Architecture

### Directory Structure
```
Source/
├── Core/              # Core engine functionality
│   └── Logging/       # Logging system
├── Math/              # Math libraries (vectors, matrices, quaternions)
├── Renderer/          # Graphics rendering system
├── Scene/             # Scene management and actor system
├── Physics/           # Physics simulation
├── Input/             # Input handling
├── Animation/         # Animation system
├── Audio/             # Audio system
├── AssetSystem/       # Asset management
├── Blueprint/         # Blueprint visual scripting
├── MaterialEditor/    # Material editing system
├── Character/         # Character controllers
├── AI/                # AI and pathfinding
├── Particles/         # Particle system
├── Editor/            # Editor tools
├── UI/                # User interface
└── Engine/            # Engine core and main loop
```

## Building

### Requirements
- CMake 3.16 or higher
- Visual Studio 2019 or higher (for Windows)
- C++17 compliant compiler
- DirectX 12 SDK
- fmt library (for formatting)

### Build Instructions

```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

## Usage

### Basic Example

```cpp
#include "Engine/EngineCore.hpp"
#include "Scene/Scene.hpp"

using namespace Luminary;

int main() {
    // Initialize engine
    auto engine = std::make_shared<EngineCore>();
    engine->Initialize();
    
    // Create scene
    auto scene = std::make_shared<Scene>("MainScene");
    
    // Spawn actors
    auto actor = scene->SpawnActor("MyActor");
    actor->GetTransform()->SetPosition(Vector3(0, 0, 0));
    
    // Game loop
    while (engine->IsRunning()) {
        engine->Update();
        scene->Update(engine->GetGameTime()->GetDeltaTime());
        engine->Render();
    }
    
    engine->Shutdown();
    return 0;
}
```

## Key Systems

### Renderer
DirectX 12-based renderer with:
- Multi-threaded command buffer recording
- Advanced material system
- Real-time lighting and shadows
- Post-processing effects

### Physics
- Rigid body dynamics
- Collision detection and response
- Raycasting and shape queries
- Constraint solving

### Animation
- Skeletal animation
- Animation blending and layering
- IK (Inverse Kinematics) support
- Animation state machines

### Blueprint System
- Node-based visual scripting
- Execution flow control
- Variable system
- Event system

## License

Luminary Engine is licensed under the MIT License.

## Contributing

Contributions are welcome! Please feel free to submit pull requests and open issues for bugs or feature requests.

## Support

For issues, questions, or suggestions, please open an issue on the GitHub repository.
