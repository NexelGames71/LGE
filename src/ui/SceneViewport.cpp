// Include OpenGL before other headers
#include <glad/glad.h>

#include "LGE/ui/SceneViewport.h"
#include "LGE/rendering/Camera.h"
#include "LGE/rendering/Renderer.h"
#include "LGE/rendering/Texture.h"
#include "LGE/rendering/Shader.h"
#include "LGE/rendering/VertexArray.h"
#include "LGE/rendering/VertexBuffer.h"
#include "LGE/core/FileSystem.h"
#include "LGE/core/Log.h"
#include "LGE/core/Input.h"
#include "LGE/core/GameObject.h"
#include "LGE/ui/UI.h"
#include "LGE/math/Vector.h"
#include "LGE/math/Matrix.h"
#include "imgui.h"
#include "ImGuizmo.h"
#include <functional>
#include <cmath>
#include <algorithm>
#include <vector>

namespace LGE {

SceneViewport::SceneViewport()
    : m_Camera(nullptr)
    , m_Width(1280)
    , m_Height(720)
    , m_Focused(false)
    , m_Hovered(false)
    , m_ViewportSizeChanged(false)
    , m_IconsLoaded(false)
    , m_SelectedObject(nullptr)
    , m_SelectedTool(0)
    , m_IsDragging(false)
{
    // Don't create framebuffer here - wait until OpenGL is initialized
    // Framebuffer will be created on first render
    m_StoredViewport[0] = 0;
    m_StoredViewport[1] = 0;
    m_StoredViewport[2] = 1280;
    m_StoredViewport[3] = 720;
}

SceneViewport::~SceneViewport() {
}

void SceneViewport::OnUpdate(float deltaTime) {
    m_ViewportSizeChanged = false;
}

void SceneViewport::BeginRender() {
    // Don't render if viewport is too small
    if (m_Width == 0 || m_Height == 0) {
        return;
    }
    
    // Create framebuffer if it doesn't exist (OpenGL should be initialized by now)
    if (!m_Framebuffer) {
        m_Framebuffer = std::make_unique<Framebuffer>(m_Width, m_Height);
    }
    
    if (!m_Framebuffer) {
        return;
    }
    
    // Store current viewport to restore later
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    m_StoredViewport[0] = viewport[0];
    m_StoredViewport[1] = viewport[1];
    m_StoredViewport[2] = viewport[2];
    m_StoredViewport[3] = viewport[3];
    
    // Render scene to framebuffer
    m_Framebuffer->Bind();
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Scene rendering will happen here (called from application)
}

void SceneViewport::EndRender() {
    if (!m_Framebuffer) {
        return;
    }
    
    // Unbind framebuffer
    m_Framebuffer->Unbind();
    
    // Restore viewport to what it was before
    glViewport(m_StoredViewport[0], m_StoredViewport[1], m_StoredViewport[2], m_StoredViewport[3]);
}

void SceneViewport::LoadIcons() {
    if (m_IconsLoaded) return;
    
    // Try to load Unreal Engine editor icons (EditorViewport transform tools)
    std::vector<std::string> translatePaths = {
        "assets/icons/EditorViewport.TranslateMode.png",
        "assets/icons/EditorViewport.TranslateMode.Small.png",
        "assets/icons/icon_translate_40x.png",
        "assets/icons/icon_translate_24x.png",
        "assets/icons/icon_translate_16x.png",
        "assets/icons/translate.png"
    };
    
    std::vector<std::string> rotatePaths = {
        "assets/icons/EditorViewport.RotateMode.png",
        "assets/icons/EditorViewport.RotateMode.Small.png",
        "assets/icons/icon_rotate_40x.png",
        "assets/icons/icon_rotate_24x.png",
        "assets/icons/icon_rotate_16x.png",
        "assets/icons/rotate.png"
    };
    
    std::vector<std::string> scalePaths = {
        "assets/icons/EditorViewport.ScaleMode.png",
        "assets/icons/EditorViewport.ScaleMode.Small.png",
        "assets/icons/icon_scale_40x.png",
        "assets/icons/icon_scale_24x.png",
        "assets/icons/icon_scale_16x.png",
        "assets/icons/scale.png"
    };
    
    // Load translate icon
    m_TranslateIcon = std::make_shared<Texture>();
    bool translateLoaded = false;
    for (const auto& path : translatePaths) {
        if (m_TranslateIcon->LoadImageFile(path)) {
            translateLoaded = true;
            break;
        }
    }
    if (!translateLoaded) {
        m_TranslateIcon.reset(); // Failed to load, will use fallback
    }
    
    // Load rotate icon
    m_RotateIcon = std::make_shared<Texture>();
    bool rotateLoaded = false;
    for (const auto& path : rotatePaths) {
        if (m_RotateIcon->LoadImageFile(path)) {
            rotateLoaded = true;
            break;
        }
    }
    if (!rotateLoaded) {
        m_RotateIcon.reset(); // Failed to load, will use fallback
    }
    
    // Load scale icon
    m_ScaleIcon = std::make_shared<Texture>();
    bool scaleLoaded = false;
    for (const auto& path : scalePaths) {
        if (m_ScaleIcon->LoadImageFile(path)) {
            scaleLoaded = true;
            break;
        }
    }
    if (!scaleLoaded) {
        m_ScaleIcon.reset(); // Failed to load, will use fallback
    }
    
    m_IconsLoaded = true;
}

void SceneViewport::OnUIRender() {
    // Modern viewport styling
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.10f, 0.10f, 0.12f, 1.0f));
    ImGui::Begin("Scene Viewport", nullptr);

    m_Focused = ImGui::IsWindowFocused();
    m_Hovered = ImGui::IsWindowHovered();

    // Load icons on first render
    if (!m_IconsLoaded) {
        LoadIcons();
    }

    // Modern transform tool icons above viewport
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8.0f, 6.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4.0f, 4.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
    
    // Helper function to draw icon button with texture or fallback
    auto DrawIconButton = [this](const char* label, bool selected, ImVec2 size, std::shared_ptr<Texture> icon, std::function<void(ImVec2, ImVec2)> fallbackDraw) -> bool {
        // Modern button colors with blue accent
        ImGui::PushStyleColor(ImGuiCol_Button, selected ? ImVec4(0.40f, 0.70f, 1.00f, 0.60f) : ImVec4(0.20f, 0.20f, 0.23f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, selected ? ImVec4(0.45f, 0.75f, 1.00f, 0.70f) : ImVec4(0.25f, 0.25f, 0.28f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, selected ? ImVec4(0.35f, 0.65f, 0.95f, 0.80f) : ImVec4(0.30f, 0.30f, 0.33f, 1.0f));
        
        // Use unique label with ## prefix (## makes it invisible but gives unique ID)
        std::string uniqueLabel = std::string("##") + label;
        bool clicked = ImGui::Button(uniqueLabel.c_str(), size);
        
        // Draw icon in the center of the button
        if (ImGui::IsItemVisible()) {
            ImVec2 pos = ImGui::GetItemRectMin();
            ImVec2 rectSize = ImGui::GetItemRectSize();
            ImVec2 center = ImVec2(pos.x + rectSize.x * 0.5f, pos.y + rectSize.y * 0.5f);
            
            if (icon && icon->GetRendererID() != 0) {
                // Use texture icon
                float iconSize = std::min(rectSize.x, rectSize.y) * 0.7f;
                ImVec2 iconMin = ImVec2(center.x - iconSize * 0.5f, center.y - iconSize * 0.5f);
                ImVec2 iconMax = ImVec2(center.x + iconSize * 0.5f, center.y + iconSize * 0.5f);
                ImGui::GetWindowDrawList()->AddImage(
                    reinterpret_cast<void*>(static_cast<intptr_t>(icon->GetRendererID())),
                    iconMin, iconMax,
                    ImVec2(0, 1), ImVec2(1, 0)); // Flip vertically for OpenGL
            } else {
                // Use fallback drawn icon
                ImDrawList* drawList = ImGui::GetWindowDrawList();
                fallbackDraw(center, rectSize);
            }
        }
        
        ImGui::PopStyleColor(3);
        return clicked;
    };
    
    // Translate icon
    if (DrawIconButton("Translate", m_SelectedTool == 0, ImVec2(36.0f, 32.0f), m_TranslateIcon, [](ImVec2 center, ImVec2 size) {
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        ImU32 color = IM_COL32(255, 255, 255, 255);
        float radius = 6.0f;
        drawList->AddLine(ImVec2(center.x + radius, center.y), ImVec2(center.x - radius, center.y), color, 1.5f);
        drawList->AddLine(ImVec2(center.x + radius * 0.5f, center.y - radius * 0.5f), ImVec2(center.x + radius, center.y), color, 1.5f);
        drawList->AddLine(ImVec2(center.x + radius * 0.5f, center.y + radius * 0.5f), ImVec2(center.x + radius, center.y), color, 1.5f);
        drawList->AddLine(ImVec2(center.x, center.y - radius), ImVec2(center.x, center.y + radius), color, 1.5f);
        drawList->AddLine(ImVec2(center.x - radius * 0.5f, center.y - radius * 0.5f), ImVec2(center.x, center.y - radius), color, 1.5f);
        drawList->AddLine(ImVec2(center.x + radius * 0.5f, center.y - radius * 0.5f), ImVec2(center.x, center.y - radius), color, 1.5f);
        drawList->AddCircleFilled(center, 2.0f, color);
    })) {
        m_SelectedTool = 0;
    }
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Translate (W)");
    }
    
    ImGui::SameLine();
    
    // Rotate icon
    if (DrawIconButton("Rotate", m_SelectedTool == 1, ImVec2(36.0f, 32.0f), m_RotateIcon, [](ImVec2 center, ImVec2 size) {
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        ImU32 color = IM_COL32(255, 255, 255, 255);
        float radius = 7.0f;
        const int num_segments = 20;
        for (int i = 0; i < num_segments * 3 / 4; i++) {
            float angle1 = (i / (float)num_segments) * 2.0f * 3.14159f - 3.14159f * 0.5f;
            float angle2 = ((i + 1) / (float)num_segments) * 2.0f * 3.14159f - 3.14159f * 0.5f;
            ImVec2 p1 = ImVec2(center.x + std::cos(angle1) * radius, center.y + std::sin(angle1) * radius);
            ImVec2 p2 = ImVec2(center.x + std::cos(angle2) * radius, center.y + std::sin(angle2) * radius);
            drawList->AddLine(p1, p2, color, 1.5f);
        }
        float arrowAngle = (3.0f / 4.0f) * 2.0f * 3.14159f - 3.14159f * 0.5f;
        ImVec2 arrowTip = ImVec2(center.x + std::cos(arrowAngle) * radius, center.y + std::sin(arrowAngle) * radius);
        ImVec2 arrowLeft = ImVec2(arrowTip.x - std::cos(arrowAngle + 0.5f) * 3.0f, arrowTip.y - std::sin(arrowAngle + 0.5f) * 3.0f);
        ImVec2 arrowRight = ImVec2(arrowTip.x - std::cos(arrowAngle - 0.5f) * 3.0f, arrowTip.y - std::sin(arrowAngle - 0.5f) * 3.0f);
        drawList->AddTriangleFilled(arrowTip, arrowLeft, arrowRight, color);
    })) {
        m_SelectedTool = 1;
    }
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Rotate (E)");
    }
    
    ImGui::SameLine();
    
    // Scale icon
    if (DrawIconButton("Scale", m_SelectedTool == 2, ImVec2(36.0f, 32.0f), m_ScaleIcon, [](ImVec2 center, ImVec2 size) {
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        ImU32 color = IM_COL32(255, 255, 255, 255);
        float boxSize = 8.0f;
        ImVec2 boxMin = ImVec2(center.x - boxSize * 0.5f, center.y - boxSize * 0.5f);
        ImVec2 boxMax = ImVec2(center.x + boxSize * 0.5f, center.y + boxSize * 0.5f);
        drawList->AddRect(boxMin, boxMax, color, 0.0f, 0, 1.5f);
        float handleSize = 3.0f;
        drawList->AddRectFilled(ImVec2(boxMin.x - handleSize * 0.5f, boxMin.y - handleSize * 0.5f), ImVec2(boxMin.x + handleSize * 0.5f, boxMin.y + handleSize * 0.5f), color);
        drawList->AddRectFilled(ImVec2(boxMax.x - handleSize * 0.5f, boxMin.y - handleSize * 0.5f), ImVec2(boxMax.x + handleSize * 0.5f, boxMin.y + handleSize * 0.5f), color);
        drawList->AddRectFilled(ImVec2(boxMin.x - handleSize * 0.5f, boxMax.y - handleSize * 0.5f), ImVec2(boxMin.x + handleSize * 0.5f, boxMax.y + handleSize * 0.5f), color);
        drawList->AddRectFilled(ImVec2(boxMax.x - handleSize * 0.5f, boxMax.y - handleSize * 0.5f), ImVec2(boxMax.x + handleSize * 0.5f, boxMax.y + handleSize * 0.5f), color);
    })) {
        m_SelectedTool = 2;
    }
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("Scale (R)");
    }
    
    ImGui::PopStyleVar(3);
    
    ImGui::Separator();

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    
    // Ensure minimum size
    if (viewportPanelSize.x < 1.0f) viewportPanelSize.x = 1.0f;
    if (viewportPanelSize.y < 1.0f) viewportPanelSize.y = 1.0f;
    
    if (viewportPanelSize.x != m_Width || viewportPanelSize.y != m_Height) {
        m_Width = static_cast<uint32_t>(viewportPanelSize.x);
        m_Height = static_cast<uint32_t>(viewportPanelSize.y);
        m_ViewportSizeChanged = true;
        
        if (m_Width > 0 && m_Height > 0) {
            if (m_Framebuffer) {
                m_Framebuffer->Resize(m_Width, m_Height);
            }
            
            // Update camera aspect ratio
            if (m_Camera) {
                float aspectRatio = static_cast<float>(m_Width) / static_cast<float>(m_Height);
                m_Camera->SetPerspective(45.0f, aspectRatio, 0.1f, 100.0f);
            }
        }
    }

    // Display the framebuffer texture
    if (m_Framebuffer && m_Width > 0 && m_Height > 0) {
        uint64_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
        ImGui::Image(reinterpret_cast<void*>(textureID), viewportPanelSize, ImVec2(0, 1), ImVec2(1, 0));
        
        // Set up ImGuizmo
        ImGuizmo::SetDrawlist();
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, 
                         ImGui::GetWindowWidth(), ImGui::GetWindowHeight());
        
        // Render ImGuizmo if object is selected
        if (m_SelectedObject && m_Camera) {
            RenderImGuizmo();
        }
        
        // Handle mouse input for selection (only when viewport is focused and hovered)
        if (m_Focused && m_Hovered && !UI::WantCaptureMouse() && !ImGuizmo::IsOver()) {
            HandleMouseInput();
        }
    }

    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
}

