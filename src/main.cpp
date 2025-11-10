#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "LGE/core/Application.h"
#include "LGE/rendering/Renderer.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <filesystem>
#include "LGE/core/Log.h"
#include "LGE/core/FileSystem.h"
#include "LGE/core/Window.h"
#include "LGE/rendering/Shader.h"
#include "LGE/rendering/VertexBuffer.h"
#include "LGE/rendering/VertexArray.h"
#include "LGE/rendering/Camera.h"
#include "LGE/rendering/CameraController.h"
#include "LGE/rendering/Skybox.h"
#include "LGE/rendering/DirectionalLight.h"
#include "LGE/rendering/Material.h"
#include "LGE/ui/UI.h"
#include "LGE/ui/SceneViewport.h"
#include "LGE/ui/Hierarchy.h"
#include "LGE/ui/Inspector.h"
#include "LGE/ui/ContentBrowser.h"
#include "LGE/ui/Toolbar.h"
#include "LGE/ui/MainMenuBar.h"
#include "LGE/ui/Details.h"
#include "LGE/ui/ProjectBrowser.h"
#include "LGE/ui/SplashScreen.h"
#include "LGE/core/Input.h"
#include "LGE/core/GameObject.h"
#include "LGE/core/MaterialComponent.h"
#include "LGE/assets/AssetManager.h"
#include "LGE/assets/TextureImporter.h"
#include "LGE/assets/ImporterRegistry.h"
#include "LGE/assets/TextureExporter.h"
#include "LGE/assets/ExporterRegistry.h"
#include "imgui.h"
#include "imgui_internal.h"
#include <memory>
#include <cstdint>
#include <cmath>
#include <fstream>
#include <algorithm>

class ExampleApp : public LGE::Application {
public:
    ExampleApp() : LGE::Application("LGE Game Engine Example") {}

    bool Initialize() override {
        if (!LGE::Application::Initialize()) {
            return false;
        }

        // Initialize empty scene - no test content
        // GameObjects will be loaded from scene files

        // Don't initialize Asset Manager yet - wait until project is loaded
        // This will be initialized when a project is created or opened
        m_AssetManager = std::make_unique<LGE::AssetManager>();
        
        // Register importers
        auto textureImporter = std::make_shared<LGE::TextureImporter>();
        LGE::ImporterRegistry::Get().RegisterImporter(textureImporter);
        
        // Register exporters
        auto textureExporter = std::make_shared<LGE::TextureExporter>();
        LGE::ExporterRegistry::Get().RegisterExporter(textureExporter);
        
        // Setup UI panels
        m_ProjectBrowser = std::make_unique<LGE::ProjectBrowser>();
        m_SplashScreen = std::make_unique<LGE::SplashScreen>();
        m_MainMenuBar = std::make_unique<LGE::MainMenuBar>();
        m_Toolbar = std::make_unique<LGE::Toolbar>();
        m_Hierarchy = std::make_unique<LGE::Hierarchy>();
        m_Inspector = std::make_unique<LGE::Inspector>();
        m_ContentBrowser = std::make_unique<LGE::ContentBrowser>();
        m_SceneViewport = std::make_unique<LGE::SceneViewport>();
        m_Details = std::make_unique<LGE::Details>();
        
        // Setup MainMenuBar callbacks
        if (m_MainMenuBar) {
            m_MainMenuBar->SetOnSaveScene([this]() {
                ShowSaveSceneDialog();
            });
        }
        
        // Initialize project state
        m_ProjectLoaded = false;
        m_CurrentProjectPath = "";
        
        // Asset Manager will be connected to Content Browser when project is loaded
        // Scene viewport, hierarchy, and inspector will be set up when project is loaded
        
        // Setup camera (will be used when project is loaded)
        float aspectRatio = static_cast<float>(m_Window->GetWidth()) / static_cast<float>(m_Window->GetHeight());
        m_Camera = std::make_unique<LGE::Camera>();
        m_Camera->SetPerspective(45.0f, aspectRatio, 0.1f, 100.0f);
        m_Camera->SetPosition(LGE::Math::Vector3(0.0f, 0.0f, 3.0f));
        m_Camera->SetTarget(LGE::Math::Vector3(0.0f, 0.0f, 0.0f));
        
        // Setup initial docking layout (only on first run)
        static bool firstTime = true;
        if (firstTime) {
            firstTime = false;
            
            // Wait for next frame to ensure ImGui is ready
            // We'll set up the layout in OnRender after first frame
        }

        // Setup camera controller
        m_CameraController = std::make_unique<LGE::CameraController>(m_Camera.get());
        m_CameraController->SetMovementSpeed(5.0f);
        m_CameraController->SetPanSpeed(1.0f);  // Reduced pan speed for smoother panning
        m_CameraController->SetZoomSpeed(5.0f);  // Increased base zoom speed
        m_CameraController->SetMouseSensitivity(0.05f);
        m_CameraController->SetZoomLimits(0.5f, 20.0f);  // Min 0.5 units, Max 20 units

        // Setup directional light (sunlight from above)
        // Note: This only affects game objects, not the skybox brightness
        m_DirectionalLight = std::make_unique<LGE::DirectionalLight>();
        m_DirectionalLight->SetDirection(LGE::Math::Vector3(0.3f, -0.8f, 0.5f));  // Slight angle for better lighting
        m_DirectionalLight->SetColor(LGE::Math::Vector3(1.0f, 0.98f, 0.95f));  // Warm white
        m_DirectionalLight->SetIntensity(0.8f);  // Lower intensity so objects aren't over-lit

        // Setup skybox
        m_Skybox = std::make_unique<LGE::Skybox>();
        m_Skybox->Init();
        
        // Load HDR skybox texture (supports both .hdr and .exr)
        if (!m_Skybox->LoadHDR("assets/EXR Sky/kloofendal_48d_partly_cloudy_puresky_4k.exr")) {
            // Fallback to gradient if HDR fails to load
            m_Skybox->SetTopColor(LGE::Math::Vector3(0.5f, 0.7f, 1.0f));      // Light blue
            m_Skybox->SetBottomColor(LGE::Math::Vector3(0.8f, 0.9f, 1.0f));   // Light gray-blue
            LGE::Log::Warn("Failed to load HDR skybox, using gradient fallback");
        }

        LGE::Log::Info("Cube setup complete!");
        LGE::Log::Info("Camera initialized!");
        LGE::Log::Info("Skybox initialized!");
        LGE::Log::Info("Controls: WASD - Move, Right Click + Drag - Rotate, Mouse Wheel - Zoom, Middle Mouse + Drag - Pan");
        return true;
    }
    
