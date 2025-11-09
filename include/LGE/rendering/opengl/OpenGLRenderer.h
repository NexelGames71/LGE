#pragma once

#include "LGE/rendering/Renderer.h"

namespace LGE {

class OpenGLRenderer : public Renderer {
public:
    OpenGLRenderer(Window* window);
    virtual ~OpenGLRenderer();

    void Init() override;
    void Shutdown() override;
    void BeginFrame() override;
    void EndFrame() override;
    void Clear(float r, float g, float b, float a) override;
    void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

private:
    bool m_Initialized;
};

} // namespace LGE

