#pragma once

#include "LGE/rendering/Shader.h"
#include "LGE/rendering/Texture.h"
#include "LGE/math/Vector.h"
#include <memory>
#include <string>
#include <unordered_map>

namespace LGE {

// Forward declaration
class Texture;

// Render Mode Enums (Unreal-style)
enum class BlendMode {
    Opaque,      // No blending
    Masked,      // Alpha test
    Translucent, // Alpha blending
    Additive,    // Additive blending
    Modulate     // Modulate blending
};

enum class CullMode {
    None,        // Two-sided
    Front,       // Cull front faces
    Back         // Cull back faces (default)
};

enum class DepthWriteMode {
    Enabled,     // Write to depth buffer
    Disabled     // Don't write to depth buffer
};

// Texture slot identifiers
enum class TextureSlot {
    Albedo = 0,
    Metallic = 1,
    Roughness = 2,
    Normal = 3,
    AmbientOcclusion = 4,
    Emissive = 5,
    Height = 6,
    Opacity = 7,
    Subsurface = 8,
    ClearCoat = 9,
    ClearCoatRoughness = 10,
    Anisotropy = 11,
    Transmission = 12,
    MaxSlots = 16
};

class Material {
public:
    Material();
    Material(const std::string& name);
    ~Material();

    // Shader management
    void SetShader(std::shared_ptr<Shader> shader) { m_Shader = shader; }
    std::shared_ptr<Shader> GetShader() const { return m_Shader; }

    // Legacy property setters (backward compatibility)
    void SetFloat(const std::string& name, float value);
    void SetVector3(const std::string& name, const Math::Vector3& value);
    void SetVector4(const std::string& name, const Math::Vector4& value);
    void SetColor(const std::string& name, const Math::Vector3& color);

    float GetFloat(const std::string& name) const;
    Math::Vector3 GetVector3(const std::string& name) const;
    Math::Vector4 GetVector4(const std::string& name) const;

    // PBR Property Setters (support both scalar/vector and textures)
    // Base Color / Albedo
    void SetBaseColor(const Math::Vector3& color);
    void SetBaseColorTexture(std::shared_ptr<Texture> texture);
    Math::Vector3 GetBaseColor() const;
    std::shared_ptr<Texture> GetBaseColorTexture() const;

    // Metallic
    void SetMetallic(float value);
    void SetMetallicTexture(std::shared_ptr<Texture> texture);
    float GetMetallic() const;
    std::shared_ptr<Texture> GetMetallicTexture() const;

    // Roughness
    void SetRoughness(float value);
    void SetRoughnessTexture(std::shared_ptr<Texture> texture);
    float GetRoughness() const;
    std::shared_ptr<Texture> GetRoughnessTexture() const;

    // Specular
    void SetSpecular(float value);
    float GetSpecular() const;

    // Normal Map
    void SetNormalTexture(std::shared_ptr<Texture> texture);
    std::shared_ptr<Texture> GetNormalTexture() const;

    // Ambient Occlusion
    void SetAOTexture(std::shared_ptr<Texture> texture);
    std::shared_ptr<Texture> GetAOTexture() const;

    // Emissive
    void SetEmissiveColor(const Math::Vector3& color);
    void SetEmissiveTexture(std::shared_ptr<Texture> texture);
    Math::Vector3 GetEmissiveColor() const;
    std::shared_ptr<Texture> GetEmissiveTexture() const;

    // Opacity
    void SetOpacity(float value);
    void SetOpacityTexture(std::shared_ptr<Texture> texture);
    float GetOpacity() const;
    std::shared_ptr<Texture> GetOpacityTexture() const;

    // Advanced PBR Properties
    void SetClearCoat(float value);
    void SetClearCoatRoughness(float value);
    float GetClearCoat() const;
    float GetClearCoatRoughness() const;

    void SetSubsurface(float value);
    void SetSubsurfaceColor(const Math::Vector3& color);
    float GetSubsurface() const;
    Math::Vector3 GetSubsurfaceColor() const;