    void LoadProject(const std::string& projectPath) {
        if (projectPath.empty()) {
            LGE::Log::Error("ExampleApp: Cannot load project - path is empty");
            return;
        }
        
        // Normalize the project path
        std::filesystem::path path(projectPath);
        if (!path.is_absolute()) {
            path = std::filesystem::absolute(path);
        }
        
        // If it's a project file (.lgeproject), extract the project directory
        std::string projectDir;
        if (path.extension() == ".lgeproject") {
            // It's a project file, get the parent directory
            projectDir = path.parent_path().string();
            LGE::Log::Info("ExampleApp: Project file detected, using directory: " + projectDir);
        } else {
            // It's a directory, use it directly
            projectDir = path.string();
            LGE::Log::Info("ExampleApp: Project directory detected: " + projectDir);
        }
        
        // Set project path (but don't mark as loaded yet - wait for splash screen)
        m_CurrentProjectPath = projectDir;
        
        // Close Project Browser and show Splash Screen
        if (m_ProjectBrowser) {
            m_ProjectBrowser->SetOpen(false);
        }
        
        if (m_SplashScreen) {
            m_SplashScreen->StartLoading();
            m_SplashScreen->SetOpen(true);
        }
        
        LGE::Log::Info("ExampleApp: Starting project load: " + m_CurrentProjectPath);
    }
    
