#pragma once

#include <cstdint>
#include <string>

namespace LGE {

class Texture {
public:
    Texture();
    ~Texture();

    bool LoadHDR(const std::string& filepath);
    bool LoadEXR(const std::string& filepath);
    bool LoadHDRImage(const std::string& filepath); // Auto-detects HDR or EXR
    bool LoadImageFile(const std::string& filepath); // Load PNG/JPG images
    void Bind(uint32_t slot = 0) const;
    void Unbind() const;

    uint32_t GetRendererID() const { return m_RendererID; }
    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }

private:
    uint32_t m_RendererID;
    int m_Width;
    int m_Height;
    bool m_IsHDR;
};

} // namespace LGE

