#pragma once

#include <cstdint>

namespace LGE {

class Window;

class UI {
public:
    static void Initialize(Window* window);
    static void Shutdown();
    static void BeginFrame();
    static void EndFrame();
    static void Render();

    static bool WantCaptureMouse();
    static bool WantCaptureKeyboard();
};

} // namespace LGE





