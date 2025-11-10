#include "LGE/ui/MainMenuBar.h"
#include "imgui.h"

namespace LGE {

MainMenuBar::MainMenuBar() {
}

MainMenuBar::~MainMenuBar() {
}

void MainMenuBar::OnUIRender() {
    // Modern menu bar styling
    ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.12f, 0.12f, 0.14f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f)); // Transparent border
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New Scene")) {
                // New scene
            }
            if (ImGui::MenuItem("Open Scene")) {
                // Open scene
            }
            if (ImGui::MenuItem("Save Scene")) {
                if (m_OnSaveScene) {
                    m_OnSaveScene();
                }
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit")) {
                // Exit
            }
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo")) {
                // Undo
            }
            if (ImGui::MenuItem("Redo")) {
                // Redo
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Preferences")) {
                // Preferences
            }
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("Window")) {
            if (ImGui::MenuItem("Hierarchy")) {
                // Toggle hierarchy
            }
            if (ImGui::MenuItem("Inspector")) {
                // Toggle inspector
            }
            if (ImGui::MenuItem("Content Browser")) {
                // Toggle content browser
            }
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("Tools")) {
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("Build")) {
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("Select")) {
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("Actor")) {
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("Help")) {
            if (ImGui::MenuItem("About")) {
                // About
            }
            ImGui::EndMenu();
        }
        
        // Window title on the right
        ImGui::SameLine(ImGui::GetWindowWidth() - 200);
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.8f, 1.0f), "LGE Game Engine");
        
        ImGui::EndMainMenuBar();
    }
    ImGui::PopStyleVar();
    ImGui::PopStyleColor(2);
}

} // namespace LGE


