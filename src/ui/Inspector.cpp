#include "LGE/ui/Inspector.h"
#include "LGE/core/GameObject.h"
#include "LGE/math/Vector.h"
#include "imgui.h"

namespace LGE {

Inspector::Inspector()
    : m_SelectedObject(nullptr)
{
}

Inspector::~Inspector() {
}

void Inspector::OnUIRender() {
    ImGui::Begin("Inspector", nullptr);
    
    if (!m_SelectedObject) {
        ImGui::Text("No object selected");
        ImGui::End();
        return;
    }
    
    // Object name
    ImGui::Text("Name:");
    char nameBuffer[256];
    strncpy_s(nameBuffer, m_SelectedObject->GetName().c_str(), sizeof(nameBuffer) - 1);
    nameBuffer[sizeof(nameBuffer) - 1] = '\0';
    if (ImGui::InputText("##Name", nameBuffer, sizeof(nameBuffer))) {
        m_SelectedObject->SetName(std::string(nameBuffer));
    }
    
    ImGui::Separator();
    
    // Transform section
    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
        // Position
        Math::Vector3 pos = m_SelectedObject->GetPosition();
        float position[3] = { pos.x, pos.y, pos.z };
        ImGui::Text("Position");
        if (ImGui::DragFloat3("##Position", position, 0.1f)) {
            m_SelectedObject->SetPosition(Math::Vector3(position[0], position[1], position[2]));
        }
        
        // Rotation
        Math::Vector3 rot = m_SelectedObject->GetRotation();
        float rotation[3] = { rot.x, rot.y, rot.z };
        ImGui::Text("Rotation");
        if (ImGui::DragFloat3("##Rotation", rotation, 1.0f)) {
            m_SelectedObject->SetRotation(Math::Vector3(rotation[0], rotation[1], rotation[2]));
        }
        
        // Scale
        Math::Vector3 scl = m_SelectedObject->GetScale();
        float scale[3] = { scl.x, scl.y, scl.z };
        ImGui::Text("Scale");
        if (ImGui::DragFloat3("##Scale", scale, 0.1f)) {
            m_SelectedObject->SetScale(Math::Vector3(scale[0], scale[1], scale[2]));
        }
    }
    
    // Components section
    if (ImGui::CollapsingHeader("Components")) {
        ImGui::Text("Mesh Renderer");
        ImGui::Text("Material: DefaultLitMaterial");
    }
    
    ImGui::End();
}

} // namespace LGE

