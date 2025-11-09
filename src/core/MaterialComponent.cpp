#include "LGE/core/MaterialComponent.h"
#include "LGE/core/GameObject.h"

namespace LGE {

MaterialComponent::MaterialComponent()
    : Component()
{
}

MaterialComponent::~MaterialComponent() {
}

void MaterialComponent::SetMaterial(std::shared_ptr<Material> material) {
    m_Material = material;
    m_MaterialInstance = nullptr;
}

void MaterialComponent::SetMaterialInstance(std::shared_ptr<MaterialInstance> materialInstance) {
    m_MaterialInstance = materialInstance;
    if (materialInstance) {
        m_Material = materialInstance->GetBaseMaterial();
    }
}

std::shared_ptr<Material> MaterialComponent::GetMaterial() const {
    return m_Material;
}

std::shared_ptr<MaterialInstance> MaterialComponent::GetMaterialInstance() const {
    return m_MaterialInstance;
}

// PBR Property Setters
void MaterialComponent::SetBaseColor(const Math::Vector3& color) {
    if (m_MaterialInstance) {
        m_MaterialInstance->SetBaseColor(color);
    } else if (m_Material) {
        m_Material->SetBaseColor(color);
    }
}

Math::Vector3 MaterialComponent::GetBaseColor() const {
    if (m_MaterialInstance) {
        return m_MaterialInstance->GetBaseColor();
    } else if (m_Material) {
        return m_Material->GetBaseColor();
    }
    return Math::Vector3(0.5f, 0.5f, 0.5f);
}

void MaterialComponent::SetMetallic(float value) {
    if (m_MaterialInstance) {
        m_MaterialInstance->SetMetallic(value);
    } else if (m_Material) {
        m_Material->SetMetallic(value);
    }
}

float MaterialComponent::GetMetallic() const {
    if (m_MaterialInstance) {
        return m_MaterialInstance->GetMetallic();
    } else if (m_Material) {
        return m_Material->GetMetallic();
    }
    return 0.0f;
}

void MaterialComponent::SetRoughness(float value) {
    if (m_MaterialInstance) {
        m_MaterialInstance->SetRoughness(value);
    } else if (m_Material) {
        m_Material->SetRoughness(value);
    }
}

float MaterialComponent::GetRoughness() const {
    if (m_MaterialInstance) {
        return m_MaterialInstance->GetRoughness();
    } else if (m_Material) {
        return m_Material->GetRoughness();
    }
    return 0.5f;
}

void MaterialComponent::SetSpecular(float value) {
    if (m_MaterialInstance) {
        m_MaterialInstance->SetSpecular(value);
    } else if (m_Material) {
        m_Material->SetSpecular(value);
    }
}

float MaterialComponent::GetSpecular() const {
    if (m_MaterialInstance) {
        return m_MaterialInstance->GetSpecular();
    } else if (m_Material) {
        return m_Material->GetSpecular();
    }
    return 0.5f;
}

void MaterialComponent::SetEmissiveColor(const Math::Vector3& color) {
    if (m_MaterialInstance) {
        m_MaterialInstance->SetEmissiveColor(color);
    } else if (m_Material) {
        m_Material->SetEmissiveColor(color);
    }
}

Math::Vector3 MaterialComponent::GetEmissiveColor() const {
    if (m_MaterialInstance) {
        return m_MaterialInstance->GetEmissiveColor();
    } else if (m_Material) {
        return m_Material->GetEmissiveColor();
    }
    return Math::Vector3(0.0f, 0.0f, 0.0f);
}

void MaterialComponent::SetOpacity(float value) {
    if (m_MaterialInstance) {
        m_MaterialInstance->SetOpacity(value);
    } else if (m_Material) {
        m_Material->SetOpacity(value);
    }
}

float MaterialComponent::GetOpacity() const {
    if (m_MaterialInstance) {
        return m_MaterialInstance->GetOpacity();
    } else if (m_Material) {
        return m_Material->GetOpacity();
    }
    return 1.0f;
}

// Texture Properties
void MaterialComponent::SetBaseColorTexture(std::shared_ptr<Texture> texture) {
    if (m_MaterialInstance) {
        m_MaterialInstance->SetBaseColorTexture(texture);
    } else if (m_Material) {
        m_Material->SetBaseColorTexture(texture);
    }
}

std::shared_ptr<Texture> MaterialComponent::GetBaseColorTexture() const {
    if (m_MaterialInstance) {
        return m_MaterialInstance->GetBaseColorTexture();
    } else if (m_Material) {
        return m_Material->GetBaseColorTexture();
    }
    return nullptr;
}

void MaterialComponent::SetMetallicTexture(std::shared_ptr<Texture> texture) {
    if (m_MaterialInstance) {
        m_MaterialInstance->SetMetallicTexture(texture);
    } else if (m_Material) {
        m_Material->SetMetallicTexture(texture);
    }
}

std::shared_ptr<Texture> MaterialComponent::GetMetallicTexture() const {
    if (m_MaterialInstance) {
        return m_MaterialInstance->GetMetallicTexture();
    } else if (m_Material) {
        return m_Material->GetMetallicTexture();
    }
    return nullptr;
}

void MaterialComponent::SetRoughnessTexture(std::shared_ptr<Texture> texture) {
    if (m_MaterialInstance) {
        m_MaterialInstance->SetRoughnessTexture(texture);
    } else if (m_Material) {
        m_Material->SetRoughnessTexture(texture);
    }
}

std::shared_ptr<Texture> MaterialComponent::GetRoughnessTexture() const {
    if (m_MaterialInstance) {
        return m_MaterialInstance->GetRoughnessTexture();
    } else if (m_Material) {
        return m_Material->GetRoughnessTexture();
    }
    return nullptr;
}

void MaterialComponent::SetNormalTexture(std::shared_ptr<Texture> texture) {
    if (m_MaterialInstance) {
        m_MaterialInstance->SetNormalTexture(texture);
    } else if (m_Material) {
        m_Material->SetNormalTexture(texture);
    }
}

std::shared_ptr<Texture> MaterialComponent::GetNormalTexture() const {
    if (m_MaterialInstance) {
        return m_MaterialInstance->GetNormalTexture();
    } else if (m_Material) {
        return m_Material->GetNormalTexture();
    }
    return nullptr;
}

void MaterialComponent::SetAOTexture(std::shared_ptr<Texture> texture) {
    if (m_MaterialInstance) {
        m_MaterialInstance->SetAOTexture(texture);
    } else if (m_Material) {
        m_Material->SetAOTexture(texture);
    }
}

std::shared_ptr<Texture> MaterialComponent::GetAOTexture() const {
    if (m_MaterialInstance) {
        return m_MaterialInstance->GetAOTexture();
    } else if (m_Material) {
        return m_Material->GetAOTexture();
    }
    return nullptr;
}

void MaterialComponent::SetEmissiveTexture(std::shared_ptr<Texture> texture) {
    if (m_MaterialInstance) {
        m_MaterialInstance->SetEmissiveTexture(texture);
    } else if (m_Material) {
        m_Material->SetEmissiveTexture(texture);
    }
}

std::shared_ptr<Texture> MaterialComponent::GetEmissiveTexture() const {
    if (m_MaterialInstance) {
        return m_MaterialInstance->GetEmissiveTexture();
    } else if (m_Material) {
        return m_Material->GetEmissiveTexture();
    }
    return nullptr;
}

void MaterialComponent::SetOpacityTexture(std::shared_ptr<Texture> texture) {
    if (m_MaterialInstance) {
        m_MaterialInstance->SetOpacityTexture(texture);
    } else if (m_Material) {
        m_Material->SetOpacityTexture(texture);
    }
}

std::shared_ptr<Texture> MaterialComponent::GetOpacityTexture() const {
    if (m_MaterialInstance) {
        return m_MaterialInstance->GetOpacityTexture();
    } else if (m_Material) {
        return m_Material->GetOpacityTexture();
    }
    return nullptr;
}

void MaterialComponent::SetHeightTexture(std::shared_ptr<Texture> texture) {
    if (m_MaterialInstance) {
        m_MaterialInstance->SetHeightTexture(texture);
    } else if (m_Material) {
        m_Material->SetHeightTexture(texture);
    }
}

std::shared_ptr<Texture> MaterialComponent::GetHeightTexture() const {
    if (m_MaterialInstance) {
        return m_MaterialInstance->GetHeightTexture();
    } else if (m_Material) {
        return m_Material->GetHeightTexture();
    }
    return nullptr;
}

void MaterialComponent::SetBlendMode(BlendMode mode) {
    if (m_MaterialInstance) {
        m_MaterialInstance->SetBlendMode(mode);
    } else if (m_Material) {
        m_Material->SetBlendMode(mode);
    }
}

BlendMode MaterialComponent::GetBlendMode() const {
    if (m_MaterialInstance) {
        return m_MaterialInstance->GetBlendMode();
    } else if (m_Material) {
        return m_Material->GetBlendMode();
    }
    return BlendMode::Opaque;
}

void MaterialComponent::SetCullMode(CullMode mode) {
    if (m_MaterialInstance) {
        m_MaterialInstance->SetCullMode(mode);
    } else if (m_Material) {
        m_Material->SetCullMode(mode);
    }
}

CullMode MaterialComponent::GetCullMode() const {
    if (m_MaterialInstance) {
        return m_MaterialInstance->GetCullMode();
    } else if (m_Material) {
        return m_Material->GetCullMode();
    }
    return CullMode::Back;
}

void MaterialComponent::SetDepthWrite(DepthWriteMode mode) {
    if (m_MaterialInstance) {
        m_MaterialInstance->SetDepthWrite(mode);
    } else if (m_Material) {
        m_Material->SetDepthWrite(mode);
    }
}

DepthWriteMode MaterialComponent::GetDepthWrite() const {
    if (m_MaterialInstance) {
        return m_MaterialInstance->GetDepthWrite();
    } else if (m_Material) {
        return m_Material->GetDepthWrite();
    }
    return DepthWriteMode::Enabled;
}

void MaterialComponent::Bind() const {
    if (m_MaterialInstance) {
        m_MaterialInstance->Bind();
    } else if (m_Material) {
        m_Material->Bind();
    }
}

} // namespace LGE

