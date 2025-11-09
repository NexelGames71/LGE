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
    ImGui::Begin("Hierarchy", nullptr);
    
    // Search bar
    static char searchBuffer[256] = "";
    ImGui::InputText("##Search", searchBuffer, sizeof(searchBuffer));
    
    ImGui::Separator();
    
    // Scene hierarchy tree
    if (ImGui::TreeNode("Scene")) {
        // Show all GameObjects
        for (auto& obj : m_GameObjects) {
            if (obj) {
                bool isSelected = (m_SelectedObject == obj.get());
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
            }
        }
        
        ImGui::TreePop();
    }
    
    ImGui::End();
}

} // namespace LGE

