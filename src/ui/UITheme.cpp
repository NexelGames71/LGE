#include "LGE/ui/UITheme.h"
#include "imgui.h"

namespace LGE {

void UITheme::ApplyModernTheme() {
    SetupColors();
    SetupStyle();
}

void UITheme::SetupColors() {
    ImVec4* colors = ImGui::GetStyle().Colors;
    
    // Highly metallic dark theme with Unreal Engine style - strong metallic blue/green tints
    
    // Window - darker with strong metallic blue/green tint
    colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);           // Dark metallic background
    colors[ImGuiCol_ChildBg] = ImVec4(0.06f, 0.08f, 0.10f, 1.00f);            // Slightly darker metallic
    colors[ImGuiCol_PopupBg] = ImVec4(0.10f, 0.12f, 0.14f, 1.00f);            // Popup with strong metallic tint
    
    // Borders - strong metallic blue/green tint for highly reflective look
    colors[ImGuiCol_Border] = ImVec4(0.28f, 0.38f, 0.45f, 1.00f);             // Strong metallic blue border
    colors[ImGuiCol_BorderShadow] = ImVec4(0.08f, 0.12f, 0.16f, 0.40f);      // Metallic shadow
    
    // Frame (inputs, buttons) - highly metallic appearance
    colors[ImGuiCol_FrameBg] = ImVec4(0.12f, 0.16f, 0.20f, 1.00f);           // Metallic input background
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.20f, 0.28f, 0.35f, 1.00f);    // Bright metallic on hover
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.28f, 0.36f, 0.42f, 1.00f);     // Active metallic state
    
    // Title bar - darker metallic
    colors[ImGuiCol_TitleBg] = ImVec4(0.06f, 0.08f, 0.10f, 1.00f);            // Inactive metallic title
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);      // Active metallic title
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.06f, 0.08f, 0.10f, 1.00f);   // Collapsed metallic title
    
    // Menu bar - metallic appearance
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
    
    // Scrollbar - strong metallic blue tint
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.06f, 0.08f, 0.10f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.32f, 0.42f, 0.50f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.50f, 0.58f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.48f, 0.58f, 0.66f, 1.00f);
    
    // Buttons - highly metallic appearance
    colors[ImGuiCol_Button] = ImVec4(0.14f, 0.18f, 0.22f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.32f, 0.40f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.32f, 0.40f, 0.48f, 1.00f);
    
    // Header (collapsing headers, selectable headers) - highly metallic
    colors[ImGuiCol_Header] = ImVec4(0.12f, 0.16f, 0.20f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.22f, 0.30f, 0.38f, 1.00f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.30f, 0.38f, 0.46f, 1.00f);
    
    // Separator - strong metallic blue tint
    colors[ImGuiCol_Separator] = ImVec4(0.24f, 0.32f, 0.40f, 1.00f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.32f, 0.40f, 0.48f, 1.00f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.48f, 0.56f, 1.00f);
    
    // Resize grip - highly metallic
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.36f, 0.44f, 0.60f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.38f, 0.46f, 0.54f, 0.80f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.48f, 0.56f, 0.64f, 1.00f);
    
    // Tabs - highly metallic appearance
    colors[ImGuiCol_Tab] = ImVec4(0.10f, 0.14f, 0.18f, 1.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.20f, 0.28f, 0.36f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.16f, 0.22f, 0.28f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.08f, 0.11f, 0.14f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.10f, 0.14f, 0.18f, 1.00f);
    
    // Table - highly metallic
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.26f, 0.34f, 0.42f, 1.00f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.18f, 0.24f, 0.30f, 1.00f);
    colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.08f, 0.10f, 0.12f, 0.50f);
    
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