void SceneViewport::HandleMouseInput() {
    if (!m_Camera) return;
    
    // Don't process mouse input if middle mouse is pressed (camera panning)
    // Middle mouse is handled by CameraController
    if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_MIDDLE)) {
        m_IsDragging = false;
        return;
    }
    
    // Get mouse position in viewport
    ImVec2 mousePos = ImGui::GetMousePos();
    ImVec2 viewportMin = ImGui::GetItemRectMin();
    ImVec2 viewportMax = ImGui::GetItemRectMax();
    
    // Check if mouse is within viewport
    if (mousePos.x < viewportMin.x || mousePos.x > viewportMax.x ||
        mousePos.y < viewportMin.y || mousePos.y > viewportMax.y) {
        return;
    }
    
    // Convert to viewport coordinates (0,0 at top-left)
    float viewportX = mousePos.x - viewportMin.x;
    float viewportY = mousePos.y - viewportMin.y;
    
    // Convert to normalized device coordinates (-1 to 1, with Y flipped)
    float ndcX = (viewportX / m_Width) * 2.0f - 1.0f;
    float ndcY = 1.0f - (viewportY / m_Height) * 2.0f;
    
    bool leftMousePressed = Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT);
    static bool leftMouseWasPressed = false;
    
    // Handle left mouse button for selection
    if (leftMousePressed && !leftMouseWasPressed) {
        // Mouse just clicked - perform object selection
        // Simple selection: check if clicking near any GameObject
        // For now, we'll use a simple screen-space check
        // TODO: Implement proper raycasting for accurate object selection
        
        bool objectSelected = false;
        
        // Check all GameObjects to see if we clicked on one
        for (auto& obj : m_GameObjects) {
            if (!obj) continue;
            
            // Get object position in world space
            Math::Vector3 objPos = obj->GetPosition();
            
            // Project object position to screen space
            // This is a simplified check - proper implementation would use raycasting
            Math::Vector3 viewPos = m_Camera->GetPosition();
            Math::Vector3 toObject = Math::Vector3(
                objPos.x - viewPos.x,
                objPos.y - viewPos.y,
                objPos.z - viewPos.z
            );
            
            // Simple distance check - if object is roughly in front of camera
            float distance = std::sqrt(toObject.x * toObject.x + toObject.y * toObject.y + toObject.z * toObject.z);
            
            // If clicking anywhere on screen and object is in front of camera, select it
            // This is a simplified selection - proper implementation would use proper projection/raycasting
            // For now, if object is within reasonable distance, select it when clicking anywhere
            if (distance < 20.0f) {
                // Select this object (first one found that's close enough)
                m_SelectedObject = obj.get();
                obj->SetSelected(true);
                
                // Deselect others
                for (auto& other : m_GameObjects) {
                    if (other.get() != obj.get()) {
                        other->SetSelected(false);
                    }
                }
                
                objectSelected = true;
                Log::Info("Selected object: " + obj->GetName() + " at click position: " + 
                         std::to_string(ndcX) + ", " + std::to_string(ndcY) + " distance: " + std::to_string(distance));
                break;
            }
        }
        
        // If we selected an object, prepare for dragging
        if (m_SelectedObject) {
            m_DragStartPos = Math::Vector3(ndcX, ndcY, 0.0f);
            m_DragStartObjectPos = m_SelectedObject->GetPosition();
            m_DragStartObjectRot = m_SelectedObject->GetRotation();
            m_DragStartObjectScale = m_SelectedObject->GetScale();
            m_IsDragging = true;
        } else if (!objectSelected) {
            // Clicked on empty space - deselect
            m_SelectedObject = nullptr;
            for (auto& obj : m_GameObjects) {
                if (obj) {
                    obj->SetSelected(false);
                }
            }
        }
    } else if (leftMousePressed && leftMouseWasPressed && m_IsDragging && m_SelectedObject) {
        // Mouse is being dragged - perform transform via gizmo
        // TODO: Implement proper gizmo-based transformation with axis constraints
        Math::Vector3 currentMousePos(ndcX, ndcY, 0.0f);
        Math::Vector3 delta = Math::Vector3(
            currentMousePos.x - m_DragStartPos.x,
            currentMousePos.y - m_DragStartPos.y,
            0.0f
        );
        
        // Transform based on selected tool
        if (m_SelectedTool == 0) { // Translate
            // Calculate movement in world space
            float moveSpeed = 2.0f;
            Math::Vector3 worldDelta = Math::Vector3(
                delta.x * moveSpeed,
                -delta.y * moveSpeed, // Invert Y
                0.0f
            );
            m_SelectedObject->SetPosition(Math::Vector3(
                m_DragStartObjectPos.x + worldDelta.x,
                m_DragStartObjectPos.y + worldDelta.y,
                m_DragStartObjectPos.z + worldDelta.z
            ));
        } else if (m_SelectedTool == 1) { // Rotate
            float rotateSpeed = 90.0f; // degrees per unit of mouse movement
            m_SelectedObject->SetRotation(Math::Vector3(
                m_DragStartObjectRot.x,
                m_DragStartObjectRot.y + delta.x * rotateSpeed,
                m_DragStartObjectRot.z
            ));
        } else if (m_SelectedTool == 2) { // Scale
            float scaleSpeed = 1.0f;
            float scaleDelta = (delta.x + delta.y) * scaleSpeed;
            Math::Vector3 newScale = Math::Vector3(
                m_DragStartObjectScale.x + scaleDelta,
                m_DragStartObjectScale.y + scaleDelta,
                m_DragStartObjectScale.z + scaleDelta
            );
            // Clamp scale to prevent negative values
            newScale.x = std::max(0.1f, newScale.x);
            newScale.y = std::max(0.1f, newScale.y);
            newScale.z = std::max(0.1f, newScale.z);
            m_SelectedObject->SetScale(newScale);
        }
    } else if (!leftMousePressed && leftMouseWasPressed) {
        // Mouse released
        m_IsDragging = false;
    }
    
    leftMouseWasPressed = leftMousePressed;
}