    void SetIOR(float value); // Index of Refraction
    float GetIOR() const;

    void SetAnisotropy(float value);
    void SetAnisotropyDirection(const Math::Vector3& direction);
    float GetAnisotropy() const;
    Math::Vector3 GetAnisotropyDirection() const;

    void SetTransmission(float value);
    float GetTransmission() const;

    // Height / Displacement
    void SetHeightTexture(std::shared_ptr<Texture> texture);
    std::shared_ptr<Texture> GetHeightTexture() const;

    // Texture slot management (generic)
    void SetTexture(TextureSlot slot, std::shared_ptr<Texture> texture);
    std::shared_ptr<Texture> GetTexture(TextureSlot slot) const;

    // Render Mode Flags
    void SetBlendMode(BlendMode mode) { m_BlendMode = mode; }
    BlendMode GetBlendMode() const { return m_BlendMode; }

    void SetCullMode(CullMode mode) { m_CullMode = mode; }
    CullMode GetCullMode() const { return m_CullMode; }

    void SetDepthWrite(DepthWriteMode mode) { m_DepthWrite = mode; }
    DepthWriteMode GetDepthWrite() const { return m_DepthWrite; }

    // Material binding (uploads all properties to shader)
    void Bind() const;
    void Unbind() const;

    // Name
    const std::string& GetName() const { return m_Name; }
    void SetName(const std::string& name) { m_Name = name; }

    // Static factory methods for common materials
    static std::shared_ptr<Material> CreateDefaultGridMaterial();
    static std::shared_ptr<Material> CreateDefaultLitMaterial();
    static std::shared_ptr<Material> CreateDefaultPBRMaterial();

private:
    std::string m_Name;
    std::shared_ptr<Shader> m_Shader;

    // Legacy property storage (backward compatibility)
    std::unordered_map<std::string, float> m_FloatProperties;
    std::unordered_map<std::string, Math::Vector3> m_Vector3Properties;
    std::unordered_map<std::string, Math::Vector4> m_Vector4Properties;

    // PBR Property Storage
    // Base Color
    Math::Vector3 m_BaseColor = Math::Vector3(0.5f, 0.5f, 0.5f);
    std::shared_ptr<Texture> m_BaseColorTexture;

    // Metallic-Roughness
    float m_Metallic = 0.0f;
    std::shared_ptr<Texture> m_MetallicTexture;
    float m_Roughness = 0.5f;
    std::shared_ptr<Texture> m_RoughnessTexture;
    float m_Specular = 0.5f;

    // Normal and AO
    std::shared_ptr<Texture> m_NormalTexture;
    std::shared_ptr<Texture> m_AOTexture;

    // Emissive
    Math::Vector3 m_EmissiveColor = Math::Vector3(0.0f, 0.0f, 0.0f);
    std::shared_ptr<Texture> m_EmissiveTexture;

    // Opacity
    float m_Opacity = 1.0f;
    std::shared_ptr<Texture> m_OpacityTexture;

    // Advanced PBR
    float m_ClearCoat = 0.0f;
    float m_ClearCoatRoughness = 0.0f;
    float m_Subsurface = 0.0f;
    Math::Vector3 m_SubsurfaceColor = Math::Vector3(1.0f, 1.0f, 1.0f);
    float m_IOR = 1.5f; // Default glass IOR
    float m_Anisotropy = 0.0f;
    Math::Vector3 m_AnisotropyDirection = Math::Vector3(1.0f, 0.0f, 0.0f);
    float m_Transmission = 0.0f;
    std::shared_ptr<Texture> m_HeightTexture;

    // Texture slot array
    std::shared_ptr<Texture> m_TextureSlots[static_cast<int>(TextureSlot::MaxSlots)];

    // Render Mode Flags
    BlendMode m_BlendMode = BlendMode::Opaque;
    CullMode m_CullMode = CullMode::Back;
    DepthWriteMode m_DepthWrite = DepthWriteMode::Enabled;
};

} // namespace LGE


