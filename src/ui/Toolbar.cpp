#include "LGE/ui/Toolbar.h"
#include "LGE/rendering/Texture.h"
#include "LGE/core/FileSystem.h"
#include "imgui.h"
#include <memory>
#include <vector>
#include <string>

namespace LGE {

Toolbar::Toolbar() 
    : m_IconsLoaded(false)
    , m_PreviewState(GamePreviewState::Stopped)
{
}

Toolbar::~Toolbar() {
}

void Toolbar::LoadIcons() {
    if (m_IconsLoaded) return;
    
    // Try to load play icon
    std::vector<std::string> playPaths = {
        "unreal-engine-editor-icons/imgs/Icons/generic_play_16x.png",
        "assets/icons/generic_play_16x.png",
        "assets/icons/play.png"
    };
    
    m_PlayIcon = std::make_shared<Texture>();
    bool playLoaded = false;
    for (const auto& path : playPaths) {
        if (m_PlayIcon->LoadImageFile(path)) {
            playLoaded = true;
            break;
        }
    }
    if (!playLoaded) {
        m_PlayIcon.reset();
    }
    
    // Try to load stop icon
    std::vector<std::string> stopPaths = {
        "unreal-engine-editor-icons/imgs/Icons/generic_stop_16x.png",
        "assets/icons/generic_stop_16x.png",
        "assets/icons/stop.png"
    };
    
    m_StopIcon = std::make_shared<Texture>();
    bool stopLoaded = false;
    for (const auto& path : stopPaths) {
        if (m_StopIcon->LoadImageFile(path)) {
            stopLoaded = true;
            break;
        }
    }
    if (!stopLoaded) {
        m_StopIcon.reset();
    }
    
    // Try to load pause icon
    std::vector<std::string> pausePaths = {
        "unreal-engine-editor-icons/imgs/Icons/generic_pause_16x.png",
        "assets/icons/generic_pause_16x.png",
        "assets/icons/pause.png"
    };
    
    m_PauseIcon = std::make_shared<Texture>();
    bool pauseLoaded = false;
    for (const auto& path : pausePaths) {
        if (m_PauseIcon->LoadImageFile(path)) {
            pauseLoaded = true;
            break;
        }
    }
    if (!pauseLoaded) {
        m_PauseIcon.reset();
    }
    
    m_IconsLoaded = true;
}

void Toolbar::OnUIRender() {
    // Modern toolbar styling
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 4.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.12f, 0.12f, 0.14f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.25f, 0.25f, 0.28f, 1.0f));
    
    // Make toolbar non-dockable and position it below menu bar
    float menuBarHeight = ImGui::GetFrameHeight();
    float toolbarHeight = 28.0f; // Increased to accommodate icon buttons
    
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
    
    // Load icons on first render
    if (!m_IconsLoaded) {
        LoadIcons();
    }
    
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(6.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6.0f, 4.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
    
    // Selection mode dropdown (matches Unreal Engine)
    const char* selectionModes[] = { "World", "Local" };
    static int currentSelectionMode = 0;
    ImGui::SetNextItemWidth(100.0f);
    ImGui::Combo("##SelectionMode", &currentSelectionMode, selectionModes, IM_ARRAYSIZE(selectionModes));
    
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
    
    // Center the play/stop/pause buttons
    float windowWidth = ImGui::GetWindowWidth();
    float buttonGroupWidth = 80.0f; // Approximate width of three buttons (24px each + spacing)
    float centerX = (windowWidth - buttonGroupWidth) * 0.5f;
    
    // Calculate center position
    ImGui::SetCursorPosX(centerX);
    
    // Helper function to draw icon button
    auto DrawIconButton = [](const char* label, bool selected, ImVec2 size, std::shared_ptr<Texture> icon) -> bool {
        ImGui::PushStyleColor(ImGuiCol_Button, selected ? ImVec4(0.40f, 0.70f, 1.00f, 0.60f) : ImVec4(0.20f, 0.20f, 0.23f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, selected ? ImVec4(0.45f, 0.75f, 1.00f, 0.70f) : ImVec4(0.25f, 0.25f, 0.28f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, selected ? ImVec4(0.35f, 0.65f, 0.95f, 0.80f) : ImVec4(0.30f, 0.30f, 0.33f, 1.0f));
        
        std::string uniqueLabel = std::string("##") + label;
        bool clicked = ImGui::Button(uniqueLabel.c_str(), size);
        
        // Draw icon in the center of the button
        if (ImGui::IsItemVisible() && icon && icon->GetRendererID() != 0) {
            ImVec2 pos = ImGui::GetItemRectMin();
            ImVec2 rectSize = ImGui::GetItemRectSize();
            ImVec2 center = ImVec2(pos.x + rectSize.x * 0.5f, pos.y + rectSize.y * 0.5f);
            
            float iconSize = std::min(rectSize.x, rectSize.y) * 0.6f;
            ImVec2 iconMin = ImVec2(center.x - iconSize * 0.5f, center.y - iconSize * 0.5f);
            ImVec2 iconMax = ImVec2(center.x + iconSize * 0.5f, center.y + iconSize * 0.5f);
            ImGui::GetWindowDrawList()->AddImage(
                reinterpret_cast<void*>(static_cast<intptr_t>(icon->GetRendererID())),
                iconMin, iconMax,
                ImVec2(0, 1), ImVec2(1, 0)); // Flip vertically for OpenGL
        }
        
        ImGui::PopStyleColor(3);
        return clicked;
    };
    
    // Play button
    bool isPlaying = (m_PreviewState == GamePreviewState::Playing);
    if (DrawIconButton("Play", isPlaying, ImVec2(24.0f, 20.0f), m_PlayIcon)) {
        if (m_PreviewState == GamePreviewState::Stopped || m_PreviewState == GamePreviewState::Paused) {
            m_PreviewState = GamePreviewState::Playing;
            // TODO: Start game preview
        }
    }
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Play (P)");
    }
    
    ImGui::SameLine();
    
    // Pause button
    bool isPaused = (m_PreviewState == GamePreviewState::Paused);
    if (DrawIconButton("Pause", isPaused, ImVec2(24.0f, 20.0f), m_PauseIcon)) {
        if (m_PreviewState == GamePreviewState::Playing) {
            m_PreviewState = GamePreviewState::Paused;
            // TODO: Pause game preview
        }
    }
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Pause");
    }
    
    ImGui::SameLine();
    
    // Stop button
    bool isStopped = (m_PreviewState == GamePreviewState::Stopped);
    if (DrawIconButton("Stop", isStopped, ImVec2(24.0f, 20.0f), m_StopIcon)) {
        m_PreviewState = GamePreviewState::Stopped;
        // TODO: Stop game preview
    }
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Stop (S)");
    }
    
    ImGui::SameLine();
    ImGui::Separator();
    ImGui::SameLine();
    
    // Viewport options (matching Unreal Engine)
    ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.8f, 1.0f), "Perspective");
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.8f, 1.0f), "Lit");
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
    
    ImGui::PopStyleVar(3);
    ImGui::End();
    
    ImGui::PopStyleColor(2);
    ImGui::PopStyleVar(2); // Pop WindowPadding and WindowBorderSize
}

} // namespace LGE

