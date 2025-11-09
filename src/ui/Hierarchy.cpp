#include "LGE/ui/Hierarchy.h"
#include "LGE/core/GameObject.h"
#include "imgui.h"

namespace LGE {

Hierarchy::Hierarchy()
    : m_SelectedObject(nullptr)
{
}

Hierarchy::~Hierarchy() {
}

void Hierarchy::OnUIRender() {
    // Modern window styling
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12.0f, 8.0f));
    ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_None);
    ImGui::PopStyleVar();
    
    // Modern search bar
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8.0f, 6.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
    static char searchBuffer[256] = "";
    ImGui::PushItemWidth(-1);
    ImGui::InputTextWithHint("##Search", "Search...", searchBuffer, sizeof(searchBuffer));
    ImGui::PopItemWidth();
    ImGui::PopStyleVar(2);
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    
    // Scene hierarchy tree with modern styling
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2.0f, 4.0f));
    if (ImGui::TreeNodeEx("Scene", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth)) {
        // Show all GameObjects
        for (auto& obj : m_GameObjects) {
            if (obj) {
                bool isSelected = (m_SelectedObject == obj.get());
                
                // Apply selection styling
                if (isSelected) {
                    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.40f, 0.70f, 1.00f, 0.40f));
                    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.40f, 0.70f, 1.00f, 0.50f));
                    ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.40f, 0.70f, 1.00f, 0.60f));
                }
                
                if (ImGui::Selectable(obj->GetName().c_str(), isSelected)) {
                    m_SelectedObject = obj.get();
                    obj->SetSelected(true);
                    // Deselect others
                    for (auto& other : m_GameObjects) {
                        if (other.get() != obj.get()) {
                            other->SetSelected(false);
                        }
                    }
                }
                
                if (isSelected) {
                    ImGui::PopStyleColor(3);
                }
            }
        }
        
        ImGui::TreePop();
    }
    ImGui::PopStyleVar();
    
    ImGui::End();
}

} // namespace LGE

