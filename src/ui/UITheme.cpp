#include "LGE/ui/UITheme.h"
#include "imgui.h"

namespace LGE {

void UITheme::ApplyModernTheme() {
    SetupColors();
    SetupStyle();
}

void UITheme::SetupColors() {
    ImVec4* colors = ImGui::GetStyle().Colors;
    
    // Modern dark theme with blue accents (Unreal/Unity style)
    
    // Window
    colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.15f, 0.17f, 1.00f);           // Dark gray background
    colors[ImGuiCol_ChildBg] = ImVec4(0.12f, 0.12f, 0.14f, 1.00f);            // Slightly darker for child windows
    colors[ImGuiCol_PopupBg] = ImVec4(0.18f, 0.18f, 0.20f, 1.00f);            // Popup background
    
    // Borders
    colors[ImGuiCol_Border] = ImVec4(0.25f, 0.25f, 0.28f, 1.00f);             // Subtle border
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);      // No shadow
    
    // Frame (inputs, buttons)
    colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.23f, 1.00f);           // Input background
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.25f, 0.28f, 1.00f);    // Hover state
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.30f, 0.30f, 0.33f, 1.00f);     // Active state
    
    // Title bar
    colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);            // Inactive title
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.12f, 0.12f, 0.14f, 1.00f);      // Active title
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);   // Collapsed title
    
    // Menu bar
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.12f, 0.12f, 0.14f, 1.00f);
    
    // Scrollbar
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.30f, 0.30f, 0.33f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.35f, 0.35f, 0.38f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.40f, 0.40f, 0.43f, 1.00f);
    
    // Buttons
    colors[ImGuiCol_Button] = ImVec4(0.25f, 0.25f, 0.28f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.30f, 0.30f, 0.33f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.35f, 0.35f, 0.38f, 1.00f);
    
    // Header (collapsing headers, selectable headers)
    colors[ImGuiCol_Header] = ImVec4(0.22f, 0.22f, 0.25f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.28f, 0.28f, 0.32f, 1.00f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.35f, 0.35f, 0.40f, 1.00f);
    
    // Separator
    colors[ImGuiCol_Separator] = ImVec4(0.25f, 0.25f, 0.28f, 1.00f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.30f, 0.30f, 0.33f, 1.00f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.35f, 0.35f, 0.38f, 1.00f);
    
    // Resize grip
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.30f, 0.30f, 0.33f, 0.50f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.40f, 0.40f, 0.43f, 0.70f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.50f, 0.50f, 0.53f, 1.00f);
    
    // Tabs
    colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.18f, 0.20f, 1.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.25f, 0.25f, 0.28f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.22f, 0.22f, 0.25f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.15f, 0.15f, 0.17f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.18f, 0.18f, 0.20f, 1.00f);
    
    // Table
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.12f, 0.12f, 0.14f, 1.00f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.25f, 0.25f, 0.28f, 1.00f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.20f, 0.20f, 0.23f, 1.00f);
    colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.12f, 0.12f, 0.14f, 0.50f);
    
    // Text
    colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.97f, 1.00f);               // Almost white text
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.52f, 1.00f);      // Disabled text
    
    // Checkbox, Radio, Slider
    colors[ImGuiCol_CheckMark] = ImVec4(0.40f, 0.70f, 1.00f, 1.00f);         // Blue accent
    colors[ImGuiCol_SliderGrab] = ImVec4(0.40f, 0.70f, 1.00f, 1.00f);        // Blue accent
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.50f, 0.75f, 1.00f, 1.00f);  // Lighter blue
    
    // Plot lines
    colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.70f, 1.00f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.50f, 0.75f, 1.00f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.70f, 1.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.50f, 0.75f, 1.00f, 1.00f);
    
    // Selection
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.40f, 0.70f, 1.00f, 0.35f);    // Blue selection with transparency
    
    // Drag and drop
    colors[ImGuiCol_DragDropTarget] = ImVec4(0.40f, 0.70f, 1.00f, 0.90f);
    
    // Navigation highlight
    colors[ImGuiCol_NavHighlight] = ImVec4(0.40f, 0.70f, 1.00f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.40f, 0.70f, 1.00f, 0.80f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.60f);
    
    // Modal overlay
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.60f);
}

void UITheme::SetupStyle() {
    ImGuiStyle& style = ImGui::GetStyle();
    
    // Window
    style.WindowPadding = ImVec2(8.0f, 8.0f);
    style.WindowRounding = 4.0f;
    style.WindowBorderSize = 1.0f;
    style.WindowMinSize = ImVec2(100.0f, 100.0f);
    style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
    style.WindowMenuButtonPosition = ImGuiDir_Left;
    
    // Child windows
    style.ChildRounding = 4.0f;
    style.ChildBorderSize = 1.0f;
    style.PopupRounding = 4.0f;
    style.PopupBorderSize = 1.0f;
    
    // Frame (inputs, buttons)
    style.FramePadding = ImVec2(6.0f, 4.0f);
    style.FrameRounding = 3.0f;
    style.FrameBorderSize = 1.0f;
    
    // Item spacing
    style.ItemSpacing = ImVec2(6.0f, 4.0f);
    style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
    
    // Indent
    style.IndentSpacing = 20.0f;
    
    // Scrollbar
    style.ScrollbarSize = 14.0f;
    style.ScrollbarRounding = 3.0f;
    
    // Grab (slider grab, resize grip)
    style.GrabMinSize = 10.0f;
    style.GrabRounding = 3.0f;
    
    // Tab
    style.TabRounding = 3.0f;
    style.TabBorderSize = 1.0f;
    
    // Button text align
    style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
    
    // Selectable text align
    style.SelectableTextAlign = ImVec2(0.0f, 0.0f);
    
    // Display safe area padding
    style.DisplaySafeAreaPadding = ImVec2(3.0f, 3.0f);
    
    // Global alpha
    style.Alpha = 1.0f;
    
    // Disable rounding for docking
    style.WindowRounding = 0.0f;
    style.ChildRounding = 0.0f;
}

} // namespace LGE

