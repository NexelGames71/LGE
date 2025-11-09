#pragma once

#include <cstdint>
#include <string>

namespace LGE {

// Texture wrap modes
enum class TextureWrapMode {
    Repeat,         // GL_REPEAT
    ClampToEdge,    // GL_CLAMP_TO_EDGE
    ClampToBorder,  // GL_CLAMP_TO_BORDER
    MirrorRepeat    // GL_MIRRORED_REPEAT
};

// Texture filter modes
enum class TextureFilterMode {
    Nearest,        // GL_NEAREST
    Linear,         // GL_LINEAR
    NearestMipmapNearest,   // GL_NEAREST_MIPMAP_NEAREST
    LinearMipmapNearest,    // GL_LINEAR_MIPMAP_NEAREST
    NearestMipmapLinear,    // GL_NEAREST_MIPMAP_LINEAR
    LinearMipmapLinear      // GL_LINEAR_MIPMAP_LINEAR
};

// Color space modes
enum class TextureColorSpace {
    Linear,         // Linear color space (for normal maps, roughness, etc.)
    sRGB           // sRGB color space (for albedo/diffuse textures)
};

class Texture {
public:
    Texture();
    ~Texture();

    // Loading functions
    bool LoadHDR(const std::string& filepath);
    bool LoadEXR(const std::string& filepath);
    bool LoadHDRImage(const std::string& filepath); // Auto-detects HDR or EXR
    bool LoadImageFile(const std::string& filepath); // Load PNG/JPG images
    bool LoadImageFile(const std::string& filepath, TextureColorSpace colorSpace); // Load with color space specification
    
    // Binding
    void Bind(uint32_t slot = 0) const;
    void Unbind() const;

    // Getters
    uint32_t GetRendererID() const { return m_RendererID; }
    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }
    bool IsHDR() const { return m_IsHDR; }
    TextureColorSpace GetColorSpace() const { return m_ColorSpace; }

    // Sampler state configuration
    void SetWrapMode(TextureWrapMode wrapS, TextureWrapMode wrapT);
    void SetWrapMode(TextureWrapMode wrapMode); // Set both S and T to same mode
    void SetFilterMode(TextureFilterMode minFilter, TextureFilterMode magFilter);
    void SetFilterMode(TextureFilterMode filterMode); // Set both min and mag to same mode
    void SetAnisotropicFiltering(float anisotropy); // 1.0 = disabled, higher = more anisotropic
    void SetBorderColor(float r, float g, float b, float a = 1.0f);
    
    // Mipmap control
    void GenerateMipmaps();
    bool HasMipmaps() const { return m_HasMipmaps; }
    
    // Color space
    void SetColorSpace(TextureColorSpace colorSpace);

private:
    void ApplySamplerState(); // Apply all sampler state settings to OpenGL
    
    uint32_t m_RendererID;
    int m_Width;
    int m_Height;
    bool m_IsHDR;
    bool m_HasMipmaps;
    
    // Sampler state
    TextureWrapMode m_WrapS;
    TextureWrapMode m_WrapT;
    TextureFilterMode m_MinFilter;
    TextureFilterMode m_MagFilter;
    float m_Anisotropy;
    float m_BorderColor[4];
    TextureColorSpace m_ColorSpace;
};

} // namespace LGE

