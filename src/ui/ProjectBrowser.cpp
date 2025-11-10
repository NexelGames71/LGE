#include "LGE/ui/ProjectBrowser.h"
#include "LGE/rendering/Texture.h"
#include "LGE/core/Log.h"
#include "LGE/core/FileSystem.h"
#include "imgui.h"
#include <filesystem>
#include <algorithm>
#include <cstring>
#include <fstream>
#include <ctime>

namespace LGE {

ProjectBrowser::ProjectBrowser()
    : m_IsOpen(true)
    , m_ProjectCreated(false)
    , m_CreatedProjectPath("")
    , m_SelectedCategory(ProjectCategory::Games)
    , m_SelectedTemplateIndex(-1)
    , m_IncludeStarterContent(false)
    , m_ThumbnailsLoaded(false)
{
    // Initialize project location to default
    #ifdef _WIN32
    strncpy_s(m_ProjectLocationBuffer, "C:\\Projects", sizeof(m_ProjectLocationBuffer) - 1);
    #else
    strncpy(m_ProjectLocationBuffer, "/home/user/Projects", sizeof(m_ProjectLocationBuffer) - 1);
    #endif
    m_ProjectLocationBuffer[sizeof(m_ProjectLocationBuffer) - 1] = '\0';
    
    strncpy_s(m_ProjectNameBuffer, "MyProject", sizeof(m_ProjectNameBuffer) - 1);
    m_ProjectNameBuffer[sizeof(m_ProjectNameBuffer) - 1] = '\0';
    
    LoadTemplates();
    LoadCategoryThumbnails();
}

ProjectBrowser::~ProjectBrowser() {
}

void ProjectBrowser::LoadTemplates() {
    m_Templates.clear();
    
    // Blank Template
    ProjectTemplate blank;
    blank.name = "Blank";
    blank.description = "A clean empty project with no code.";
    blank.category = ProjectCategory::Games;
        blank.thumbnailPath = "unreal-engine-editor-icons/imgs/GameProjectDialog/blank_project_thumbnail.png";
    blank.previewPath = "unreal-engine-editor-icons/imgs/GameProjectDialog/blank_project_preview.png";
    blank.hasStarterContent = false;
    m_Templates.push_back(blank);
    
    // First Person Template
    ProjectTemplate firstPerson;
    firstPerson.name = "First Person";
    firstPerson.description = "A first-person game template with character movement and basic gameplay.";
    firstPerson.category = ProjectCategory::Games;
    firstPerson.thumbnailPath = "assets/icons/First_Person_Thumbnail.png";
    firstPerson.previewPath = "assets/icons/First_Person_Thumbnail.png";
    firstPerson.hasStarterContent = true;
    m_Templates.push_back(firstPerson);
    
    // Third Person Template
    ProjectTemplate thirdPerson;
    thirdPerson.name = "Third Person";
    thirdPerson.description = "A third-person game template with character controller and camera system.";
    thirdPerson.category = ProjectCategory::Games;
    thirdPerson.thumbnailPath = "assets/icons/Third_Person_Thumbnail.png";
    thirdPerson.previewPath = "assets/icons/Third_Person_Thumbnail.png";
    thirdPerson.hasStarterContent = true;
    m_Templates.push_back(thirdPerson);
    
    // Top Down Template
    ProjectTemplate topDown;
    topDown.name = "Top Down";
    topDown.description = "A top-down game template with isometric camera and basic controls.";
    topDown.category = ProjectCategory::Games;
    topDown.thumbnailPath = "assets/icons/Default_Thumbnail.png";
    topDown.hasStarterContent = true;
    m_Templates.push_back(topDown);
    
    // Vehicle Template
    ProjectTemplate vehicle;
    vehicle.name = "Vehicle";
    vehicle.description = "A vehicle game template with car physics and controls.";
    vehicle.category = ProjectCategory::Games;
    vehicle.thumbnailPath = "assets/icons/Default_Thumbnail.png";
    vehicle.hasStarterContent = true;
    m_Templates.push_back(vehicle);
    
    // Handheld AR Template
    ProjectTemplate handheldAR;
    handheldAR.name = "Handheld AR";
    handheldAR.description = "A handheld AR template for mobile devices.";
    handheldAR.category = ProjectCategory::Games;
    handheldAR.thumbnailPath = "assets/icons/Default_Thumbnail.png";
    handheldAR.hasStarterContent = true;
    m_Templates.push_back(handheldAR);
    
    // Virtual Reality Template
    ProjectTemplate vr;
    vr.name = "Virtual Reality";
    vr.description = "A VR template using OpenXR for Desktop, Console, and Mobile. Features teleport locomotion, snap turning, grabbable objects, and VR Spectator Camera.";
    vr.category = ProjectCategory::Games;
    vr.thumbnailPath = "assets/icons/Default_Thumbnail.png";
    vr.assetTypeReferences = {"Sound Cue", "Haptic Feedback Effect Curve", "Nixtura System"};
    vr.hasStarterContent = false;
    m_Templates.push_back(vr);
    
    // Filter templates for current category
    UpdateFilteredTemplates();
}

void ProjectBrowser::UpdateFilteredTemplates() {
    m_FilteredTemplates.clear();
    for (const auto& tmpl : m_Templates) {
        if (tmpl.category == m_SelectedCategory) {
            m_FilteredTemplates.push_back(tmpl);
        }
    }
    
    // Reset selection if current selection is invalid
    if (m_SelectedTemplateIndex >= static_cast<int>(m_FilteredTemplates.size())) {
        m_SelectedTemplateIndex = -1;
    }
}

void ProjectBrowser::LoadCategoryThumbnails() {
    if (m_ThumbnailsLoaded) return;
    
    // Load category thumbnails
    std::vector<std::string> recentPaths = {
        "unreal-engine-editor-icons/imgs/Icons/Folders/Folder_Base_512x.png",
        "assets/icons/folder.png"
    };
    
    m_RecentThumbnail = std::make_shared<Texture>();
    for (const auto& path : recentPaths) {
        if (m_RecentThumbnail->LoadImageFile(path)) {
            break;
        }
    }
    
    // Load Games category thumbnail
    std::vector<std::string> gamesPaths = {
        "assets/icons/Games_Thumbnails.png",
        "assets/icons/Default_Thumbnail.png",
        "unreal-engine-editor-icons/imgs/Icons/Folders/Folder_Base_512x.png"
    };
    
    m_GamesThumbnail = std::make_shared<Texture>();
    for (const auto& path : gamesPaths) {
        if (m_GamesThumbnail->LoadImageFile(path)) {
            break;
        }
    }
    
    if (!m_GamesThumbnail || m_GamesThumbnail->GetRendererID() == 0) {
        m_GamesThumbnail = m_RecentThumbnail; // Fallback
    }
    
    // Load template thumbnails and previews
    m_TemplateThumbnails.clear();
    m_TemplatePreviews.clear();
    for (const auto& tmpl : m_Templates) {
        // Load thumbnail
        auto thumb = std::make_shared<Texture>();
        if (thumb->LoadImageFile(tmpl.thumbnailPath)) {
            m_TemplateThumbnails.push_back(thumb);
        } else {
            m_TemplateThumbnails.push_back(m_RecentThumbnail); // Fallback
        }
        
        // Load preview (if available)
        if (!tmpl.previewPath.empty()) {
            auto preview = std::make_shared<Texture>();
            if (preview->LoadImageFile(tmpl.previewPath)) {
                m_TemplatePreviews.push_back(preview);
            } else {
                m_TemplatePreviews.push_back(nullptr);
            }
        } else {
            m_TemplatePreviews.push_back(nullptr);
        }
    }
    
    m_ThumbnailsLoaded = true;
}

void ProjectBrowser::OnUIRender() {
    if (!m_IsOpen) return;
    
    // No need for modal dimming background - the main window is cleared with Project Browser background color
    // Set window to be modal and centered - same size as splash screen
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 center = viewport->GetCenter();
    // Use same sizing as splash screen (full viewport size)
    ImVec2 windowSize = viewport->Size;
    ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
    ImGui::SetNextWindowViewport(viewport->ID);
    
    // Window flags - modal dialog style (removed AlwaysAutoResize to allow fixed size)
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                             ImGuiWindowFlags_NoDocking;
    
    // Dark background
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.12f, 0.12f, 0.14f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.25f, 0.25f, 0.28f, 1.0f));
    
    if (ImGui::Begin("Project Browser", &m_IsOpen, flags)) {
        RenderTopBar();
        ImGui::Separator();
        
        // Main content area - split into left nav, center grid, right details
        float availableHeight = ImGui::GetContentRegionAvail().y - 80.0f; // Reserve more space for bottom bar
        ImGui::BeginChild("MainContent", ImVec2(0, availableHeight), false);
        
        float totalWidth = ImGui::GetContentRegionAvail().x;
        float spacing = ImGui::GetStyle().ItemSpacing.x * 2.0f; // Account for SameLine spacing
        
        // Left Navigation Panel (25% width)
        float leftPanelWidth = totalWidth * 0.25f - spacing;
        ImGui::BeginChild("LeftNav", ImVec2(leftPanelWidth, 0), true);
        RenderLeftNavigation();
        ImGui::EndChild();
        
        ImGui::SameLine();
        
        // Center Template Grid (50% width)
        float centerWidth = totalWidth * 0.5f - spacing;
        ImGui::BeginChild("TemplateGrid", ImVec2(centerWidth, 0), true);
        RenderTemplateGrid();
        ImGui::EndChild();
        
        ImGui::SameLine();
        
        // Right Details Panel (25% width - remaining space)
        ImGui::BeginChild("DetailsPanel", ImVec2(0, 0), true);
        RenderDetailsPanel();
        ImGui::EndChild();
        
        ImGui::EndChild();
        
        ImGui::Separator();
        
        // Bottom Project Settings Bar
        RenderBottomBar();
    }
    ImGui::End();
    
    ImGui::PopStyleColor(2);
}

