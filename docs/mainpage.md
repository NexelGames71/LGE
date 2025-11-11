# Luma Engine API Reference

Welcome to the **Luma Engine** API documentation. This comprehensive reference covers all public APIs available to game developers building games with Luma Engine.

---

## 🎮 About Luma Engine

**Luma Engine** is a next-generation, cross-platform game engine developed by **Nexel Games**. Built with modern C++20, Luma Engine provides a high-performance foundation for creating games across Windows, Linux, and macOS platforms.

### Engine Version
- **Current Version**: 1.0.0-alpha
- **API Stability**: Alpha (subject to change)

---

## 🚀 Quick Start

### Creating Your First Application

```cpp
#include "LGE/core/Application.h"

class MyGame : public LGE::Application {
public:
    MyGame() : LGE::Application("My Game") {}
    
    bool Initialize() override {
        if (!LGE::Application::Initialize()) {
            return false;
        }
        // Your initialization code here
        return true;
    }
    
    void Update(float deltaTime) override {
        // Your game loop code here
    }
};

int main() {
    MyGame app;
    if (app.Initialize()) {
        app.Run();
    }
    return 0;
}
```

### Essential Headers

```cpp
#include "LGE/core/Application.h"    // Application framework
#include "LGE/core/Window.h"          // Window management
#include "LGE/core/Input.h"           // Input handling
#include "LGE/rendering/Renderer.h"   // Rendering system
#include "LGE/rendering/Camera.h"     // Camera system
#include "LGE/core/GameObject.h"      // Game objects
```

---

## 📚 Core Modules

### Core Systems

The **Core** module provides the foundation for all Luma Engine applications.

#### Application Framework
- **Application** - Main application class that manages the game loop
- **Window** - Window creation and management
- **Input** - Keyboard, mouse, and gamepad input handling
- **Log** - Logging system for debugging and information

#### Game Objects
- **GameObject** - Base class for all game entities
- **Component** - Component-based architecture
- **MaterialComponent** - Material assignment for objects

#### File System
- **FileSystem** - Cross-platform file operations
- **Path** - Path manipulation utilities

**See**: @ref LGE::Core "Core Module Documentation"

---

### Rendering System

The **Rendering** module provides a unified rendering interface supporting multiple graphics APIs.

#### Rendering Backends
- **OpenGL** - Fully implemented (OpenGL 3.3+)
- **Vulkan** - Planned
- **DirectX 12** - Planned

#### Core Rendering Classes
- **Renderer** - Main rendering interface
- **Shader** - Shader compilation and management
- **Material** - Material system with parameters
- **MaterialInstance** - Runtime material modifications
- **Texture** - Texture loading and management
- **Camera** - Camera system with projection
- **CameraController** - Camera movement and controls
- **Skybox** - Skybox rendering (HDR/EXR support)
- **DirectionalLight** - Directional lighting
- **Framebuffer** - Render-to-texture support

#### Rendering Primitives
- **VertexBuffer** - Vertex data management
- **VertexArray** - Vertex array objects
- **IndexBuffer** - Index data management

**See**: @ref LGE::Rendering "Rendering Module Documentation"

---

### Asset System

The **Assets** module handles asset importing, management, and metadata.

#### Asset Management
- **AssetManager** - Central asset management system
- **AssetMeta** - Asset metadata handling
- **AssetMetadata** - Metadata structure and operations
- **AssetFactory** - Asset creation and instantiation

#### Import/Export
- **ImporterRegistry** - Asset importer registration
- **TextureImporter** - Texture import functionality
- **ExporterRegistry** - Asset exporter registration
- **TextureExporter** - Texture export functionality

#### Asset Tracking
- **FileSystemWatcher** - File change monitoring
- **ThumbnailGenerator** - Thumbnail generation system

**See**: @ref LGE::Assets "Asset Module Documentation"

---

### File System & Project Management

