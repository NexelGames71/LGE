#include "LGE/ui/UI.h"
#include "LGE/core/Window.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
    
    // Style
    ImGui::StyleColorsDark();
    
    // Configure docking
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 0.0f;
    style.ChildRounding = 0.0f;
    style.FrameRounding = 0.0f;
    style.GrabRounding = 0.0f;
    style.PopupRounding = 0.0f;
    style.ScrollbarRounding = 0.0f;
    style.TabRounding = 0.0f;

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