void ProjectBrowser::RenderTopBar() {
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 4.0f));
    
    // Title centered
    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("Project Browser").x) * 0.5f);
    ImGui::TextColored(ImVec4(0.9f, 0.9f, 0.95f, 1.0f), "Project Browser");
    
    ImGui::PopStyleVar();
}

void ProjectBrowser::RenderLeftNavigation() {
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4.0f, 8.0f));
    
    auto RenderCategory = [this](const char* label, ProjectCategory category, std::shared_ptr<Texture> thumbnail) {
        bool isSelected = (m_SelectedCategory == category);
        
        if (isSelected) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.40f, 0.70f, 1.00f, 0.40f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.40f, 0.70f, 1.00f, 0.50f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.40f, 0.70f, 1.00f, 0.60f));
        }
        
        ImVec2 buttonSize(ImGui::GetContentRegionAvail().x, 80.0f);
        
        // Create invisible button for interaction
        std::string buttonId = std::string("##") + label;
        if (ImGui::Button(buttonId.c_str(), buttonSize)) {
            m_SelectedCategory = category;
            UpdateFilteredTemplates();
            m_SelectedTemplateIndex = -1;
        }
        
        // Draw thumbnail and label
        ImVec2 pos = ImGui::GetItemRectMin();
        ImVec2 size = ImGui::GetItemRectSize();
        
        if (thumbnail && thumbnail->GetRendererID() != 0) {
            float thumbSize = 48.0f;
            ImVec2 thumbPos(pos.x + 8.0f, pos.y + (size.y - thumbSize) * 0.5f);
            
            ImGui::GetWindowDrawList()->AddImage(
                reinterpret_cast<void*>(static_cast<intptr_t>(thumbnail->GetRendererID())),
                thumbPos, ImVec2(thumbPos.x + thumbSize, thumbPos.y + thumbSize),
                ImVec2(0, 0), ImVec2(1, 1)
            );
        }
        
        // Draw label text
        ImVec2 textPos = pos;
        textPos.x += 64.0f; // Offset for thumbnail
        textPos.y += (size.y - ImGui::GetTextLineHeight()) * 0.5f;
        ImGui::GetWindowDrawList()->AddText(textPos, IM_COL32(255, 255, 255, 255), label);
        
        if (isSelected) {
            ImGui::PopStyleColor(3);
        }
    };
    
    RenderCategory("RECENT PROJECTS", ProjectCategory::Recent, m_RecentThumbnail);
    RenderCategory("GAMES", ProjectCategory::Games, m_GamesThumbnail);
    
    ImGui::PopStyleVar();
}

