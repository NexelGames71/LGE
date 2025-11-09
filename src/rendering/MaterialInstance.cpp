#include "LGE/rendering/MaterialInstance.h"
#include "LGE/rendering/Shader.h"
#include "LGE/core/Log.h"
#include <glad/glad.h>

namespace LGE {

MaterialInstance::MaterialInstance(std::shared_ptr<Material> baseMaterial)
    : m_BaseMaterial(baseMaterial)
    , m_Name(baseMaterial ? baseMaterial->GetName() + "_Instance" : "MaterialInstance")
{
    if (!baseMaterial) {
        Log::Error("MaterialInstance created with null base material!");
    }
}

MaterialInstance::MaterialInstance(std::shared_ptr<Material> baseMaterial, const std::string& name)
    : m_BaseMaterial(baseMaterial)
    , m_Name(name)
{
    if (!baseMaterial) {
        Log::Error("MaterialInstance created with null base material!");
    }
}

MaterialInstance::~MaterialInstance() {
}

// Base Color
void MaterialInstance::SetBaseColor(const Math::Vector3& color) {
    m_BaseColorOverridden = true;
    m_ParameterOverrides["u_BaseColor"] = MaterialParameter(color);
}

void MaterialInstance::SetBaseColorTexture(std::shared_ptr<Texture> texture) {
    m_BaseColorTextureOverride = texture;
}

Math::Vector3 MaterialInstance::GetBaseColor() const {
    if (m_BaseColorOverridden) {
        auto it = m_ParameterOverrides.find("u_BaseColor");
        if (it != m_ParameterOverrides.end() && it->second.type == MaterialParameter::Type::Vector3) {
            return it->second.vector3Value;
        }
    }
    return m_BaseMaterial ? m_BaseMaterial->GetBaseColor() : Math::Vector3(0.5f, 0.5f, 0.5f);
}

std::shared_ptr<Texture> MaterialInstance::GetBaseColorTexture() const {
    if (m_BaseColorTextureOverride) {
        return m_BaseColorTextureOverride;
    }
    return m_BaseMaterial ? m_BaseMaterial->GetBaseColorTexture() : nullptr;
}

// Metallic
void MaterialInstance::SetMetallic(float value) {
    m_MetallicOverridden = true;
    m_ParameterOverrides["u_Metallic"] = MaterialParameter(value);
}

void MaterialInstance::SetMetallicTexture(std::shared_ptr<Texture> texture) {
    m_MetallicTextureOverride = texture;
}

float MaterialInstance::GetMetallic() const {
    if (m_MetallicOverridden) {
        auto it = m_ParameterOverrides.find("u_Metallic");
        if (it != m_ParameterOverrides.end() && it->second.type == MaterialParameter::Type::Float) {
            return it->second.floatValue;
        }
    }
    return m_BaseMaterial ? m_BaseMaterial->GetMetallic() : 0.0f;
}

std::shared_ptr<Texture> MaterialInstance::GetMetallicTexture() const {
    if (m_MetallicTextureOverride) {
        return m_MetallicTextureOverride;
    }
    return m_BaseMaterial ? m_BaseMaterial->GetMetallicTexture() : nullptr;
}

// Roughness
void MaterialInstance::SetRoughness(float value) {
    m_RoughnessOverridden = true;
    m_ParameterOverrides["u_Roughness"] = MaterialParameter(value);
}

void MaterialInstance::SetRoughnessTexture(std::shared_ptr<Texture> texture) {
    m_RoughnessTextureOverride = texture;
}

float MaterialInstance::GetRoughness() const {
    if (m_RoughnessOverridden) {
        auto it = m_ParameterOverrides.find("u_Roughness");
        if (it != m_ParameterOverrides.end() && it->second.type == MaterialParameter::Type::Float) {
            return it->second.floatValue;
        }
    }
    return m_BaseMaterial ? m_BaseMaterial->GetRoughness() : 0.5f;
}

std::shared_ptr<Texture> MaterialInstance::GetRoughnessTexture() const {
    if (m_RoughnessTextureOverride) {
        return m_RoughnessTextureOverride;
    }
    return m_BaseMaterial ? m_BaseMaterial->GetRoughnessTexture() : nullptr;
}

// Specular
void MaterialInstance::SetSpecular(float value) {
    m_SpecularOverridden = true;
    m_ParameterOverrides["u_Specular"] = MaterialParameter(value);
}

float MaterialInstance::GetSpecular() const {
    if (m_SpecularOverridden) {
        auto it = m_ParameterOverrides.find("u_Specular");
        if (it != m_ParameterOverrides.end() && it->second.type == MaterialParameter::Type::Float) {
            return it->second.floatValue;
        }
    }
    return m_BaseMaterial ? m_BaseMaterial->GetSpecular() : 0.5f;
}

// Normal Map
void MaterialInstance::SetNormalTexture(std::shared_ptr<Texture> texture) {
    m_NormalTextureOverride = texture;
}

std::shared_ptr<Texture> MaterialInstance::GetNormalTexture() const {
    if (m_NormalTextureOverride) {
        return m_NormalTextureOverride;
    }
    return m_BaseMaterial ? m_BaseMaterial->GetNormalTexture() : nullptr;
}

// Ambient Occlusion
void MaterialInstance::SetAOTexture(std::shared_ptr<Texture> texture) {
    m_AOTextureOverride = texture;
}

std::shared_ptr<Texture> MaterialInstance::GetAOTexture() const {
    if (m_AOTextureOverride) {
        return m_AOTextureOverride;
    }
    return m_BaseMaterial ? m_BaseMaterial->GetAOTexture() : nullptr;
}

// Emissive
void MaterialInstance::SetEmissiveColor(const Math::Vector3& color) {
    m_EmissiveColorOverridden = true;
    m_ParameterOverrides["u_EmissiveColor"] = MaterialParameter(color);
}

void MaterialInstance::SetEmissiveTexture(std::shared_ptr<Texture> texture) {
    m_EmissiveTextureOverride = texture;
}

Math::Vector3 MaterialInstance::GetEmissiveColor() const {
    if (m_EmissiveColorOverridden) {
        auto it = m_ParameterOverrides.find("u_EmissiveColor");
        if (it != m_ParameterOverrides.end() && it->second.type == MaterialParameter::Type::Vector3) {
            return it->second.vector3Value;
        }
    }
    return m_BaseMaterial ? m_BaseMaterial->GetEmissiveColor() : Math::Vector3(0.0f, 0.0f, 0.0f);
}

std::shared_ptr<Texture> MaterialInstance::GetEmissiveTexture() const {
    if (m_EmissiveTextureOverride) {
        return m_EmissiveTextureOverride;
    }
    return m_BaseMaterial ? m_BaseMaterial->GetEmissiveTexture() : nullptr;
}

// Opacity
void MaterialInstance::SetOpacity(float value) {
    m_OpacityOverridden = true;
    m_ParameterOverrides["u_Opacity"] = MaterialParameter(value);
}

void MaterialInstance::SetOpacityTexture(std::shared_ptr<Texture> texture) {
    m_OpacityTextureOverride = texture;
}

float MaterialInstance::GetOpacity() const {
    if (m_OpacityOverridden) {
        auto it = m_ParameterOverrides.find("u_Opacity");
        if (it != m_ParameterOverrides.end() && it->second.type == MaterialParameter::Type::Float) {
            return it->second.floatValue;
        }
    }
    return m_BaseMaterial ? m_BaseMaterial->GetOpacity() : 1.0f;
}

std::shared_ptr<Texture> MaterialInstance::GetOpacityTexture() const {
    if (m_OpacityTextureOverride) {
        return m_OpacityTextureOverride;
    }
    return m_BaseMaterial ? m_BaseMaterial->GetOpacityTexture() : nullptr;
}

// Advanced PBR Properties
void MaterialInstance::SetClearCoat(float value) {
    m_ClearCoatOverridden = true;
    m_ParameterOverrides["u_ClearCoat"] = MaterialParameter(value);
}

void MaterialInstance::SetClearCoatRoughness(float value) {
    m_ClearCoatRoughnessOverridden = true;
    m_ParameterOverrides["u_ClearCoatRoughness"] = MaterialParameter(value);
}

float MaterialInstance::GetClearCoat() const {
    if (m_ClearCoatOverridden) {
        auto it = m_ParameterOverrides.find("u_ClearCoat");
        if (it != m_ParameterOverrides.end() && it->second.type == MaterialParameter::Type::Float) {
            return it->second.floatValue;
        }
    }
    return m_BaseMaterial ? m_BaseMaterial->GetClearCoat() : 0.0f;
}

float MaterialInstance::GetClearCoatRoughness() const {
    if (m_ClearCoatRoughnessOverridden) {
        auto it = m_ParameterOverrides.find("u_ClearCoatRoughness");
        if (it != m_ParameterOverrides.end() && it->second.type == MaterialParameter::Type::Float) {
            return it->second.floatValue;
        }
    }
    return m_BaseMaterial ? m_BaseMaterial->GetClearCoatRoughness() : 0.0f;
}

void MaterialInstance::SetSubsurface(float value) {
    m_SubsurfaceOverridden = true;
    m_ParameterOverrides["u_Subsurface"] = MaterialParameter(value);
}

void MaterialInstance::SetSubsurfaceColor(const Math::Vector3& color) {
    m_SubsurfaceColorOverridden = true;
    m_ParameterOverrides["u_SubsurfaceColor"] = MaterialParameter(color);
}

float MaterialInstance::GetSubsurface() const {
    if (m_SubsurfaceOverridden) {
        auto it = m_ParameterOverrides.find("u_Subsurface");
        if (it != m_ParameterOverrides.end() && it->second.type == MaterialParameter::Type::Float) {
            return it->second.floatValue;
        }
    }
    return m_BaseMaterial ? m_BaseMaterial->GetSubsurface() : 0.0f;
}

Math::Vector3 MaterialInstance::GetSubsurfaceColor() const {
    if (m_SubsurfaceColorOverridden) {
        auto it = m_ParameterOverrides.find("u_SubsurfaceColor");
        if (it != m_ParameterOverrides.end() && it->second.type == MaterialParameter::Type::Vector3) {
            return it->second.vector3Value;
        }
    }
    return m_BaseMaterial ? m_BaseMaterial->GetSubsurfaceColor() : Math::Vector3(1.0f, 1.0f, 1.0f);
}

void MaterialInstance::SetIOR(float value) {
    m_IOROverridden = true;
    m_ParameterOverrides["u_IOR"] = MaterialParameter(value);
}

float MaterialInstance::GetIOR() const {
    if (m_IOROverridden) {
        auto it = m_ParameterOverrides.find("u_IOR");
        if (it != m_ParameterOverrides.end() && it->second.type == MaterialParameter::Type::Float) {
            return it->second.floatValue;
        }
    }
    return m_BaseMaterial ? m_BaseMaterial->GetIOR() : 1.5f;
}

void MaterialInstance::SetAnisotropy(float value) {
    m_AnisotropyOverridden = true;
    m_ParameterOverrides["u_Anisotropy"] = MaterialParameter(value);
}

void MaterialInstance::SetAnisotropyDirection(const Math::Vector3& direction) {
    m_AnisotropyDirectionOverridden = true;
    m_ParameterOverrides["u_AnisotropyDirection"] = MaterialParameter(direction);
}

float MaterialInstance::GetAnisotropy() const {
    if (m_AnisotropyOverridden) {
        auto it = m_ParameterOverrides.find("u_Anisotropy");
        if (it != m_ParameterOverrides.end() && it->second.type == MaterialParameter::Type::Float) {
            return it->second.floatValue;
        }
    }
    return m_BaseMaterial ? m_BaseMaterial->GetAnisotropy() : 0.0f;
}

Math::Vector3 MaterialInstance::GetAnisotropyDirection() const {
    if (m_AnisotropyDirectionOverridden) {
        auto it = m_ParameterOverrides.find("u_AnisotropyDirection");
        if (it != m_ParameterOverrides.end() && it->second.type == MaterialParameter::Type::Vector3) {
            return it->second.vector3Value;
        }
    }
    return m_BaseMaterial ? m_BaseMaterial->GetAnisotropyDirection() : Math::Vector3(1.0f, 0.0f, 0.0f);
}

void MaterialInstance::SetTransmission(float value) {
    m_TransmissionOverridden = true;
    m_ParameterOverrides["u_Transmission"] = MaterialParameter(value);
}

float MaterialInstance::GetTransmission() const {
    if (m_TransmissionOverridden) {
        auto it = m_ParameterOverrides.find("u_Transmission");
        if (it != m_ParameterOverrides.end() && it->second.type == MaterialParameter::Type::Float) {
            return it->second.floatValue;
        }
    }
    return m_BaseMaterial ? m_BaseMaterial->GetTransmission() : 0.0f;
}

// Height / Displacement
void MaterialInstance::SetHeightTexture(std::shared_ptr<Texture> texture) {
    m_HeightTextureOverride = texture;
}

std::shared_ptr<Texture> MaterialInstance::GetHeightTexture() const {
    if (m_HeightTextureOverride) {
        return m_HeightTextureOverride;
    }
    return m_BaseMaterial ? m_BaseMaterial->GetHeightTexture() : nullptr;
}

// Generic parameter overrides
void MaterialInstance::SetFloatOverride(const std::string& name, float value) {
    m_ParameterOverrides[name] = MaterialParameter(value);
}

void MaterialInstance::SetVector3Override(const std::string& name, const Math::Vector3& value) {
    m_ParameterOverrides[name] = MaterialParameter(value);
}

void MaterialInstance::SetVector4Override(const std::string& name, const Math::Vector4& value) {
    m_ParameterOverrides[name] = MaterialParameter(value);
}

void MaterialInstance::SetTextureOverride(const std::string& name, std::shared_ptr<Texture> texture) {
    m_ParameterOverrides[name] = MaterialParameter(texture);
}

void MaterialInstance::ClearOverride(const std::string& name) {
    m_ParameterOverrides.erase(name);
    
    // Clear override flags
    if (name == "u_BaseColor") m_BaseColorOverridden = false;
    else if (name == "u_Metallic") m_MetallicOverridden = false;
    else if (name == "u_Roughness") m_RoughnessOverridden = false;
    else if (name == "u_Specular") m_SpecularOverridden = false;
    else if (name == "u_EmissiveColor") m_EmissiveColorOverridden = false;
    else if (name == "u_Opacity") m_OpacityOverridden = false;
    else if (name == "u_ClearCoat") m_ClearCoatOverridden = false;
    else if (name == "u_ClearCoatRoughness") m_ClearCoatRoughnessOverridden = false;
    else if (name == "u_Subsurface") m_SubsurfaceOverridden = false;
    else if (name == "u_SubsurfaceColor") m_SubsurfaceColorOverridden = false;
    else if (name == "u_IOR") m_IOROverridden = false;
    else if (name == "u_Anisotropy") m_AnisotropyOverridden = false;
    else if (name == "u_AnisotropyDirection") m_AnisotropyDirectionOverridden = false;
    else if (name == "u_Transmission") m_TransmissionOverridden = false;
}

void MaterialInstance::ClearAllOverrides() {
    m_ParameterOverrides.clear();
    m_BaseColorOverridden = false;
    m_MetallicOverridden = false;
    m_RoughnessOverridden = false;
    m_SpecularOverridden = false;
    m_EmissiveColorOverridden = false;
    m_OpacityOverridden = false;
    m_ClearCoatOverridden = false;
    m_ClearCoatRoughnessOverridden = false;
    m_SubsurfaceOverridden = false;
    m_SubsurfaceColorOverridden = false;
    m_IOROverridden = false;
    m_AnisotropyOverridden = false;
    m_AnisotropyDirectionOverridden = false;
    m_TransmissionOverridden = false;
    
    m_BaseColorTextureOverride.reset();
    m_MetallicTextureOverride.reset();
    m_RoughnessTextureOverride.reset();
    m_NormalTextureOverride.reset();
    m_AOTextureOverride.reset();
    m_EmissiveTextureOverride.reset();
    m_OpacityTextureOverride.reset();
    m_HeightTextureOverride.reset();
    
    m_BlendModeOverride.reset();
    m_CullModeOverride.reset();
    m_DepthWriteOverride.reset();
}

// Render Mode overrides
void MaterialInstance::SetBlendMode(BlendMode mode) {
    m_BlendModeOverride = mode;
}

BlendMode MaterialInstance::GetBlendMode() const {
    if (m_BlendModeOverride.has_value()) {
        return m_BlendModeOverride.value();
    }
    return m_BaseMaterial ? m_BaseMaterial->GetBlendMode() : BlendMode::Opaque;
}

void MaterialInstance::SetCullMode(CullMode mode) {
    m_CullModeOverride = mode;
}

CullMode MaterialInstance::GetCullMode() const {
    if (m_CullModeOverride.has_value()) {
        return m_CullModeOverride.value();
    }
    return m_BaseMaterial ? m_BaseMaterial->GetCullMode() : CullMode::Back;
}

void MaterialInstance::SetDepthWrite(DepthWriteMode mode) {
    m_DepthWriteOverride = mode;
}

DepthWriteMode MaterialInstance::GetDepthWrite() const {
    if (m_DepthWriteOverride.has_value()) {
        return m_DepthWriteOverride.value();
    }
    return m_BaseMaterial ? m_BaseMaterial->GetDepthWrite() : DepthWriteMode::Enabled;
}

// Binding - applies base material then overrides
void MaterialInstance::Bind() const {
    if (!m_BaseMaterial) {
        return;
    }

    // Apply render state (from instance overrides or base material)
    BlendMode blendMode = GetBlendMode();
    CullMode cullMode = GetCullMode();
    DepthWriteMode depthWrite = GetDepthWrite();
    
    // Apply blend mode
    switch (blendMode) {
        case BlendMode::Opaque:
            glDisable(GL_BLEND);
            break;
        case BlendMode::Masked:
            glDisable(GL_BLEND);
            break;
        case BlendMode::Translucent:
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glBlendEquation(GL_FUNC_ADD);
            break;
        case BlendMode::Additive:
            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ONE);
            glBlendEquation(GL_FUNC_ADD);
            break;
        case BlendMode::Modulate:
            glEnable(GL_BLEND);
            glBlendFunc(GL_DST_COLOR, GL_ZERO);
            glBlendEquation(GL_FUNC_ADD);
            break;
    }
    
