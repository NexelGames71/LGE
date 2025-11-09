#pragma once

#include <memory>
#include <string>

namespace LGE {

class Window;
class Renderer;

class Application {
public:
    Application(const std::string& name = "LGE Application");
    virtual ~Application();

    // Initialize the application
    virtual bool Initialize();
    
    // Run the main loop
    void Run();
    
    // Cleanup resources
    virtual void Shutdown();

    // Override these in derived classes
    virtual void OnUpdate(float deltaTime) {}
    virtual void OnRender() {}
    virtual void OnEvent() {}

protected:
    std::unique_ptr<Window> m_Window;
    std::unique_ptr<Renderer> m_Renderer;
    bool m_Running;
    std::string m_Name;
};

} // namespace LGE