The **FS** (File System) module provides advanced filesystem operations and project management.

#### Virtual File System
- **VFS** - Virtual File System with mount points
- **Mount** - Mount point management
- **Path** - Path resolution and manipulation

#### Project Management
- **Project** - Project creation, loading, and validation
- **ProjectManager** - Project operations (create, open, save)
- **ProjectConfig** - Project configuration structure
- **ProjectManifest** - Project manifest handling

#### Package System
- **PackageManager** - Package installation and management
- **Package** - Package structure and operations
- **PackageInfo** - Package metadata

#### Asset Registry
- **AssetRegistry** - Asset indexing and tracking
- **MetaDB** - Metadata database
- **Dependencies** - Asset dependency tracking

#### File Operations
- **FileIO** - File read/write operations
- **FSOps** - Filesystem operations (copy, move, delete)
- **Recycle** - Recycle bin functionality
- **FSWatch** - File system watching

#### Advanced Features
- **ThreadPool** - Background operation threading
- **EditorBus** - Editor event system
- **Thumbs** - Thumbnail management
- **Search** - File and asset search
- **Validation** - Asset validation and integrity checking
- **Versioning** - Version control integration
- **Backup** - Backup and restore functionality
- **Sync** - Synchronization utilities
- **Streaming** - Asset streaming system
- **Optimization** - Asset optimization tools
- **Analytics** - Usage analytics
- **Cache** - Caching system
- **Compression** - Compression utilities
- **Bundles** - Asset bundling
- **Migration** - Data migration tools
- **Policies** - Filesystem policies and validation

**See**: @ref LGE::FS "File System Module Documentation"

---

### User Interface

The **UI** module provides editor interface components built on ImGui.

#### Editor Panels
- **UI** - Main UI system and docking
- **SceneViewport** - 3D scene viewport with camera controls
- **Hierarchy** - Scene hierarchy panel
- **Inspector** - Property inspector panel
- **ContentBrowser** - Asset browser with thumbnails
- **Details** - Detailed property editing
- **Toolbar** - Main toolbar
- **MainMenuBar** - Main menu bar

#### Project Management UI
- **ProjectBrowser** - Project selection and creation
- **ProjectManager** - Project management UI
- **PackageManager** - Package management UI
- **SplashScreen** - Splash screen with loading

#### Theming
- **UITheme** - UI theming system

**See**: @ref LGE::UI "UI Module Documentation"

---

### Mathematics

The **Math** module provides essential mathematics utilities.

#### Vector Types
- **Vector2** - 2D vector operations
- **Vector3** - 3D vector operations
- **Vector4** - 4D vector operations

#### Matrix Types
- **Matrix4** - 4x4 matrix operations (transformations, projections)

**See**: @ref LGE::Math "Math Module Documentation"

---

### Testing Framework

The **Testing** module provides testing and benchmarking utilities.

#### Testing
- **TestFramework** - Unit testing framework
- **TestCase** - Test case structure
- **TestResult** - Test result reporting
- **TestUtils** - Testing utilities

#### Benchmarking
- **BenchmarkRunner** - Benchmark execution
- **BenchmarkResult** - Benchmark results
- **BenchmarkConfig** - Benchmark configuration

**See**: @ref LGE::Testing "Testing Module Documentation"

---

## 🏗️ Architecture Overview

### Component-Based Design

Luma Engine uses a component-based architecture where game objects are composed of components:

```cpp
// Create a game object
auto obj = std::make_unique<LGE::GameObject>("MyObject");

// Add a material component
auto materialComp = obj->AddComponent<LGE::MaterialComponent>();
materialComp->SetMaterial(myMaterial);
```

### Rendering Pipeline

The rendering system abstracts multiple graphics APIs through a unified interface:

```cpp
// Initialize renderer
LGE::Renderer::Init(window);

// Render a scene
LGE::Renderer::BeginScene(camera, lights);
// ... draw objects ...
LGE::Renderer::EndScene();
```

