#include "LGE/ui/MainMenuBar.h"
#include "imgui.h"

namespace LGE {

MainMenuBar::MainMenuBar() {
}

MainMenuBar::~MainMenuBar() {
}

void MainMenuBar::OnUIRender() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New Scene")) {
                // New scene
            }
            if (ImGui::MenuItem("Open Scene")) {
                // Open scene
            }
            if (ImGui::MenuItem("Save Scene")) {
                // Save scene
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
            if (ImGui::MenuItem("Build")) {
                // Build
            }
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
        ImGui::Text("LGE Game Engine");
        
        ImGui::EndMainMenuBar();
    }
}

} // namespace LGE


