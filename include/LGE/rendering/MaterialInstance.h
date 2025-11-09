#pragma once

#include "LGE/rendering/Material.h"
#include "LGE/rendering/Texture.h"
#include "LGE/math/Vector.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <optional>

namespace LGE {

// Material parameter variant type for storing overrides
struct MaterialParameter {
    enum class Type {
        Float,
        Vector3,
        Vector4,
        Texture
    };
    
    Type type;
    union {
        float floatValue;
        Math::Vector3 vector3Value;
        Math::Vector4 vector4Value;
    };
    std::shared_ptr<Texture> textureValue;
    
    MaterialParameter() : type(Type::Float), floatValue(0.0f) {}
    MaterialParameter(float value) : type(Type::Float), floatValue(value) {}
    MaterialParameter(const Math::Vector3& value) : type(Type::Vector3), vector3Value(value) {}
    MaterialParameter(const Math::Vector4& value) : type(Type::Vector4), vector4Value(value) {}
    MaterialParameter(std::shared_ptr<Texture> texture) : type(Type::Texture), textureValue(texture) {}
};

// Material Instance - allows overriding parameters from a base Material
// without recompiling the shader (Unreal-style)
class MaterialInstance {
public:
    MaterialInstance(std::shared_ptr<Material> baseMaterial);
    MaterialInstance(std::shared_ptr<Material> baseMaterial, const std::string& name);
    ~MaterialInstance();

    // Get base material
    std::shared_ptr<Material> GetBaseMaterial() const { return m_BaseMaterial; }

    // PBR Property Overrides (same interface as Material)
    // Base Color
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

    void SetIOR(float value);
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

    // Generic parameter overrides (for legacy properties)
    void SetFloatOverride(const std::string& name, float value);
    void SetVector3Override(const std::string& name, const Math::Vector3& value);
    void SetVector4Override(const std::string& name, const Math::Vector4& value);
    void SetTextureOverride(const std::string& name, std::shared_ptr<Texture> texture);

    // Clear overrides
    void ClearOverride(const std::string& name);
    void ClearAllOverrides();

    // Render Mode overrides
    void SetBlendMode(BlendMode mode);
    BlendMode GetBlendMode() const;

    void SetCullMode(CullMode mode);
    CullMode GetCullMode() const;

    void SetDepthWrite(DepthWriteMode mode);
    DepthWriteMode GetDepthWrite() const;

    // Binding - applies base material then overrides
    void Bind() const;
    void Unbind() const;

    // Name
    const std::string& GetName() const { return m_Name; }
    void SetName(const std::string& name) { m_Name = name; }

    // Check if a parameter is overridden
    bool IsOverridden(const std::string& name) const;

private:
    std::string m_Name;
    std::shared_ptr<Material> m_BaseMaterial;
    
    // Parameter overrides
    std::unordered_map<std::string, MaterialParameter> m_ParameterOverrides;
    
    // PBR property override flags
    bool m_BaseColorOverridden = false;
    bool m_MetallicOverridden = false;
    bool m_RoughnessOverridden = false;
    bool m_SpecularOverridden = false;
    bool m_EmissiveColorOverridden = false;
    bool m_OpacityOverridden = false;
    bool m_ClearCoatOverridden = false;
    bool m_ClearCoatRoughnessOverridden = false;
    bool m_SubsurfaceOverridden = false;
    bool m_SubsurfaceColorOverridden = false;
    bool m_IOROverridden = false;
    bool m_AnisotropyOverridden = false;
    bool m_AnisotropyDirectionOverridden = false;
    bool m_TransmissionOverridden = false;
    
    // Texture overrides
    std::shared_ptr<Texture> m_BaseColorTextureOverride;
    std::shared_ptr<Texture> m_MetallicTextureOverride;
    std::shared_ptr<Texture> m_RoughnessTextureOverride;
    std::shared_ptr<Texture> m_NormalTextureOverride;
    std::shared_ptr<Texture> m_AOTextureOverride;
    std::shared_ptr<Texture> m_EmissiveTextureOverride;
    std::shared_ptr<Texture> m_OpacityTextureOverride;
    std::shared_ptr<Texture> m_HeightTextureOverride;
    
    // Render mode overrides
    std::optional<BlendMode> m_BlendModeOverride;
    std::optional<CullMode> m_CullModeOverride;
    std::optional<DepthWriteMode> m_DepthWriteOverride;
};

} // namespace LGE

