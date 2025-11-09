#include "LGE/ui/ContentBrowser.h"
#include "LGE/rendering/Texture.h"
#include "LGE/core/FileSystem.h"
#include "imgui.h"
#include <vector>
#include <string>

namespace LGE {

ContentBrowser::ContentBrowser()
    : m_FolderIconLoaded(false)
{
}

ContentBrowser::~ContentBrowser() {
}

void ContentBrowser::LoadFolderIcon() {
    if (m_FolderIconLoaded) return;
    
    std::vector<std::string> folderPaths = {
        "unreal-engine-editor-icons/imgs/Icons/Folders/Folder_Base_512x.png",
        "assets/icons/Folder_Base_512x.png",
        "assets/icons/folder.png"
    };
    
    m_FolderIcon = std::make_shared<Texture>();
    bool folderLoaded = false;
    for (const auto& path : folderPaths) {
        if (m_FolderIcon->LoadImageFile(path)) {
            folderLoaded = true;
            break;
        }
    }
    if (!folderLoaded) {
        m_FolderIcon.reset();
    }
    
    m_FolderIconLoaded = true;
}

void ContentBrowser::OnUIRender() {
    // Load folder icon on first render
    if (!m_FolderIconLoaded) {
        LoadFolderIcon();
    }
    
    // Modern window styling
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12.0f, 8.0f));
    ImGui::Begin("Content Browser", nullptr, ImGuiWindowFlags_None);
    ImGui::PopStyleVar();
    
    // Modern toolbar
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4.0f, 4.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8.0f, 4.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
    
    if (ImGui::Button("Add")) {
        // Add asset
    }
    ImGui::SameLine();
    if (ImGui::Button("Import")) {
        // Import asset
    }
    ImGui::SameLine();
    if (ImGui::Button("Save All")) {
        // Save all assets
    }
    
    ImGui::PopStyleVar(3);
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    
    // Split view: folders on left, content on right
    float panelWidth = ImGui::GetContentRegionAvail().x;
    float folderPanelWidth = panelWidth * 0.25f;
    
    // Folder tree with modern styling
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 4.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
    ImGui::BeginChild("FolderTree", ImVec2(folderPanelWidth, 0), true, ImGuiWindowFlags_None);
    ImGui::PopStyleVar(2);
    
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2.0f, 4.0f));
    if (ImGui::TreeNodeEx("Assets", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth)) {
        if (ImGui::TreeNodeEx("Shaders", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_SpanAvailWidth)) {
            ImGui::TreePop();
        }
        if (ImGui::TreeNodeEx("HDR Sky", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_SpanAvailWidth)) {
            ImGui::TreePop();
        }
        if (ImGui::TreeNodeEx("EXR Sky", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_SpanAvailWidth)) {
            ImGui::TreePop();
        }
        ImGui::TreePop();
    }
    ImGui::PopStyleVar();
    ImGui::EndChild();
    
    ImGui::SameLine();
    
    // Content area with modern styling
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 4.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
    ImGui::BeginChild("ContentArea", ImVec2(0, 0), true, ImGuiWindowFlags_None);
    ImGui::PopStyleVar(2);
    
    // Display folder thumbnails in grid view
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
    
    // Example folders to display
    const char* folders[] = { "Shaders", "HDR Sky", "EXR Sky", "Textures", "Materials", "Models" };
    const int numFolders = sizeof(folders) / sizeof(folders[0]);
    
    float thumbnailSize = 64.0f;
    float padding = 8.0f;
    float availableWidth = ImGui::GetContentRegionAvail().x;
    int columns = (int)(availableWidth / (thumbnailSize + padding));
    if (columns < 1) columns = 1;
    
    ImGui::Columns(columns, nullptr, false);
    
    for (int i = 0; i < numFolders; i++) {
        ImGui::PushID(i);
        
        // Draw folder thumbnail
        if (m_FolderIcon && m_FolderIcon->GetRendererID() != 0) {
            ImVec2 cursorPos = ImGui::GetCursorPos();
            ImVec2 imageSize(thumbnailSize, thumbnailSize);
            
            // Draw folder icon with color tint
            ImGui::Image(
                reinterpret_cast<void*>(static_cast<intptr_t>(m_FolderIcon->GetRendererID())),
                imageSize,
                ImVec2(0, 0), ImVec2(1, 1), // Normal texture coordinates
                ImVec4(0.95f, 0.85f, 0.55f, 1.0f), // Tint color (light yellow/orange for folders)
                ImVec4(0.0f, 0.0f, 0.0f, 0.0f)  // Border color
            );
            
            // Make it clickable
            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip(folders[i]);
            }
            if (ImGui::IsItemClicked()) {
                // Handle folder click
            }
            
            // Folder name below icon
            ImVec2 textSize = ImGui::CalcTextSize(folders[i]);
            ImGui::SetCursorPosX(cursorPos.x + (thumbnailSize - textSize.x) * 0.5f);
            ImGui::Text(folders[i]);
        } else {
            // Fallback: just text if icon not loaded
            ImGui::Text(folders[i]);
        }
        
        ImGui::PopID();
        ImGui::NextColumn();
    }
    
    ImGui::Columns(1);
    ImGui::PopStyleVar();
    
    ImGui::EndChild();
    
    ImGui::End();
}

} // namespace LGE