void ProjectBrowser::RenderTemplateGrid() {
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
    
    // Grid layout
    float availableWidth = ImGui::GetContentRegionAvail().x;
    float thumbnailSize = 120.0f;
    int columns = static_cast<int>(availableWidth / (thumbnailSize + 16.0f));
    if (columns < 1) columns = 1;
    
    ImGui::Columns(columns, nullptr, false);
    
    for (size_t i = 0; i < m_FilteredTemplates.size(); i++) {
        const auto& tmpl = m_FilteredTemplates[i];
        bool isSelected = (m_SelectedTemplateIndex == static_cast<int>(i));
        
        ImGui::PushID(static_cast<int>(i));
        
        if (isSelected) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.40f, 0.70f, 1.00f, 0.40f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.40f, 0.70f, 1.00f, 0.50f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.40f, 0.70f, 1.00f, 0.60f));
        }
        
        // Template button
        ImVec2 buttonSize(thumbnailSize, thumbnailSize);
        if (ImGui::Button("##Template", buttonSize)) {
            m_SelectedTemplateIndex = static_cast<int>(i);
        }
        
        // Draw thumbnail - fill entire slot
        if (i < m_TemplateThumbnails.size() && m_TemplateThumbnails[i] && m_TemplateThumbnails[i]->GetRendererID() != 0) {
            ImVec2 pos = ImGui::GetItemRectMin();
            ImVec2 size = ImGui::GetItemRectSize();
            
            ImGui::GetWindowDrawList()->AddImage(
                reinterpret_cast<void*>(static_cast<intptr_t>(m_TemplateThumbnails[i]->GetRendererID())),
                pos, ImVec2(pos.x + size.x, pos.y + size.y),
                ImVec2(0, 0), ImVec2(1, 1)
            );
        }
        
        if (isSelected) {
            ImGui::PopStyleColor(3);
        }
        
        // Template name below thumbnail
        ImVec2 textSize = ImGui::CalcTextSize(tmpl.name.c_str());
        ImVec2 cursorPos = ImGui::GetCursorPos();
        ImGui::SetCursorPosX(cursorPos.x + (thumbnailSize - textSize.x) * 0.5f);
        ImGui::Text(tmpl.name.c_str());
        
        ImGui::NextColumn();
        ImGui::PopID();
    }
    
    ImGui::Columns(1);
    ImGui::PopStyleVar();
}

