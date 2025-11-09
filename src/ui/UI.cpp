#include "LGE/ui/UI.h"
#include "LGE/ui/UITheme.h"
#include "LGE/core/Window.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ImGuizmo.h"

namespace LGE {

static bool s_Initialized = false;

void UI::Initialize(Window* window) {
    if (s_Initialized) {
        return;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;  // Enable docking
    
    // Load modern font
    // Try to load a modern font from file first, then fall back to system fonts
    ImFont* font = nullptr;
    
    // Try loading from common font file paths
    std::vector<std::string> fontPaths = {
        "assets/fonts/Roboto-Regular.ttf",
        "assets/fonts/Inter-Regular.ttf",
        "assets/fonts/SegoeUI.ttf",
        "C:/Windows/Fonts/segoeui.ttf",  // Windows system font
        "C:/Windows/Fonts/calibri.ttf",   // Windows system font
    };
    
    for (const auto& path : fontPaths) {
        font = io.Fonts->AddFontFromFileTTF(path.c_str(), 16.0f);
        if (font) {
            io.FontDefault = font;
            break;
        }
    }
    
    // If no font loaded, use default but with better size
    if (!font) {
        ImFontConfig fontConfig;
        fontConfig.SizePixels = 16.0f;
        font = io.Fonts->AddFontDefault(&fontConfig);
        io.FontDefault = font;
    }
    
    // Apply modern theme
    UITheme::ApplyModernTheme();

    GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(window->GetNativeWindow());
    ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    s_Initialized = true;
}

void UI::Shutdown() {
    if (!s_Initialized) {
        return;
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    s_Initialized = false;
}

void UI::BeginFrame() {
    if (!s_Initialized) {
        return;
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();
}

void UI::EndFrame() {
    if (!s_Initialized) {
        return;
    }

    ImGui::Render();
}

void UI::Render() {
    if (!s_Initialized) {
        return;
    }

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool UI::WantCaptureMouse() {
    if (!s_Initialized) {
        return false;
    }
    ImGuiIO& io = ImGui::GetIO();
    return io.WantCaptureMouse;
}

bool UI::WantCaptureKeyboard() {
    if (!s_Initialized) {
        return false;
    }
    ImGuiIO& io = ImGui::GetIO();
    return io.WantCaptureKeyboard;
}

} // namespace LGE