### Virtual File System

The VFS allows mounting different directories and accessing them through virtual paths:

```cpp
// Mount a directory
LGE::VFS::MountPath("Assets", "/path/to/assets");

// Access files through virtual path
std::string content = LGE::VFS::ReadFile("Assets/texture.png");
```

---

## 📖 API Organization

### By Namespace

- @ref LGE::Core "Core" - Core engine systems
- @ref LGE::Rendering "Rendering" - Rendering system
- @ref LGE::Assets "Assets" - Asset management
- @ref LGE::FS "File System" - Filesystem and project management
- @ref LGE::UI "User Interface" - Editor UI components
- @ref LGE::Math "Mathematics" - Math utilities
- @ref LGE::Testing "Testing" - Testing framework

### By Category

- **Classes** - All engine classes organized by module
- **Functions** - Standalone functions and utilities
- **Enumerations** - Engine enumerations and constants
- **Structures** - Data structures and configuration types

---

## 🔍 Finding What You Need

### Search

Use the search bar in the top navigation to quickly find classes, functions, or topics.

### Browse by Module

Navigate through the **Namespaces** menu to explore functionality organized by module.

### Class Hierarchy

View the **Classes** section to see the complete class hierarchy with inheritance diagrams.

### Call Graphs

Many functions include call graphs showing:
- **Call Graph**: What functions this function calls
- **Caller Graph**: What functions call this function

---

## 🎯 Common Tasks

### Initializing the Engine

```cpp
#include "LGE/core/Application.h"

class MyApp : public LGE::Application {
    bool Initialize() override {
        return LGE::Application::Initialize();
    }
};
```

### Loading Assets

```cpp
#include "LGE/assets/AssetManager.h"

auto texture = LGE::AssetManager::LoadTexture("path/to/texture.png");
```

### Creating a Camera

```cpp
#include "LGE/rendering/Camera.h"

LGE::Camera camera(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
```

### Handling Input

```cpp
#include "LGE/core/Input.h"

if (LGE::Input::IsKeyPressed(LGE::KeyCode::Space)) {
    // Handle space key
}
```

---

## 📝 Code Examples

### Complete Application Example

See the example application in `src/main.cpp` for a complete working example of:
- Application initialization
- Project loading
- UI setup
- Rendering loop
- Asset management

---

## 🔧 Platform Support

### Supported Platforms
- **Windows** (10/11)
- **Linux** (Ubuntu 20.04+, other distributions)
- **macOS** (10.15+)

### Graphics APIs
- **OpenGL** 3.3+ (Fully supported)
- **Vulkan** (Planned)
- **DirectX 12** (Planned)

---

## 📚 Additional Resources

### Filesystem Principles

For detailed information about the Luma Engine filesystem architecture, see:
- @ref fs-principles "Filesystem Principles Documentation"

### Project Structure

Every Luma Engine project follows a canonical structure:
```
<Project>/
  Assets/         # User content (mutable)
  Packages/       # Engine packages (read-only)
  Cache/          # Derived data
  Settings/       # Project settings
  Project.lproj   # Project manifest
```

---

## ⚠️ Important Notes

### API Stability

This is an **alpha version** of Luma Engine. The API is subject to change. When using the engine:

- **Do not** rely on undocumented behavior
- **Do** check the changelog when updating versions
- **Do** report issues and API inconsistencies

### Public vs Internal API

This documentation focuses on the **public API** intended for game developers. Internal implementation details are intentionally hidden to:
- Keep the documentation focused and maintainable
- Reduce API surface complexity
- Allow internal refactoring without breaking public APIs

---

## 🤝 Contributing

For information about contributing to Luma Engine, please refer to the project repository.

---

## 📄 License

Luma Engine is developed by Nexel Games. See the project repository for license information.

---

*Generated by Doxygen for Luma Engine 1.0.0-alpha*