    // Apply cull mode
    switch (cullMode) {
        case CullMode::None:
            glDisable(GL_CULL_FACE);
            break;
        case CullMode::Front:
            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);
            break;
        case CullMode::Back:
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            break;
    }
    
    // Apply depth write
    switch (depthWrite) {
        case DepthWriteMode::Enabled:
            glDepthMask(GL_TRUE);
            break;
        case DepthWriteMode::Disabled:
            glDepthMask(GL_FALSE);
            break;
    }

    // Get the shader from base material
    auto shader = m_BaseMaterial->GetShader();
    if (!shader) {
        return;
    }

    // Bind shader and upload base material properties
    // We manually upload properties to avoid double-applying render state
    shader->Bind();
    
    // Upload base material's legacy properties
    // (We'd need access to private members for this, so we'll use Bind() and re-apply render state)
    // Bind the base material (it will apply its render state)
    m_BaseMaterial->Bind();
    
    // Re-apply our render state (which may override base material's render state)
    // This ensures instance overrides take precedence
    // Apply blend mode
    switch (blendMode) {
        case BlendMode::Opaque:
            glDisable(GL_BLEND);
            break;
        case BlendMode::Masked:
            glDisable(GL_BLEND);
            break;
        case BlendMode::Translucent:
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glBlendEquation(GL_FUNC_ADD);
            break;
        case BlendMode::Additive:
            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ONE);
            glBlendEquation(GL_FUNC_ADD);
            break;
        case BlendMode::Modulate:
            glEnable(GL_BLEND);
            glBlendFunc(GL_DST_COLOR, GL_ZERO);
            glBlendEquation(GL_FUNC_ADD);
            break;
    }
    
    // Apply cull mode
    switch (cullMode) {
        case CullMode::None:
            glDisable(GL_CULL_FACE);
            break;
        case CullMode::Front:
            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);
            break;
        case CullMode::Back:
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            break;
    }
    
    // Apply depth write
    switch (depthWrite) {
        case DepthWriteMode::Enabled:
            glDepthMask(GL_TRUE);
            break;
        case DepthWriteMode::Disabled:
            glDepthMask(GL_FALSE);
            break;
    }

    // Apply scalar/vector overrides
    for (const auto& override : m_ParameterOverrides) {
        const auto& param = override.second;
        switch (param.type) {
            case MaterialParameter::Type::Float:
                shader->SetUniform1f(override.first, param.floatValue);
                break;
            case MaterialParameter::Type::Vector3:
                shader->SetUniform3f(override.first, param.vector3Value.x, param.vector3Value.y, param.vector3Value.z);
                break;
            case MaterialParameter::Type::Vector4:
                shader->SetUniform4f(override.first, param.vector4Value.x, param.vector4Value.y, param.vector4Value.z, param.vector4Value.w);
                break;
            case MaterialParameter::Type::Texture:
                // Texture overrides are handled separately below
                break;
        }
    }

    // Apply PBR property overrides
    if (m_BaseColorOverridden) {
        auto it = m_ParameterOverrides.find("u_BaseColor");
        if (it != m_ParameterOverrides.end() && it->second.type == MaterialParameter::Type::Vector3) {
            shader->SetUniform3f("u_BaseColor", it->second.vector3Value.x, it->second.vector3Value.y, it->second.vector3Value.z);
        }
    }

    if (m_MetallicOverridden) {
        auto it = m_ParameterOverrides.find("u_Metallic");
        if (it != m_ParameterOverrides.end() && it->second.type == MaterialParameter::Type::Float) {
            shader->SetUniform1f("u_Metallic", it->second.floatValue);
        }
    }

    if (m_RoughnessOverridden) {
        auto it = m_ParameterOverrides.find("u_Roughness");
        if (it != m_ParameterOverrides.end() && it->second.type == MaterialParameter::Type::Float) {
            shader->SetUniform1f("u_Roughness", it->second.floatValue);
        }
    }

    if (m_SpecularOverridden) {
        auto it = m_ParameterOverrides.find("u_Specular");
        if (it != m_ParameterOverrides.end() && it->second.type == MaterialParameter::Type::Float) {
            shader->SetUniform1f("u_Specular", it->second.floatValue);
        }
    }

    if (m_EmissiveColorOverridden) {
        auto it = m_ParameterOverrides.find("u_EmissiveColor");
        if (it != m_ParameterOverrides.end() && it->second.type == MaterialParameter::Type::Vector3) {
            shader->SetUniform3f("u_EmissiveColor", it->second.vector3Value.x, it->second.vector3Value.y, it->second.vector3Value.z);
        }
    }

    if (m_OpacityOverridden) {
        auto it = m_ParameterOverrides.find("u_Opacity");
        if (it != m_ParameterOverrides.end() && it->second.type == MaterialParameter::Type::Float) {
            shader->SetUniform1f("u_Opacity", it->second.floatValue);
        }
    }

    // Apply texture overrides (rebind textures if overridden)
    int textureSlot = 0;
    
    // Base Color Texture
    auto baseColorTex = GetBaseColorTexture();
    if (baseColorTex) {
        baseColorTex->Bind(textureSlot);
        shader->SetUniform1i("u_AlbedoMap", textureSlot);
        shader->SetUniform1i("u_HasAlbedoMap", 1);
        textureSlot++;
    } else {
        shader->SetUniform1i("u_HasAlbedoMap", 0);
    }

    // Metallic Texture
    auto metallicTex = GetMetallicTexture();
    if (metallicTex) {
        metallicTex->Bind(textureSlot);
        shader->SetUniform1i("u_MetallicMap", textureSlot);
        shader->SetUniform1i("u_HasMetallicMap", 1);
        textureSlot++;
    } else {
        shader->SetUniform1i("u_HasMetallicMap", 0);
    }

    // Roughness Texture
    auto roughnessTex = GetRoughnessTexture();
    if (roughnessTex) {
        roughnessTex->Bind(textureSlot);
        shader->SetUniform1i("u_RoughnessMap", textureSlot);
        shader->SetUniform1i("u_HasRoughnessMap", 1);
        textureSlot++;
    } else {
        shader->SetUniform1i("u_HasRoughnessMap", 0);
    }

    // Normal Texture
    auto normalTex = GetNormalTexture();
    if (normalTex) {
        normalTex->Bind(textureSlot);
        shader->SetUniform1i("u_NormalMap", textureSlot);
        shader->SetUniform1i("u_HasNormalMap", 1);
        textureSlot++;
    } else {
        shader->SetUniform1i("u_HasNormalMap", 0);
    }

    // AO Texture
    auto aoTex = GetAOTexture();
    if (aoTex) {
        aoTex->Bind(textureSlot);
        shader->SetUniform1i("u_AOMap", textureSlot);
        shader->SetUniform1i("u_HasAOMap", 1);
        textureSlot++;
    } else {
        shader->SetUniform1i("u_HasAOMap", 0);
    }

    // Emissive Texture
    auto emissiveTex = GetEmissiveTexture();
    if (emissiveTex) {
        emissiveTex->Bind(textureSlot);
        shader->SetUniform1i("u_EmissiveMap", textureSlot);
        shader->SetUniform1i("u_HasEmissiveMap", 1);
        textureSlot++;
    } else {
        shader->SetUniform1i("u_HasEmissiveMap", 0);
    }

    // Opacity Texture
    auto opacityTex = GetOpacityTexture();
    if (opacityTex) {
        opacityTex->Bind(textureSlot);
        shader->SetUniform1i("u_OpacityMap", textureSlot);
        shader->SetUniform1i("u_HasOpacityMap", 1);
        textureSlot++;
    } else {
        shader->SetUniform1i("u_HasOpacityMap", 0);
    }

    // Height Texture
    auto heightTex = GetHeightTexture();
    if (heightTex) {
        heightTex->Bind(textureSlot);
        shader->SetUniform1i("u_HeightMap", textureSlot);
        shader->SetUniform1i("u_HasHeightMap", 1);
        textureSlot++;
    } else {
        shader->SetUniform1i("u_HasHeightMap", 0);
    }
}

void MaterialInstance::Unbind() const {
    if (m_BaseMaterial) {
        m_BaseMaterial->Unbind();
    }
}

bool MaterialInstance::IsOverridden(const std::string& name) const {
    return m_ParameterOverrides.find(name) != m_ParameterOverrides.end();
}

} // namespace LGE

