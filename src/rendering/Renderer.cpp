#include "LGE/rendering/Renderer.h"
#include "LGE/rendering/opengl/OpenGLRenderer.h"
#include "LGE/core/Log.h"

namespace LGE {

Renderer::API Renderer::s_API = Renderer::API::OpenGL;

std::unique_ptr<Renderer> Renderer::Create(API api, Window* window) {
    switch (api) {
        case API::OpenGL:
            Log::Info("Creating OpenGL renderer...");
            return std::make_unique<OpenGLRenderer>(window);
        
        case API::DirectX12:
            Log::Warn("DirectX12 renderer not yet implemented!");
            return nullptr;
        
        case API::Vulkan:
            Log::Warn("Vulkan renderer not yet implemented!");
            return nullptr;
        
        case API::None:
        default:
            Log::Error("No renderer API specified!");
            return nullptr;
    }
}

} // namespace LGE

