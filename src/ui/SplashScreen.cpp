#include "LGE/ui/SplashScreen.h"
#include "LGE/rendering/Texture.h"
#include "LGE/core/Log.h"
#include "imgui.h"
#include <algorithm>
#include <vector>
#include <string>

namespace LGE {

SplashScreen::SplashScreen()
    : m_IsOpen(false)
    , m_LoadingComplete(false)
    , m_LoadingProgress(0.0f)
    , m_LoadingTime(0.0f)
    , m_CurrentStage("")
    , m_SplashImageLoaded(false)
    , m_CurrentStageIndex(0)
{
    // Define loading stages
    m_LoadingStages = {
        {"Compiling shaders...", 1.5f},
        {"Loading game assets...", 2.0f},
        {"Initializing renderer...", 1.0f},
        {"Loading scene...", 1.5f},
        {"Ready!", 0.5f}
    };
    
    LoadSplashImage();
}

SplashScreen::~SplashScreen() {
}

void SplashScreen::LoadSplashImage() {
    if (m_SplashImageLoaded) return;
    
    m_SplashImage = std::make_shared<Texture>();
    std::vector<std::string> splashPaths = {
        "assets/splash/PayRoll Map.png",
        "assets/splash/PayRoll Map (1).png",
        "assets/splash/splash.png"
    };
    
    for (const auto& path : splashPaths) {
        if (m_SplashImage->LoadImageFile(path)) {
            m_SplashImageLoaded = true;
            Log::Info("SplashScreen: Loaded splash image: " + path);
            break;
        }
    }
    
    if (!m_SplashImageLoaded) {
        Log::Warn("SplashScreen: Failed to load splash image, using default background");
    }
}

void SplashScreen::StartLoading() {
    m_IsOpen = true;
    m_LoadingComplete = false;
    m_LoadingProgress = 0.0f;
    m_LoadingTime = 0.0f;
    m_CurrentStageIndex = 0;
    m_CurrentStage = m_LoadingStages.empty() ? "Loading..." : m_LoadingStages[0].name;
}

void SplashScreen::UpdateLoading(float deltaTime) {
    if (m_LoadingComplete || m_LoadingStages.empty()) {
        return;
    }
    
    m_LoadingTime += deltaTime;
    
    // Calculate progress through current stage
    float currentStageDuration = m_LoadingStages[m_CurrentStageIndex].duration;
    float stageProgress = std::min(m_LoadingTime / currentStageDuration, 1.0f);
    
    // Calculate overall progress
    float totalDuration = 0.0f;
    for (const auto& stage : m_LoadingStages) {
        totalDuration += stage.duration;
    }
    
    float elapsedTime = 0.0f;
    for (size_t i = 0; i < m_CurrentStageIndex; i++) {
        elapsedTime += m_LoadingStages[i].duration;
    }
    elapsedTime += m_LoadingTime;
    
    m_LoadingProgress = std::min(elapsedTime / totalDuration, 1.0f);
    
    // Move to next stage if current stage is complete
    if (m_LoadingTime >= currentStageDuration) {
        m_LoadingTime = 0.0f;
        m_CurrentStageIndex++;
        
        if (m_CurrentStageIndex >= m_LoadingStages.size()) {
            m_LoadingComplete = true;
            m_CurrentStage = "Ready!";
            m_LoadingProgress = 1.0f;
        } else {
            m_CurrentStage = m_LoadingStages[m_CurrentStageIndex].name;
        }
    }
}

void SplashScreen::OnUIRender() {
    if (!m_IsOpen) return;
    
    // Update loading progress
    static float lastTime = 0.0f;
    float currentTime = ImGui::GetTime();
    float deltaTime = currentTime - lastTime;
    lastTime = currentTime;
    UpdateLoading(deltaTime);
    
    // Full screen overlay
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                             ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse |
                             ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking |
                             ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
                             ImGuiWindowFlags_NoBackground;
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1.0f)); // Black background
    
    if (ImGui::Begin("##SplashScreen", nullptr, flags)) {
        // Draw splash image if available
        if (m_SplashImage && m_SplashImage->GetRendererID() != 0) {
            ImVec2 windowSize = ImGui::GetWindowSize();
            ImVec2 imageSize(windowSize.x, windowSize.y);
            
            // Center the image
            ImVec2 imagePos = ImGui::GetWindowPos();
            ImGui::GetWindowDrawList()->AddImage(
                reinterpret_cast<void*>(static_cast<intptr_t>(m_SplashImage->GetRendererID())),
                imagePos, ImVec2(imagePos.x + imageSize.x, imagePos.y + imageSize.y),
                ImVec2(0, 0), ImVec2(1, 1)
            );
        }
        
        // Loading text and spinner (no background panel)
        ImVec2 windowSize = ImGui::GetWindowSize();
        
        // Loading text (centered at bottom)
        float textSize = ImGui::GetFontSize() * 1.5f;
        ImVec2 textSizeVec = ImGui::CalcTextSize(m_CurrentStage.c_str());
        float textWidth = textSizeVec.x;
        
        // Center the text horizontally, position near bottom
        ImVec2 textPos(
            (windowSize.x - textWidth) * 0.5f,
            windowSize.y - 100.0f
        );
        
        // Current stage text
        ImGui::GetWindowDrawList()->AddText(
            ImGui::GetFont(), textSize,
            textPos, IM_COL32(255, 255, 255, 255),
            m_CurrentStage.c_str()
        );
        
        // Spinner animation (positioned in bottom right corner)
        float spinnerRadius = 15.0f;
        float spinnerMargin = 30.0f; // Margin from edges
        ImVec2 spinnerCenter(
            windowSize.x - spinnerMargin - spinnerRadius,
            windowSize.y - spinnerMargin - spinnerRadius
        );
        
        // Calculate rotation angle based on time
        float rotationSpeed = 2.0f; // radians per second
        float currentTime = ImGui::GetTime();
        float angle = currentTime * rotationSpeed;
        
        // Draw spinner (rotating circle with segments)
        int numSegments = 12;
        float segmentAngle = 2.0f * 3.14159f / numSegments;
        
        for (int i = 0; i < numSegments; i++) {
            float segmentStartAngle = angle + i * segmentAngle;
            float segmentEndAngle = segmentStartAngle + segmentAngle * 0.7f; // Leave gap between segments
            
            ImVec2 startPoint(
                spinnerCenter.x + std::cos(segmentStartAngle) * spinnerRadius,
                spinnerCenter.y + std::sin(segmentStartAngle) * spinnerRadius
            );
            ImVec2 endPoint(
                spinnerCenter.x + std::cos(segmentEndAngle) * spinnerRadius,
                spinnerCenter.y + std::sin(segmentEndAngle) * spinnerRadius
            );
            
            // Fade opacity based on segment position
            float opacity = 0.3f + 0.7f * (1.0f - (float)i / numSegments);
            ImU32 color = IM_COL32(100, 150, 255, (int)(255 * opacity));
            
            // Draw arc segment
            ImGui::GetWindowDrawList()->AddLine(
                startPoint, endPoint,
                color, 3.0f
            );
        }
        
        // Draw outer ring
        ImGui::GetWindowDrawList()->AddCircle(
            spinnerCenter, spinnerRadius,
            IM_COL32(100, 150, 255, 100), 32, 2.0f
        );
    }
    ImGui::End();
    
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(2);
}

} // namespace LGE