    void CompleteProjectLoading() {
        // This is called after splash screen loading completes
        if (m_CurrentProjectPath.empty()) {
            return;
        }
        
        // Mark project as loaded
        m_ProjectLoaded = true;
        
        // Update Asset Manager to use project's Assets directory
        // The Assets folder is where all project assets are located
        if (m_AssetManager) {
            std::filesystem::path assetsPath = std::filesystem::path(m_CurrentProjectPath) / "Assets";
            
            // Ensure Assets directory exists - create if it doesn't
            if (!std::filesystem::exists(assetsPath)) {
                std::filesystem::create_directories(assetsPath);
                LGE::Log::Info("ExampleApp: Created Assets folder: " + assetsPath.string());
            }
            
            // Initialize Asset Manager with the project's Assets directory
            // All assets for this project will be located in this folder
            if (m_AssetManager->Initialize(assetsPath.string())) {
                LGE::Log::Info("ExampleApp: Asset Manager initialized with project Assets folder: " + assetsPath.string());
                
                // Scan assets in the project's Assets directory
                m_AssetManager->ScanAssets();
            } else {
                LGE::Log::Error("ExampleApp: Failed to initialize Asset Manager with project Assets path: " + assetsPath.string());
            }
        }
        
        // Connect Asset Manager to Content Browser
        if (m_ContentBrowser && m_AssetManager) {
            m_ContentBrowser->SetAssetManager(m_AssetManager.get());
        }
        
        // Set default scene path (SampleScene.scene in Assets/Scenes)
        std::filesystem::path defaultScenePath = std::filesystem::path(m_CurrentProjectPath) / "Assets" / "Scenes" / "SampleScene.scene";
        if (std::filesystem::exists(defaultScenePath)) {
            m_CurrentScenePath = std::filesystem::absolute(defaultScenePath).string();
            std::replace(m_CurrentScenePath.begin(), m_CurrentScenePath.end(), '\\', '/');
            LGE::Log::Info("ExampleApp: Loaded default scene: " + m_CurrentScenePath);
        }
        
        // Setup scene viewport, hierarchy, and inspector
        if (m_SceneViewport) {
            m_SceneViewport->SetCamera(m_Camera.get());
            m_SceneViewport->SetGameObjects(m_GameObjects);
            m_SceneViewport->SetSelectedObject(nullptr);
        }
        
        if (m_Hierarchy && m_Inspector) {
            m_Hierarchy->SetGameObjects(m_GameObjects);
            m_Hierarchy->SetSelectedObject(nullptr);
            m_Inspector->SetSelectedObject(nullptr);
        }
        
        // Close splash screen
        if (m_SplashScreen) {
            m_SplashScreen->SetOpen(false);
        }
        
        LGE::Log::Info("ExampleApp: Project loaded: " + m_CurrentProjectPath);
    }

    void OnUpdate(float deltaTime) override {
        // Only update if project is loaded
        if (!m_ProjectLoaded) {
            return;
        }
        
        // Update animation time
        m_Time += deltaTime;
        
        // Update camera controller with viewport state
        if (m_CameraController && m_SceneViewport) {
            bool viewportFocused = m_SceneViewport->IsFocused();
            bool viewportHovered = m_SceneViewport->IsHovered();
            m_CameraController->OnUpdate(deltaTime, viewportFocused, viewportHovered);
        }
    }

