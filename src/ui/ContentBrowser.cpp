#include "LGE/ui/ContentBrowser.h"
#include "imgui.h"

namespace LGE {

ContentBrowser::ContentBrowser() {
}

ContentBrowser::~ContentBrowser() {
}

void ContentBrowser::OnUIRender() {
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
    
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.6f, 0.6f, 1.0f));
    ImGui::TextWrapped("Content area - assets will be displayed here");
    ImGui::PopStyleColor();
    
    ImGui::EndChild();
    
    ImGui::End();
}

} // namespace LGE

