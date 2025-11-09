#pragma once

#include "LGE/core/Component.h"
#include "LGE/rendering/Material.h"
#include "LGE/rendering/MaterialInstance.h"
#include "LGE/rendering/Texture.h"
#include "LGE/math/Vector.h"
#include <memory>

namespace LGE {

// Material Component - attaches a Material or MaterialInstance to a GameObject
class MaterialComponent : public Component {
public:
    MaterialComponent();
    ~MaterialComponent();

    const std::string& GetTypeName() const override { static std::string name = "Material"; return name; }

    // Set material (creates a material instance if base material is provided)
    void SetMaterial(std::shared_ptr<Material> material);
    void SetMaterialInstance(std::shared_ptr<MaterialInstance> materialInstance);
    
    // Get material (returns instance if available, otherwise base material)
    std::shared_ptr<Material> GetMaterial() const;
    std::shared_ptr<MaterialInstance> GetMaterialInstance() const;
    
    // Check if using instance
    bool IsUsingInstance() const { return m_MaterialInstance != nullptr; }

    // PBR Property Setters (delegate to material/instance)
    // Base Color
    void SetBaseColor(const Math::Vector3& color);
    Math::Vector3 GetBaseColor() const;

    // Metallic
    void SetMetallic(float value);
    float GetMetallic() const;

    // Roughness
    void SetRoughness(float value);
    float GetRoughness() const;

    // Specular
    void SetSpecular(float value);
    float GetSpecular() const;

    // Emissive
    void SetEmissiveColor(const Math::Vector3& color);
    Math::Vector3 GetEmissiveColor() const;

    // Opacity
    void SetOpacity(float value);
    float GetOpacity() const;

    // Texture Properties
    void SetBaseColorTexture(std::shared_ptr<Texture> texture);
    std::shared_ptr<Texture> GetBaseColorTexture() const;
    
    void SetMetallicTexture(std::shared_ptr<Texture> texture);
    std::shared_ptr<Texture> GetMetallicTexture() const;
    
    void SetRoughnessTexture(std::shared_ptr<Texture> texture);
    std::shared_ptr<Texture> GetRoughnessTexture() const;
    
    void SetNormalTexture(std::shared_ptr<Texture> texture);
    std::shared_ptr<Texture> GetNormalTexture() const;
    
    void SetAOTexture(std::shared_ptr<Texture> texture);
    std::shared_ptr<Texture> GetAOTexture() const;
    
    void SetEmissiveTexture(std::shared_ptr<Texture> texture);
    std::shared_ptr<Texture> GetEmissiveTexture() const;
    
    void SetOpacityTexture(std::shared_ptr<Texture> texture);
    std::shared_ptr<Texture> GetOpacityTexture() const;
    
    void SetHeightTexture(std::shared_ptr<Texture> texture);
    std::shared_ptr<Texture> GetHeightTexture() const;

    // Render Mode
    void SetBlendMode(BlendMode mode);
    BlendMode GetBlendMode() const;

    void SetCullMode(CullMode mode);
    CullMode GetCullMode() const;

    void SetDepthWrite(DepthWriteMode mode);
    DepthWriteMode GetDepthWrite() const;

    // Bind material for rendering
    void Bind() const;

private:
    std::shared_ptr<Material> m_Material;
    std::shared_ptr<MaterialInstance> m_MaterialInstance;
};

} // namespace LGE

