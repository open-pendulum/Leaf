# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build System

This project uses CMake as its build system. To build the project:

```bash
# Create build directory
mkdir build && cd build

# Configure with CMake (Debug build shown)
cmake -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=Debug ..

# Build the project
cmake --build . --config Debug

# Run the sandbox application
./bin/Sandbox.exe
```

The main components are:
- **Leaf**: Static library containing the engine core
- **Sandbox**: Example application demonstrating engine usage
- **Third-party dependencies**: GLFW, OpenGL, ImGui, GLM, spdlog, glad (all included)

## Project Structure

### Core Architecture

Leaf is a 2D/3D game engine built with modern C++ (C++20) and OpenGL. The architecture follows a layered design:

1. **Application Layer** (`core/Application.h`): Main application class that manages the window, event loop, and layer stack
2. **Layer System** (`core/Layer.h`): Manages different game/application layers and overlays (e.g., ImGui as overlay)
3. **Event System** (`core/events/`): Event-driven architecture with polymorphic event handling
4. **Renderer** (`core/renderer/`): Abstraction layer for rendering with OpenGL backend
5. **Platform Abstraction** (`platform/`): Windows-specific implementations for window, input, and OpenGL context

### Key Components

- **Window Management**: Platform-agnostic window interface with Windows implementation
- **Input System**: Keyboard and mouse input with platform-specific backends
- **Rendering**: OpenGL-based renderer with buffer, vertex array, shader abstractions
- **ImGui Integration**: Dear ImGui integration with optional docking support
- **Math Library**: Uses GLM for vector/matrix math

### Entry Point

Applications must implement `Leaf::CreateApplication()` in their main source file. The sandbox example shows a typical implementation:

```cpp
class Sandbox : public Leaf::Application {
public:
    Sandbox() {
        PushLayer(new ExampleLayer());  // Add game logic layers
    }
};

Leaf::Application *Leaf::CreateApplication() {
    return new Sandbox();
}
```

## Development Notes

- Precompiled headers (PCH) are enabled via `LeafPCH.h`
- The engine uses OpenGL 3.3+ with GLAD for loading OpenGL functions
- ImGui is configured with optional docking support (controlled by CMake option)
- Build type affects compiler flags (Debug: -g -O0, Release: optimized)
- CMake build outputs binaries to `bin/` directory
- The sandbox demonstrates orthographic camera control, shader usage, and rendering batches