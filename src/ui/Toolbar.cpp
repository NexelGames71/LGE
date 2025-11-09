#include "LGE/ui/Toolbar.h"
#include "imgui.h"

namespace LGE {

Toolbar::Toolbar() {
}

Toolbar::~Toolbar() {
}

void Toolbar::OnUIRender() {
    // Toolbar style
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4.0f, 4.0f));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
    
    // Make toolbar non-dockable and position it below menu bar
    float menuBarHeight = ImGui::GetFrameHeight();
    float toolbarHeight = 40.0f;
    
    ImGui::SetNextWindowPos(ImVec2(0, menuBarHeight));
    ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, toolbarHeight));
    
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | 
                             ImGuiWindowFlags_NoResize | 
                             ImGuiWindowFlags_NoMove |
                             ImGuiWindowFlags_NoScrollbar |
                             ImGuiWindowFlags_NoCollapse |
                             ImGuiWindowFlags_NoDocking |
                             ImGuiWindowFlags_NoBringToFrontOnFocus;
    
    ImGui::Begin("Toolbar", nullptr, flags);
    
    // Selection mode dropdown (matches Unreal Engine)
    const char* selectionModes[] = { "World", "Local" };
    static int currentSelectionMode = 0;
    ImGui::SetNextItemWidth(100.0f);
    ImGui::Combo("Selection Mode", &currentSelectionMode, selectionModes, IM_ARRAYSIZE(selectionModes));
    
    ImGui::SameLine();
    ImGui::Separator();
    ImGui::SameLine();
    
    // Platforms icon/button (simplified as button for now)
    if (ImGui::Button("Platforms")) {
        // Platform options
    }
    
    ImGui::SameLine();
    ImGui::Separator();
    ImGui::SameLine();
    
    // Viewport options (matching Unreal Engine)
    ImGui::Text("Perspective");
    ImGui::SameLine();
    ImGui::Text("Lit");
    ImGui::SameLine();
    if (ImGui::Button("Show")) {
        // Show options menu
    }
    
    ImGui::SameLine();
    ImGui::Separator();
    ImGui::SameLine();
    
    // Settings gear icon (matching Unreal Engine)
    if (ImGui::Button("⚙")) {
        // Settings menu
    }
    
    ImGui::End();
    
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
}

} // namespace LGE