void ProjectBrowser::RenderDetailsPanel() {
    if (m_SelectedTemplateIndex < 0 || m_SelectedTemplateIndex >= static_cast<int>(m_FilteredTemplates.size())) {
        ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "No template selected");
        return;
    }
    
    const auto& tmpl = m_FilteredTemplates[m_SelectedTemplateIndex];
    
    // Find the original template index to get preview
    int originalIndex = -1;
    for (size_t i = 0; i < m_Templates.size(); i++) {
        if (m_Templates[i].name == tmpl.name) {
            originalIndex = static_cast<int>(i);
            break;
        }
    }
    
    // Title
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4.0f, 8.0f));
    ImGui::TextColored(ImVec4(0.9f, 0.9f, 0.95f, 1.0f), tmpl.name.c_str());
    ImGui::Spacing();
    
    // Preview image (if available)
    if (originalIndex >= 0 && originalIndex < static_cast<int>(m_TemplatePreviews.size())) {
        auto preview = m_TemplatePreviews[originalIndex];
        if (preview && preview->GetRendererID() != 0) {
            float previewWidth = ImGui::GetContentRegionAvail().x;
            float previewHeight = previewWidth * (static_cast<float>(preview->GetHeight()) / static_cast<float>(preview->GetWidth()));
            
            ImGui::Image(
                reinterpret_cast<void*>(static_cast<intptr_t>(preview->GetRendererID())),
                ImVec2(previewWidth, previewHeight),
                ImVec2(0, 0), ImVec2(1, 1)
            );
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
        }
    }
    
    // Description
    ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.8f, 1.0f), "Description:");
    ImGui::Spacing();
    ImGui::TextWrapped(tmpl.description.c_str());
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    
    // Asset Type References
    if (!tmpl.assetTypeReferences.empty()) {
        ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.8f, 1.0f), "Asset Type References:");
        ImGui::Spacing();
        for (const auto& ref : tmpl.assetTypeReferences) {
            ImGui::BulletText(ref.c_str());
        }
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
    }
    
    // Project Defaults
    ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.8f, 1.0f), "Project Defaults:");
    ImGui::Spacing();
    ImGui::Text("Default settings for this template");
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    
    // Starter Content
    ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.8f, 1.0f), "Starter Content:");
    ImGui::Spacing();
    if (ImGui::Checkbox("Include Starter Content", &m_IncludeStarterContent)) {
        // Update template's starter content flag
    }
    
    ImGui::PopStyleVar();
}

