#pragma once

#include <memory>

namespace LGE {

class Texture;

enum class GamePreviewState {
    Stopped,
    Playing,
    Paused
};

class Toolbar {
public:
    Toolbar();
    ~Toolbar();

    void OnUIRender();
    
    GamePreviewState GetPreviewState() const { return m_PreviewState; }

private:
    void LoadIcons();
    
    std::shared_ptr<Texture> m_PlayIcon;
    std::shared_ptr<Texture> m_StopIcon;
    std::shared_ptr<Texture> m_PauseIcon;
    bool m_IconsLoaded;
    GamePreviewState m_PreviewState;
};

} // namespace LGE


