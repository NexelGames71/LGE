#include "LGE/ui/Inspector.h"
#include "LGE/core/GameObject.h"
#include "LGE/core/MaterialComponent.h"
#include "LGE/rendering/Texture.h"
#include "LGE/core/FileSystem.h"
#include "LGE/core/Log.h"
#include "LGE/math/Vector.h"
#include "imgui.h"
#include <string>
#include <map>
#include <functional>

namespace LGE {

Inspector::Inspector()
    : m_SelectedObject(nullptr)
{
}

Inspector::~Inspector() {
}

void Inspector::OnUIRender() {
    // Modern window styling
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12.0f, 8.0f));
    ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_None);
    ImGui::PopStyleVar();
    
    if (!m_SelectedObject) {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.6f, 0.6f, 1.0f));
        ImGui::TextWrapped("No object selected");
        ImGui::PopStyleColor();
        ImGui::End();
        return;
    }
    
    // Object name with modern styling
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8.0f, 6.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
    
    ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.8f, 1.0f), "Name");
    char nameBuffer[256];
    strncpy_s(nameBuffer, m_SelectedObject->GetName().c_str(), sizeof(nameBuffer) - 1);
    nameBuffer[sizeof(nameBuffer) - 1] = '\0';
    ImGui::PushItemWidth(-1);
    if (ImGui::InputText("##Name", nameBuffer, sizeof(nameBuffer))) {
        m_SelectedObject->SetName(std::string(nameBuffer));
    }
    ImGui::PopItemWidth();
    ImGui::PopStyleVar(2);
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    
    // Transform section with modern styling
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4.0f, 6.0f));
    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed)) {
        ImGui::Spacing();
        
        // Position
        Math::Vector3 pos = m_SelectedObject->GetPosition();
        float position[3] = { pos.x, pos.y, pos.z };
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.8f, 1.0f), "Position");
        ImGui::PushItemWidth(-1);
        if (ImGui::DragFloat3("##Position", position, 0.1f, 0.0f, 0.0f, "%.2f")) {
            m_SelectedObject->SetPosition(Math::Vector3(position[0], position[1], position[2]));
        }
        ImGui::PopItemWidth();
        ImGui::Spacing();
        
        // Rotation
        Math::Vector3 rot = m_SelectedObject->GetRotation();
        float rotation[3] = { rot.x, rot.y, rot.z };
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.8f, 1.0f), "Rotation");
        ImGui::PushItemWidth(-1);
        if (ImGui::DragFloat3("##Rotation", rotation, 1.0f, 0.0f, 0.0f, "%.1f")) {
            m_SelectedObject->SetRotation(Math::Vector3(rotation[0], rotation[1], rotation[2]));
        }
        ImGui::PopItemWidth();
        ImGui::Spacing();
        
        // Scale
        Math::Vector3 scl = m_SelectedObject->GetScale();
        float scale[3] = { scl.x, scl.y, scl.z };
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.8f, 1.0f), "Scale");
        ImGui::PushItemWidth(-1);
        if (ImGui::DragFloat3("##Scale", scale, 0.1f, 0.0f, 0.0f, "%.2f")) {
            m_SelectedObject->SetScale(Math::Vector3(scale[0], scale[1], scale[2]));
        }
        ImGui::PopItemWidth();
        ImGui::Spacing();
    }
    ImGui::PopStyleVar();
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    
    // Components section with modern styling
    if (ImGui::CollapsingHeader("Components", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed)) {
        ImGui::Spacing();
        // Material Component
        auto materialComp = m_SelectedObject->GetComponent<MaterialComponent>();
        if (materialComp) {
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4.0f, 6.0f));
            if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed)) {
                ImGui::Spacing();
                // Base Color
                Math::Vector3 baseColor = materialComp->GetBaseColor();
                float baseColorArray[3] = { baseColor.x, baseColor.y, baseColor.z };
                ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.8f, 1.0f), "Base Color");
                ImGui::PushItemWidth(-1);
                if (ImGui::ColorEdit3("##BaseColor", baseColorArray, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel)) {
                    materialComp->SetBaseColor(Math::Vector3(baseColorArray[0], baseColorArray[1], baseColorArray[2]));
                }
                ImGui::PopItemWidth();
                ImGui::Spacing();
                
                // Metallic
                float metallic = materialComp->GetMetallic();
                ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.8f, 1.0f), "Metallic");
                ImGui::PushItemWidth(-1);
                if (ImGui::SliderFloat("##Metallic", &metallic, 0.0f, 1.0f, "%.2f")) {
                    materialComp->SetMetallic(metallic);
                }
                ImGui::PopItemWidth();
                ImGui::Spacing();
                
                // Roughness
                float roughness = materialComp->GetRoughness();
                ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.8f, 1.0f), "Roughness");
                ImGui::PushItemWidth(-1);
                if (ImGui::SliderFloat("##Roughness", &roughness, 0.0f, 1.0f, "%.2f")) {
                    materialComp->SetRoughness(roughness);
                }
                ImGui::PopItemWidth();
                ImGui::Spacing();
                
                // Specular
                float specular = materialComp->GetSpecular();
                ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.8f, 1.0f), "Specular");
                ImGui::PushItemWidth(-1);
                if (ImGui::SliderFloat("##Specular", &specular, 0.0f, 1.0f, "%.2f")) {
                    materialComp->SetSpecular(specular);
                }
                ImGui::PopItemWidth();
                ImGui::Spacing();
                
                // Emissive Color
                Math::Vector3 emissive = materialComp->GetEmissiveColor();
                float emissiveArray[3] = { emissive.x, emissive.y, emissive.z };
                ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.8f, 1.0f), "Emissive Color");
                ImGui::PushItemWidth(-1);
                if (ImGui::ColorEdit3("##Emissive", emissiveArray, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel)) {
                    materialComp->SetEmissiveColor(Math::Vector3(emissiveArray[0], emissiveArray[1], emissiveArray[2]));
                }
                ImGui::PopItemWidth();
                ImGui::Spacing();
                
                // Opacity
                float opacity = materialComp->GetOpacity();
                ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.8f, 1.0f), "Opacity");
                ImGui::PushItemWidth(-1);
                if (ImGui::SliderFloat("##Opacity", &opacity, 0.0f, 1.0f, "%.2f")) {
                    materialComp->SetOpacity(opacity);
                }
                ImGui::PopItemWidth();
                ImGui::Spacing();
                
                ImGui::Separator();
                ImGui::Spacing();
                ImGui::TextColored(ImVec4(0.9f, 0.9f, 0.95f, 1.0f), "Textures");
                ImGui::Spacing();
                
                // Helper function to display texture slot
                auto ShowTextureSlot = [&](const char* label, std::shared_ptr<Texture> texture, 
                                          std::function<void(std::shared_ptr<Texture>)> setter,
                                          TextureColorSpace colorSpace = TextureColorSpace::sRGB) {
                    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6.0f, 4.0f));
                    ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.8f, 1.0f), label);
                    ImGui::SameLine(120.0f);
                    
                    // Show texture status
                    if (texture) {
                        ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "[Loaded]");
                        ImGui::SameLine();
                        ImGui::Text("(%dx%d)", texture->GetWidth(), texture->GetHeight());
                        ImGui::SameLine();
                        if (ImGui::Button(("Remove##" + std::string(label)).c_str())) {
                            setter(nullptr);
                        }
                    } else {
                        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "[None]");
                    }
                    
                    // File path input (use static map to persist paths)
                    static std::map<std::string, std::string> texturePaths;
                    std::string key = std::string(label) + "_" + std::to_string(m_SelectedObject->GetID());
                    
                    if (texturePaths.find(key) == texturePaths.end()) {
                        texturePaths[key] = "";
                    }
                    
                    char pathBuffer[512];
                    strncpy_s(pathBuffer, texturePaths[key].c_str(), sizeof(pathBuffer) - 1);
                    pathBuffer[sizeof(pathBuffer) - 1] = '\0';
                    
                    ImGui::PushItemWidth(-120.0f);
                    if (ImGui::InputText(("##Path" + std::string(label)).c_str(), pathBuffer, sizeof(pathBuffer))) {
                        texturePaths[key] = std::string(pathBuffer);
                    }
                    ImGui::PopItemWidth();
                    
                    ImGui::SameLine();
                    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8.0f, 4.0f));
                    if (ImGui::Button(("Load##" + std::string(label)).c_str())) {
                        std::string filepath = texturePaths[key];
                        if (!filepath.empty()) {
                            auto newTexture = std::make_shared<Texture>();
                            
                            // Try to load as regular image first
                            if (newTexture->LoadImageFile(filepath, colorSpace)) {
                                setter(newTexture);
                                Log::Info("Loaded texture: " + filepath);
                            } else if (newTexture->LoadHDRImage(filepath)) {
                                setter(newTexture);
                                Log::Info("Loaded HDR texture: " + filepath);
                            } else {
                                Log::Error("Failed to load texture: " + filepath);
                            }
                        }
                    }
                    ImGui::PopStyleVar();
                    ImGui::PopStyleVar();
                    ImGui::Spacing();
                };
                
                // Base Color Texture (sRGB for albedo)
                ShowTextureSlot("Albedo/Base Color", materialComp->GetBaseColorTexture(), 
                    [&](std::shared_ptr<Texture> tex) { materialComp->SetBaseColorTexture(tex); },
                    TextureColorSpace::sRGB);
                
                // Metallic Texture (Linear - data texture)
                ShowTextureSlot("Metallic", materialComp->GetMetallicTexture(),
                    [&](std::shared_ptr<Texture> tex) { materialComp->SetMetallicTexture(tex); },
                    TextureColorSpace::Linear);
                
                // Roughness Texture (Linear - data texture)
                ShowTextureSlot("Roughness", materialComp->GetRoughnessTexture(),
                    [&](std::shared_ptr<Texture> tex) { materialComp->SetRoughnessTexture(tex); },
                    TextureColorSpace::Linear);
                
                // Normal Texture (Linear - data texture)
                ShowTextureSlot("Normal Map", materialComp->GetNormalTexture(),
                    [&](std::shared_ptr<Texture> tex) { materialComp->SetNormalTexture(tex); },
                    TextureColorSpace::Linear);
                
                // AO Texture (Linear - data texture)
                ShowTextureSlot("Ambient Occlusion", materialComp->GetAOTexture(),
                    [&](std::shared_ptr<Texture> tex) { materialComp->SetAOTexture(tex); },
                    TextureColorSpace::Linear);
                
                // Emissive Texture (sRGB - color texture)
                ShowTextureSlot("Emissive", materialComp->GetEmissiveTexture(),
                    [&](std::shared_ptr<Texture> tex) { materialComp->SetEmissiveTexture(tex); },
                    TextureColorSpace::sRGB);
                
                // Opacity Texture (Linear - data texture)
                ShowTextureSlot("Opacity", materialComp->GetOpacityTexture(),
                    [&](std::shared_ptr<Texture> tex) { materialComp->SetOpacityTexture(tex); },
                    TextureColorSpace::Linear);
                
                // Height Texture (Linear - data texture)
                ShowTextureSlot("Height/Displacement", materialComp->GetHeightTexture(),
                    [&](std::shared_ptr<Texture> tex) { materialComp->SetHeightTexture(tex); },
                    TextureColorSpace::Linear);
                
                ImGui::Separator();
                ImGui::Spacing();
                
                // Render Mode
                ImGui::TextColored(ImVec4(0.9f, 0.9f, 0.95f, 1.0f), "Render Settings");
                ImGui::Spacing();
                
                BlendMode blendMode = materialComp->GetBlendMode();
                const char* blendModes[] = { "Opaque", "Masked", "Translucent", "Additive", "Modulate" };
                int currentBlendMode = static_cast<int>(blendMode);
                ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.8f, 1.0f), "Blend Mode");
                ImGui::PushItemWidth(-1);
                if (ImGui::Combo("##BlendMode", &currentBlendMode, blendModes, IM_ARRAYSIZE(blendModes))) {
                    materialComp->SetBlendMode(static_cast<BlendMode>(currentBlendMode));
                }
                ImGui::PopItemWidth();
                ImGui::Spacing();
                
                CullMode cullMode = materialComp->GetCullMode();
                const char* cullModes[] = { "None", "Front", "Back" };
                int currentCullMode = static_cast<int>(cullMode);
                ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.8f, 1.0f), "Cull Mode");
                ImGui::PushItemWidth(-1);
                if (ImGui::Combo("##CullMode", &currentCullMode, cullModes, IM_ARRAYSIZE(cullModes))) {
                    materialComp->SetCullMode(static_cast<CullMode>(currentCullMode));
                }
                ImGui::PopItemWidth();
                ImGui::Spacing();
                
                DepthWriteMode depthWrite = materialComp->GetDepthWrite();
                bool depthWriteEnabled = (depthWrite == DepthWriteMode::Enabled);
                ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.8f, 1.0f), "Depth Write");
                if (ImGui::Checkbox("##DepthWrite", &depthWriteEnabled)) {
                    materialComp->SetDepthWrite(depthWriteEnabled ? DepthWriteMode::Enabled : DepthWriteMode::Disabled);
                }
                ImGui::Spacing();
                
                // Show if using instance
                if (materialComp->IsUsingInstance()) {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.4f, 0.9f, 0.6f, 1.0f));
                    ImGui::Text("Using Material Instance");
                    ImGui::PopStyleColor();
                }
                ImGui::PopStyleVar();
            }
        } else {
            // Add Material Component button with modern styling
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(12.0f, 6.0f));
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.50f, 0.75f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.30f, 0.55f, 0.80f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.20f, 0.45f, 0.70f, 1.0f));
            if (ImGui::Button("Add Material Component", ImVec2(-1, 0))) {
                auto newMaterialComp = std::make_shared<MaterialComponent>();
                m_SelectedObject->AddComponent(newMaterialComp);
            }
            ImGui::PopStyleColor(3);
            ImGui::PopStyleVar();
        }
    }
    
    ImGui::End();
}

} // namespace LGE