    void OnRender() override {
        // Render Splash Screen if open (after project creation/opening)
        if (m_SplashScreen && m_SplashScreen->IsOpen()) {
            // Set clear color to black for splash screen
            if (m_Renderer) {
                m_Renderer->Clear(0.0f, 0.0f, 0.0f, 1.0f);
            }
            
            m_SplashScreen->OnUIRender();
            
            // Check if loading is complete
            if (m_SplashScreen->IsLoadingComplete()) {
                CompleteProjectLoading();
            }
            return; // Don't render main editor UI while splash screen is open
        }
        
        // Render Project Browser first (if open or no project loaded)
        if (m_ProjectBrowser && (!m_ProjectLoaded || m_ProjectBrowser->IsOpen())) {
            // Set clear color to match Project Browser background (dark gray)
            if (m_Renderer) {
                m_Renderer->Clear(0.12f, 0.12f, 0.14f, 1.0f);
            }
            
            m_ProjectBrowser->OnUIRender();
            
            // If project was created, load it and close the browser
            if (m_ProjectBrowser->WasProjectCreated()) {
                std::string projectPath = m_ProjectBrowser->GetCreatedProjectPath();
                LoadProject(projectPath);
                m_ProjectBrowser->SetOpen(false);
            }
            return; // Don't render main editor UI while project browser is open or no project loaded
        }
        
        // Only render editor UI if a project is loaded
        if (!m_ProjectLoaded) {
            return;
        }
        
        // Render main menu bar
        if (m_MainMenuBar) {
            m_MainMenuBar->OnUIRender();
        }
        
        // Render save scene dialog
        RenderSaveSceneDialog();
        
        // Create dock space over viewport
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        
        // Account for menu bar and toolbar
        float menuBarHeight = ImGui::GetFrameHeight();
        float toolbarPadding = 2.0f; // Padding between menu bar and toolbar
        float toolbarHeight = 5.0f; // Match Toolbar.cpp height
        float topOffset = menuBarHeight + toolbarPadding + toolbarHeight;
        
        ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x, viewport->WorkPos.y + topOffset));
        ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x, viewport->WorkSize.y - topOffset));
        ImGui::SetNextWindowViewport(viewport->ID);
        
        ImGuiWindowFlags host_window_flags = 0;
        host_window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking;
        host_window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        host_window_flags |= ImGuiWindowFlags_MenuBar;
        
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        
        char label[32];
        snprintf(label, IM_ARRAYSIZE(label), "DockSpaceViewport_%08X", viewport->ID);
        
        ImGui::Begin(label, NULL, host_window_flags);
        ImGui::PopStyleVar(3);
        
        // Create dock space (below toolbar)
        ImGuiID dockspace_id = ImGui::GetID("DockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
        
        // Setup initial docking layout (only on first frame)
        static bool firstTime = true;
        if (firstTime) {
            firstTime = false;
            
            // Build initial layout using DockBuilder API (internal API)
            ImGuiDockNode* node = ImGui::DockBuilderGetNode(dockspace_id);
            if (node == nullptr) {
                ImGui::DockBuilderRemoveNode(dockspace_id); // Clear any existing layout
                ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
                ImGui::DockBuilderSetNodeSize(dockspace_id, ImVec2(viewport->WorkSize.x, viewport->WorkSize.y - topOffset));
                
                // Split the dock space - Hierarchy, Scene Viewport, Inspector, and Content Browser
                ImGuiID dockMain = dockspace_id;
                
                // Split left side for Hierarchy
                ImGuiID dockIdLeft = ImGui::DockBuilderSplitNode(dockMain, ImGuiDir_Left, 0.2f, nullptr, &dockMain);
                
                // Split right side for Inspector
                ImGuiID dockIdRight = ImGui::DockBuilderSplitNode(dockMain, ImGuiDir_Right, 0.25f, nullptr, &dockMain);
                
                // Split bottom for Content Browser (full width)
                ImGuiID dockIdBottom = ImGui::DockBuilderSplitNode(dockMain, ImGuiDir_Down, 0.25f, nullptr, &dockMain);
                
                // Dock windows
                ImGui::DockBuilderDockWindow("Hierarchy", dockIdLeft);  // Left
                ImGui::DockBuilderDockWindow("Scene Viewport", dockMain);  // Center
                ImGui::DockBuilderDockWindow("Inspector", dockIdRight);  // Right
                ImGui::DockBuilderDockWindow("Content Browser", dockIdBottom);  // Bottom (full width)
                
                ImGui::DockBuilderFinish(dockspace_id);
            }
        }
        
        // Render all dockable panels
        
        // Update GameObjects list in viewport (in case new objects were added)
        if (m_SceneViewport) {
            m_SceneViewport->SetGameObjects(m_GameObjects);
        }
        
        // Render hierarchy (only if project is loaded)
        if (m_Hierarchy && m_ProjectLoaded) {
            m_Hierarchy->OnUIRender();
            // Sync selection from Hierarchy to other panels
            if (m_Hierarchy->GetSelectedObject() != m_SceneViewport->GetSelectedObject()) {
                m_SceneViewport->SetSelectedObject(m_Hierarchy->GetSelectedObject());
                m_Inspector->SetSelectedObject(m_Hierarchy->GetSelectedObject());
            }
        }
        
        // Render scene viewport (only if project is loaded)
        if (m_SceneViewport && m_ProjectLoaded) {
            m_SceneViewport->OnUIRender();
            // Sync selection from SceneViewport to other panels
            if (m_SceneViewport->GetSelectedObject() != m_Hierarchy->GetSelectedObject()) {
                m_Hierarchy->SetSelectedObject(m_SceneViewport->GetSelectedObject());
                m_Inspector->SetSelectedObject(m_SceneViewport->GetSelectedObject());
            }
        }
        
        // Render inspector (only if project is loaded)
        if (m_Inspector && m_ProjectLoaded) {
            m_Inspector->OnUIRender();
        }
        
        // Render content browser (only if project is loaded)
        if (m_ContentBrowser && m_ProjectLoaded) {
            m_ContentBrowser->OnUpdate(0.016f); // Approximate delta time
            m_ContentBrowser->OnUIRender();
        }
        
        ImGui::End();
        
        // Render toolbar after dock space (so it appears on top, fixed below menu bar)
        if (m_Toolbar) {
            m_Toolbar->OnUIRender();
        }
        
        // Render scene to framebuffer (only if project is loaded)
        if (m_ProjectLoaded && m_SceneViewport && m_SceneViewport->GetWidth() > 0 && m_SceneViewport->GetHeight() > 0) {
            // Begin rendering to framebuffer
            m_SceneViewport->BeginRender();
            
            // Render skybox first (so it's behind everything) with cloud movement
            if (m_Skybox) {
                // Very slow horizontal cloud movement (simple linear offset)
                float cloudSpeed = 0.001f; // Extremely slow - subtle cloud drift
                float cloudOffset = m_Time * cloudSpeed;
                // Keep offset in 0-1 range for seamless looping
                cloudOffset = std::fmod(cloudOffset, 1.0f);
                m_Skybox->Render(*m_Camera, cloudOffset);
            }
            
            // Render GameObjects from scene
            // Scene rendering will be implemented when scene loading is complete
            
            // ImGuizmo is rendered in OnUIRender, not here
            
            // End rendering to framebuffer (restores viewport)
            m_SceneViewport->EndRender();
        }
    }

    void Shutdown() override {
        m_ContentBrowser.reset();
        m_Inspector.reset();
        m_Hierarchy.reset();
        m_Toolbar.reset();
        m_MainMenuBar.reset();
        m_SceneViewport.reset();
        m_Skybox.reset();
        m_CameraController.reset();
        m_Camera.reset();
        LGE::Application::Shutdown();
    }