void ProjectBrowser::RenderBottomBar() {
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 4.0f));
    
    float windowWidth = ImGui::GetWindowWidth();
    float availableWidth = ImGui::GetContentRegionAvail().x;
    
    // Project Location (left side)
    ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.8f, 1.0f), "Project Location:");
    ImGui::SameLine();
    
    // Calculate available width for location input
    float locationLabelWidth = ImGui::CalcTextSize("Project Location:").x;
    float browseButtonWidth = 80.0f;
    float spacing = ImGui::GetStyle().ItemSpacing.x * 2.0f; // Account for SameLine spacing
    float locationInputWidth = std::max(200.0f, (availableWidth * 0.4f) - locationLabelWidth - browseButtonWidth - spacing);
    
    ImGui::PushItemWidth(locationInputWidth);
    ImGui::InputText("##Location", m_ProjectLocationBuffer, sizeof(m_ProjectLocationBuffer));
    ImGui::PopItemWidth();
    
    ImGui::SameLine();
    if (ImGui::Button("Browse...", ImVec2(browseButtonWidth, 0))) {
        // TODO: Open folder browser dialog
        Log::Info("ProjectBrowser: Browse button clicked - Folder dialog not yet implemented");
    }
    
    // Project Name and buttons (right side) - use remaining space
    float rightSideStart = windowWidth - 400.0f; // Reserve space for name + buttons
    if (rightSideStart > ImGui::GetCursorPosX() + 20.0f) {
        ImGui::SetCursorPosX(rightSideStart);
    } else {
        ImGui::NewLine(); // Wrap to next line if needed
    }
    
    // Project Name
    ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.8f, 1.0f), "Project Name:");
    ImGui::SameLine();
    ImGui::PushItemWidth(150.0f);
    ImGui::InputText("##Name", m_ProjectNameBuffer, sizeof(m_ProjectNameBuffer));
    ImGui::PopItemWidth();
    
    ImGui::SameLine();
    
    // Cancel Button
    if (ImGui::Button("Cancel", ImVec2(80, 0))) {
        m_IsOpen = false;
    }
    
    ImGui::SameLine();
    
    // Create Button (prominent blue)
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.50f, 0.75f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.30f, 0.55f, 0.80f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.20f, 0.45f, 0.70f, 1.0f));
    
    if (ImGui::Button("Create", ImVec2(80, 0))) {
        CreateProject();
    }
    
    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar();
}

