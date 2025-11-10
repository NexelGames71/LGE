#pragma once

#include <string>
#include <memory>
#include <vector>

namespace LGE {

class Texture;

// Splash/Loading Screen - shown after project is created/opened
class SplashScreen {
public:
    SplashScreen();
    ~SplashScreen();

    void OnUIRender();
    
    bool IsOpen() const { return m_IsOpen; }
    void SetOpen(bool open) { m_IsOpen = open; }
    
    // Start loading process
    void StartLoading();
    
    // Check if loading is complete
    bool IsLoadingComplete() const { return m_LoadingComplete; }
    
    // Get current loading stage
    std::string GetCurrentStage() const { return m_CurrentStage; }
    float GetLoadingProgress() const { return m_LoadingProgress; }

private:
    void LoadSplashImage();
    void UpdateLoading(float deltaTime);
    
    bool m_IsOpen;
    bool m_LoadingComplete;
    float m_LoadingProgress; // 0.0 to 1.0
    float m_LoadingTime;
    std::string m_CurrentStage;
    
    // Splash image
    std::shared_ptr<Texture> m_SplashImage;
    bool m_SplashImageLoaded;
    
    // Loading stages
    struct LoadingStage {
        std::string name;
        float duration; // Time in seconds
    };
    std::vector<LoadingStage> m_LoadingStages;
    size_t m_CurrentStageIndex;
};

} // namespace LGE