private:
    std::vector<std::shared_ptr<LGE::GameObject>> m_GameObjects; // All GameObjects in the scene
    std::unique_ptr<LGE::Camera> m_Camera;
    std::unique_ptr<LGE::CameraController> m_CameraController;
    std::unique_ptr<LGE::Skybox> m_Skybox;
    std::unique_ptr<LGE::DirectionalLight> m_DirectionalLight;
    std::unique_ptr<LGE::SceneViewport> m_SceneViewport;
    std::unique_ptr<LGE::Details> m_Details;
    std::unique_ptr<LGE::Hierarchy> m_Hierarchy;
    std::unique_ptr<LGE::Inspector> m_Inspector;
    std::unique_ptr<LGE::ContentBrowser> m_ContentBrowser;
    std::unique_ptr<LGE::AssetManager> m_AssetManager;
    std::unique_ptr<LGE::Toolbar> m_Toolbar;
    std::unique_ptr<LGE::MainMenuBar> m_MainMenuBar;
    std::unique_ptr<LGE::ProjectBrowser> m_ProjectBrowser;
    std::unique_ptr<LGE::SplashScreen> m_SplashScreen;
    
    // Project state
    bool m_ProjectLoaded;
    std::string m_CurrentProjectPath;
    std::string m_CurrentScenePath; // Path to the current scene file
    
    // Save scene dialog state
    bool m_ShowSaveSceneDialog = false;
    char m_SceneNameBuffer[256] = "SampleScene";
    
    // Animation time
    float m_Time = 0.0f;
    
    // Save scene dialog
    void ShowSaveSceneDialog() {
        if (m_CurrentScenePath.empty()) {
            // If no scene is loaded, use default name
            strncpy_s(m_SceneNameBuffer, "SampleScene", sizeof(m_SceneNameBuffer) - 1);
        } else {
            // Extract scene name from current path
            std::filesystem::path scenePath(m_CurrentScenePath);
            std::string sceneName = scenePath.stem().string();
            strncpy_s(m_SceneNameBuffer, sceneName.c_str(), sizeof(m_SceneNameBuffer) - 1);
        }
        m_SceneNameBuffer[sizeof(m_SceneNameBuffer) - 1] = '\0';
        m_ShowSaveSceneDialog = true;
        ImGui::OpenPopup("Save Scene");
    }
    
    void RenderSaveSceneDialog() {
        if (!m_ShowSaveSceneDialog) return;
        
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowSize(ImVec2(400, 150), ImGuiCond_Appearing);
        
        if (ImGui::BeginPopupModal("Save Scene", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Enter scene name:");
            ImGui::Spacing();
            
            ImGui::PushItemWidth(350.0f);
            if (ImGui::InputText("##SceneName", m_SceneNameBuffer, sizeof(m_SceneNameBuffer))) {
                // Text changed
            }
            ImGui::PopItemWidth();
            
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
            
            // Buttons
            float buttonWidth = 80.0f;
            float spacing = ImGui::GetStyle().ItemSpacing.x;
            float totalWidth = buttonWidth * 2 + spacing;
            float startX = (ImGui::GetWindowWidth() - totalWidth) * 0.5f;
            
            ImGui::SetCursorPosX(startX);
            
            if (ImGui::Button("Cancel", ImVec2(buttonWidth, 0))) {
                m_ShowSaveSceneDialog = false;
                ImGui::CloseCurrentPopup();
            }
            
            ImGui::SameLine();
            
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.50f, 0.75f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.30f, 0.55f, 0.80f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.20f, 0.45f, 0.70f, 1.0f));
            
            if (ImGui::Button("Save", ImVec2(buttonWidth, 0))) {
                SaveScene();
                m_ShowSaveSceneDialog = false;
                ImGui::CloseCurrentPopup();
            }
            
            ImGui::PopStyleColor(3);
            
            // Close on Escape
            if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
                m_ShowSaveSceneDialog = false;
                ImGui::CloseCurrentPopup();
            }
            
            ImGui::EndPopup();
        } else {
            // If popup is not open, reset the flag
            m_ShowSaveSceneDialog = false;
        }
    }
    
    void SaveScene() {
        if (m_CurrentProjectPath.empty()) {
            LGE::Log::Error("ExampleApp: Cannot save scene - no project loaded");
            return;
        }
        
        std::string sceneName = std::string(m_SceneNameBuffer);
        if (sceneName.empty()) {
            LGE::Log::Error("ExampleApp: Cannot save scene - scene name is empty");
            return;
        }
        
        // Ensure scene name doesn't have extension
        if (sceneName.find(".scene") != std::string::npos) {
            sceneName = sceneName.substr(0, sceneName.find(".scene"));
        }
        
        // Create Scenes directory if it doesn't exist
        std::filesystem::path scenesPath = std::filesystem::path(m_CurrentProjectPath) / "Assets" / "Scenes";
        if (!std::filesystem::exists(scenesPath)) {
            std::filesystem::create_directories(scenesPath);
        }
        
        // Create scene file path
        std::filesystem::path sceneFilePath = scenesPath / (sceneName + ".scene");
        
        // If the scene name is "SampleScene" and it already exists, prompt to rename
        // Otherwise, save the scene
        try {
            // Generate scene content (for now, just save the current scene structure)
            std::ofstream sceneFile(sceneFilePath);
            if (sceneFile.is_open()) {
                sceneFile << "{\n";
                sceneFile << "  \"Name\": \"" << sceneName << "\",\n";
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
                
                // Update current scene path
                m_CurrentScenePath = std::filesystem::absolute(sceneFilePath).string();
                std::replace(m_CurrentScenePath.begin(), m_CurrentScenePath.end(), '\\', '/');
                
                LGE::Log::Info("ExampleApp: Saved scene: " + m_CurrentScenePath);
                
                // Rescan assets if asset manager is available
                if (m_AssetManager) {
                    m_AssetManager->ScanAssets();
                }
            } else {
                LGE::Log::Error("ExampleApp: Failed to create scene file: " + sceneFilePath.string());
            }
        } catch (const std::exception& e) {
            LGE::Log::Error("ExampleApp: Failed to save scene: " + std::string(e.what()));
        }
    }
};

int main() {
    ExampleApp app;

    if (!app.Initialize()) {
        LGE::Log::Error("Failed to initialize application!");
        return -1;
    }

    app.Run();
    app.Shutdown();

    return 0;
}