void ProjectBrowser::CreateProject() {
    if (m_SelectedTemplateIndex < 0 || m_SelectedTemplateIndex >= static_cast<int>(m_FilteredTemplates.size())) {
        Log::Error("ProjectBrowser: No template selected");
        return;
    }
    
    std::string projectLocation = std::string(m_ProjectLocationBuffer);
    std::string projectName = std::string(m_ProjectNameBuffer);
    
    if (projectLocation.empty() || projectName.empty()) {
        Log::Error("ProjectBrowser: Project location and name must be specified");
        return;
    }
    
    // Create project directory
    std::filesystem::path projectPath = std::filesystem::path(projectLocation) / projectName;
    
    try {
        // Create project directory
        if (!std::filesystem::exists(projectPath)) {
            std::filesystem::create_directories(projectPath);
        }
        
        // Create Assets directory - this is where all project assets will be located
        std::filesystem::path assetsPath = projectPath / "Assets";
        if (!std::filesystem::exists(assetsPath)) {
            std::filesystem::create_directories(assetsPath);
            Log::Info("ProjectBrowser: Created Assets folder: " + assetsPath.string());
        } else {
            Log::Info("ProjectBrowser: Assets folder already exists: " + assetsPath.string());
        }
        
        // Create Scenes directory inside Assets
        std::filesystem::path scenesPath = assetsPath / "Scenes";
        if (!std::filesystem::exists(scenesPath)) {
            std::filesystem::create_directories(scenesPath);
            Log::Info("ProjectBrowser: Created Scenes folder: " + scenesPath.string());
        }
        
        // Create default scene SampleScene.scene
        std::filesystem::path defaultScenePath = scenesPath / "SampleScene.scene";
        std::ofstream sceneFile(defaultScenePath);
        if (sceneFile.is_open()) {
            sceneFile << "{\n";
            sceneFile << "  \"Name\": \"SampleScene\",\n";
            sceneFile << "  \"Type\": \"Scene\",\n";
            sceneFile << "  \"GameObjects\": [],\n";
            sceneFile << "  \"Camera\": {\n";
            sceneFile << "    \"Position\": [0.0, 0.0, 3.0],\n";
            sceneFile << "    \"Target\": [0.0, 0.0, 0.0],\n";
            sceneFile << "    \"FOV\": 45.0\n";
            sceneFile << "  },\n";
            sceneFile << "  \"Lighting\": {\n";
            sceneFile << "    \"DirectionalLight\": {\n";
            sceneFile << "      \"Direction\": [0.3, -0.8, 0.5],\n";
            sceneFile << "      \"Color\": [1.0, 0.98, 0.95],\n";
            sceneFile << "      \"Intensity\": 0.8\n";
            sceneFile << "    }\n";
            sceneFile << "  }\n";
            sceneFile << "}\n";
            sceneFile.close();
        }
        
        // Create project file (simple .lgeproject file)
        std::filesystem::path projectFile = projectPath / (projectName + ".lgeproject");
        std::ofstream file(projectFile);
        if (file.is_open()) {
            file << "{\n";
            file << "  \"Name\": \"" << projectName << "\",\n";
            file << "  \"Version\": \"1.0.0\",\n";
            file << "  \"Template\": \"" << m_FilteredTemplates[m_SelectedTemplateIndex].name << "\",\n";
            file << "  \"Created\": \"" << std::time(nullptr) << "\"\n";
            file << "}\n";
            file.close();
        }
        
        // Save the project file path (not just the directory)
        // Normalize path separators for consistency
        std::string projectFilePath = std::filesystem::absolute(projectFile).string();
        std::replace(projectFilePath.begin(), projectFilePath.end(), '\\', '/');
        m_CreatedProjectPath = projectFilePath;
        m_ProjectCreated = true;
        m_IsOpen = false;
        
        Log::Info("ProjectBrowser: Created project: " + m_CreatedProjectPath);
        
    } catch (const std::exception& e) {
        Log::Error("ProjectBrowser: Failed to create project: " + std::string(e.what()));
    }
}

} // namespace LGE

