#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "LGE/core/Application.h"
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
#include "LGE/core/Input.h"
#include "LGE/core/GameObject.h"
#include "LGE/core/MaterialComponent.h"
#include "imgui.h"
#include "imgui_internal.h"
#include <memory>
#include <cstdint>
#include <cmath>

class ExampleApp : public LGE::Application {
public:
    ExampleApp() : LGE::Application("LGE Game Engine Example") {}

    bool Initialize() override {
        if (!LGE::Application::Initialize()) {
            return false;
        }

        // Setup materials
        m_GridMaterial = LGE::Material::CreateDefaultGridMaterial();
        m_LitMaterial = LGE::Material::CreateDefaultLitMaterial();
        
        // Also create a PBR material for testing
        auto pbrMaterial = LGE::Material::CreateDefaultPBRMaterial();
        
        if (!m_GridMaterial || !m_LitMaterial) {
            LGE::Log::Error("Failed to create default materials!");
            return false;
        }
        
        // Use PBR material for MaterialComponent if available
        if (pbrMaterial) {
            m_LitMaterial = pbrMaterial; // Use PBR material for testing
        }
        
        // Keep shader reference for backward compatibility
        m_Shader = m_LitMaterial->GetShader();

        // Cube vertices with colors and normals
        // Format: x, y, z, r, g, b, nx, ny, nz
        // Each face has 2 triangles (6 vertices)
        float vertices[] = {
            // Front face (Red) - normal: 0, 0, 1
            -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,  // Bottom left
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,  // Bottom right
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,  // Top right
            -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,  // Bottom left
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,  // Top right
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,  // Top left
            
            // Back face (Green) - normal: 0, 0, -1
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, -1.0f,  // Bottom left
             0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, -1.0f,  // Top right
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, -1.0f,  // Bottom right
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, -1.0f,  // Bottom left
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, -1.0f,  // Top left
             0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, -1.0f,  // Top right
            
            // Left face (Blue) - normal: -1, 0, 0
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,  // Bottom back
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,  // Bottom front
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,  // Top front
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,  // Bottom back
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,  // Top front
            -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,  // Top back
            
            // Right face (Yellow) - normal: 1, 0, 0
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  // Bottom back
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  // Top back
             0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  // Bottom front
             0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  // Bottom front
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  // Top back
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  // Top front
            
            // Top face (Cyan) - normal: 0, 1, 0
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,  // Back left
             0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,  // Front right
             0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,  // Back right
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,  // Back left
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,  // Front left
             0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,  // Front right
            
            // Bottom face (Magenta) - normal: 0, -1, 0
            -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,  0.0f, -1.0f, 0.0f,  // Back left
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,  0.0f, -1.0f, 0.0f,  // Back right
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f, -1.0f, 0.0f,  // Front right
            -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.0f,  0.0f, -1.0f, 0.0f,  // Back left
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f, -1.0f, 0.0f,  // Front right
            -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f, -1.0f, 0.0f   // Front left
        };

        // Create vertex buffer
        m_VertexBuffer = std::make_unique<LGE::VertexBuffer>(vertices, static_cast<uint32_t>(sizeof(vertices)));

        // Create vertex array and set up attributes manually (position + color + normal)
        m_VertexArray = std::make_unique<LGE::VertexArray>();
        m_VertexArray->Bind();
        m_VertexBuffer->Bind();
        
        // Set up vertex attributes: position (0), color (1), normal (2)
        // Stride: 9 floats (3 pos + 3 color + 3 normal)
        // Position
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
        // Color
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
        // Normal
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
        
        m_VertexArray->Unbind();
        m_VertexBuffer->Unbind();

        // Create GameObject for the cube
        m_CubeObject = std::make_shared<LGE::GameObject>("Cube");
        m_CubeObject->SetPosition(LGE::Math::Vector3(0.0f, 0.0f, 0.0f));
        m_CubeObject->SetRotation(LGE::Math::Vector3(0.0f, 0.0f, 0.0f));
        m_CubeObject->SetScale(LGE::Math::Vector3(1.0f, 1.0f, 1.0f));
        m_CubeObject->SetSelected(true);
        
        // Create and attach MaterialComponent
        auto materialComp = std::make_shared<LGE::MaterialComponent>();
        materialComp->SetMaterial(m_LitMaterial);
        m_CubeObject->AddComponent(materialComp);
        
        // Add to GameObjects list
        m_GameObjects.push_back(m_CubeObject);

        // Setup UI panels
        m_MainMenuBar = std::make_unique<LGE::MainMenuBar>();
        m_Toolbar = std::make_unique<LGE::Toolbar>();
        m_Hierarchy = std::make_unique<LGE::Hierarchy>();
        m_Inspector = std::make_unique<LGE::Inspector>();
        m_ContentBrowser = std::make_unique<LGE::ContentBrowser>();
        m_SceneViewport = std::make_unique<LGE::SceneViewport>();
        m_Details = std::make_unique<LGE::Details>();
        
        // Setup camera
        float aspectRatio = static_cast<float>(m_Window->GetWidth()) / static_cast<float>(m_Window->GetHeight());
        m_Camera = std::make_unique<LGE::Camera>();
        m_Camera->SetPerspective(45.0f, aspectRatio, 0.1f, 100.0f);
        m_Camera->SetPosition(LGE::Math::Vector3(0.0f, 0.0f, 3.0f));
        m_Camera->SetTarget(LGE::Math::Vector3(0.0f, 0.0f, 0.0f));
        
        // Set camera to viewport
        m_SceneViewport->SetCamera(m_Camera.get());
        
        // Set cube as selected object in viewport and panels
        if (m_CubeObject) {
            m_SceneViewport->SetGameObjects(m_GameObjects);
            m_SceneViewport->SetSelectedObject(m_CubeObject.get());
            m_Hierarchy->SetGameObjects(m_GameObjects);
            m_Hierarchy->SetSelectedObject(m_CubeObject.get());
            m_Inspector->SetSelectedObject(m_CubeObject.get());
        }
        
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

    void OnUpdate(float deltaTime) override {
        // Update animation time
        m_Time += deltaTime;
        
        // Update camera controller
        if (m_CameraController) {
            m_CameraController->OnUpdate(deltaTime);
        }
    }

    void OnRender() override {
        // Render main menu bar
        if (m_MainMenuBar) {
            m_MainMenuBar->OnUIRender();
        }
        
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
        
        if (m_Hierarchy) {
            m_Hierarchy->OnUIRender();
            // Sync selection from Hierarchy to other panels
            if (m_Hierarchy->GetSelectedObject() != m_SceneViewport->GetSelectedObject()) {
                m_SceneViewport->SetSelectedObject(m_Hierarchy->GetSelectedObject());
                m_Inspector->SetSelectedObject(m_Hierarchy->GetSelectedObject());
            }
        }
        
        if (m_SceneViewport) {
            m_SceneViewport->OnUIRender();
            // Sync selection from SceneViewport to other panels
            if (m_SceneViewport->GetSelectedObject() != m_Hierarchy->GetSelectedObject()) {
                m_Hierarchy->SetSelectedObject(m_SceneViewport->GetSelectedObject());
                m_Inspector->SetSelectedObject(m_SceneViewport->GetSelectedObject());
            }
        }
        
        if (m_Inspector) {
            m_Inspector->OnUIRender();
        }
        
        if (m_ContentBrowser) {
            m_ContentBrowser->OnUIRender();
        }
        
        ImGui::End();
        
        // Render toolbar after dock space (so it appears on top, fixed below menu bar)
        if (m_Toolbar) {
            m_Toolbar->OnUIRender();
        }
        
        // Render scene to framebuffer (everything should render here)
        if (m_SceneViewport && m_SceneViewport->GetWidth() > 0 && m_SceneViewport->GetHeight() > 0) {
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
            
            // Render the cube with material from MaterialComponent
            auto materialComp = m_CubeObject ? m_CubeObject->GetComponent<LGE::MaterialComponent>() : nullptr;
            auto material = materialComp ? materialComp->GetMaterial() : m_LitMaterial;
            
            if (material && material->GetShader()) {
                // Bind material (from component or fallback)
                if (materialComp) {
                    materialComp->Bind();
                } else {
                    material->Bind();
                }
                auto shader = material->GetShader();
                
                // Set view-projection matrix
                shader->SetUniformMat4("u_ViewProjection", m_Camera->GetViewProjectionMatrix().GetData());
                
                // Set view matrix (needed for PBR shader)
                shader->SetUniformMat4("u_View", m_Camera->GetViewMatrix().GetData());
                
                // Set model matrix from GameObject transform
                // Always use the cube's GameObject transform (cube is a test object)
                if (m_CubeObject) {
                    const LGE::Math::Matrix4& modelMatrix = m_CubeObject->GetTransformMatrix();
                    shader->SetUniformMat4("u_Model", modelMatrix.GetData());
                } else {
                    // Fallback: use identity matrix
                    LGE::Math::Matrix4 modelMatrix;
                    shader->SetUniformMat4("u_Model", modelMatrix.GetData());
                }
                
                // Set light uniforms with dynamic variation
                if (m_DirectionalLight) {
                    LGE::Math::Vector3 baseLightDir = m_DirectionalLight->GetDirection();
                    
                    // Slowly rotate light direction for dynamic lighting
                    float lightRotationSpeed = 0.2f; // Slower than cube
                    float lightAngle = m_Time * lightRotationSpeed;
                    LGE::Math::Matrix4 lightRot = LGE::Math::Matrix4::Rotate(lightAngle, LGE::Math::Vector3(0.0f, 1.0f, 0.0f));
                    LGE::Math::Vector4 rotatedDir = lightRot * LGE::Math::Vector4(baseLightDir.x, baseLightDir.y, baseLightDir.z, 0.0f);
                    LGE::Math::Vector3 lightDir(rotatedDir.x, rotatedDir.y, rotatedDir.z);
                    
                    // Subtle intensity variation (simulating time of day)
                    float intensityVariation = 0.15f * std::sin(m_Time * 0.3f) + 1.0f; // Oscillates between 0.85 and 1.15
                    float dynamicIntensity = m_DirectionalLight->GetIntensity() * intensityVariation;
                    
                    LGE::Math::Vector3 lightColor = m_DirectionalLight->GetColor();
                    shader->SetUniform3f("u_LightDirection", lightDir.x, lightDir.y, lightDir.z);
                    shader->SetUniform3f("u_LightColor", lightColor.x, lightColor.y, lightColor.z);
                    shader->SetUniform1f("u_LightIntensity", dynamicIntensity);
                }
                
                // Set view position for specular lighting
                LGE::Math::Vector3 viewPos = m_Camera->GetPosition();
                shader->SetUniform3f("u_ViewPos", viewPos.x, viewPos.y, viewPos.z);
                
                // Material properties are already set by material->Bind()
                // But we need to set the use vertex color flag
                shader->SetUniform1i("u_UseVertexColor", 0); // Use material color, not vertex color
                
                m_VertexArray->Bind();
                // Draw cube: 6 faces * 2 triangles * 3 vertices = 36 vertices
                glDrawArrays(GL_TRIANGLES, 0, 36);
                m_VertexArray->Unbind();
                if (materialComp) {
                    materialComp->GetMaterial()->Unbind();
                } else {
                    material->Unbind();
                }
            }
            
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
        m_GridMaterial.reset();
        m_LitMaterial.reset();
        m_Shader.reset();
        m_VertexBuffer.reset();
        m_VertexArray.reset();
        m_Skybox.reset();
        m_CameraController.reset();
        m_Camera.reset();
        LGE::Application::Shutdown();
    }

private:
    std::shared_ptr<LGE::Shader> m_Shader; // Kept for backward compatibility
    std::shared_ptr<LGE::Material> m_GridMaterial;
    std::shared_ptr<LGE::Material> m_LitMaterial;
    std::unique_ptr<LGE::VertexBuffer> m_VertexBuffer;
    std::unique_ptr<LGE::VertexArray> m_VertexArray;
    std::shared_ptr<LGE::GameObject> m_CubeObject; // GameObject for the cube
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
    std::unique_ptr<LGE::Toolbar> m_Toolbar;
    std::unique_ptr<LGE::MainMenuBar> m_MainMenuBar;
    
    // Animation time
    float m_Time = 0.0f;
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

