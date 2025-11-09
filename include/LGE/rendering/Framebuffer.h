#pragma once

#include <cstdint>

namespace LGE {

class Framebuffer {
public:
    Framebuffer(uint32_t width, uint32_t height);
    ~Framebuffer();

    void Bind() const;
    void Unbind() const;
    void Resize(uint32_t width, uint32_t height);

    uint32_t GetColorAttachmentRendererID() const { return m_ColorAttachment; }
    uint32_t GetWidth() const { return m_Width; }
    uint32_t GetHeight() const { return m_Height; }

private:
    uint32_t m_RendererID;
    uint32_t m_ColorAttachment;
    uint32_t m_DepthAttachment;
    uint32_t m_Width;
    uint32_t m_Height;

    void Invalidate();
    void Release();
};

} // namespace LGE





