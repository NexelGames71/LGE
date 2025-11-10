#pragma once

#include <functional>

namespace LGE {

class MainMenuBar {
public:
    MainMenuBar();
    ~MainMenuBar();

    void OnUIRender();
    
    // Callbacks
    void SetOnSaveScene(std::function<void()> callback) { m_OnSaveScene = callback; }

private:
    std::function<void()> m_OnSaveScene;
};

} // namespace LGE