void SceneViewport::RenderImGuizmo() {
    if (!m_SelectedObject || !m_Camera) {
        return;
    }
    
    // Get camera matrices
    const Math::Matrix4& viewMatrix = m_Camera->GetViewMatrix();
    const Math::Matrix4& projectionMatrix = m_Camera->GetProjectionMatrix();
    
    // Convert matrices to float arrays (column-major for OpenGL)
    float view[16];
    float projection[16];
    float objectMatrix[16];
    
    // Copy view matrix
    const float* viewData = viewMatrix.GetData();
    for (int i = 0; i < 16; i++) {
        view[i] = viewData[i];
    }
    
    // Copy projection matrix
    const float* projData = projectionMatrix.GetData();
    for (int i = 0; i < 16; i++) {
        projection[i] = projData[i];
    }
    
    // Get object transform matrix
    const Math::Matrix4& transformMatrix = m_SelectedObject->GetTransformMatrix();
    const float* transformData = transformMatrix.GetData();
    for (int i = 0; i < 16; i++) {
        objectMatrix[i] = transformData[i];
    }
    
    // Determine operation based on selected tool
    ImGuizmo::OPERATION operation = ImGuizmo::TRANSLATE;
    if (m_SelectedTool == 1) {
        operation = ImGuizmo::ROTATE;
    } else if (m_SelectedTool == 2) {
        operation = ImGuizmo::SCALE;
    }
    
    // Use local mode
    ImGuizmo::MODE mode = ImGuizmo::LOCAL;
    
    // Manipulate the object
    bool manipulated = ImGuizmo::Manipulate(view, projection, operation, mode, objectMatrix);
    
    // If the gizmo was manipulated, update the GameObject
    if (manipulated) {
        // Decompose the matrix to get position, rotation, and scale
        Math::Vector3 translation, rotation, scale;
        
        // Extract translation
        translation.x = objectMatrix[12];
        translation.y = objectMatrix[13];
        translation.z = objectMatrix[14];
        
        // Extract scale (length of each column)
        scale.x = std::sqrt(objectMatrix[0] * objectMatrix[0] + objectMatrix[1] * objectMatrix[1] + objectMatrix[2] * objectMatrix[2]);
        scale.y = std::sqrt(objectMatrix[4] * objectMatrix[4] + objectMatrix[5] * objectMatrix[5] + objectMatrix[6] * objectMatrix[6]);
        scale.z = std::sqrt(objectMatrix[8] * objectMatrix[8] + objectMatrix[9] * objectMatrix[9] + objectMatrix[10] * objectMatrix[10]);
        
        // Update position and scale
        m_SelectedObject->SetPosition(translation);
        m_SelectedObject->SetScale(scale);
        
        // For rotation, extract Euler angles from the rotation matrix
        if (operation == ImGuizmo::ROTATE || operation == ImGuizmo::TRANSLATE) {
            // Extract rotation from the upper-left 3x3 matrix
            float sy = std::sqrt(objectMatrix[0] * objectMatrix[0] + objectMatrix[4] * objectMatrix[4]);
            bool singular = sy < 1e-6;
            
            if (!singular) {
                rotation.x = std::atan2(objectMatrix[9], objectMatrix[10]) * 180.0f / 3.14159f;
                rotation.y = std::atan2(-objectMatrix[8], sy) * 180.0f / 3.14159f;
                rotation.z = std::atan2(objectMatrix[4], objectMatrix[0]) * 180.0f / 3.14159f;
            } else {
                rotation.x = std::atan2(-objectMatrix[6], objectMatrix[5]) * 180.0f / 3.14159f;
                rotation.y = std::atan2(-objectMatrix[8], sy) * 180.0f / 3.14159f;
                rotation.z = 0.0f;
            }
            
            m_SelectedObject->SetRotation(rotation);
        }
    }
}

Math::Vector3 SceneViewport::ScreenToWorldRay(float screenX, float screenY) {
    // Convert screen coordinates to world ray
    // This is a placeholder - full implementation would unproject screen coordinates
    return Math::Vector3(0.0f, 0.0f, 0.0f);
}

} // namespace LGE

