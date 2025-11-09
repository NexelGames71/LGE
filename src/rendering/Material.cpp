#include "LGE/rendering/Material.h"
#include "LGE/rendering/Shader.h"
#include "LGE/rendering/Texture.h"
#include "LGE/core/FileSystem.h"
#include "LGE/core/Log.h"
#include <algorithm>
#include <cstring>

namespace LGE {

Material::Material() : m_Name("DefaultMaterial") {
    // Initialize texture slots to nullptr
    std::memset(m_TextureSlots, 0, sizeof(m_TextureSlots));
}

Material::Material(const std::string& name) : m_Name(name) {
    // Initialize texture slots to nullptr
    std::memset(m_TextureSlots, 0, sizeof(m_TextureSlots));
}

Material::~Material() {
}

void Material::SetFloat(const std::string& name, float value) {
    m_FloatProperties[name] = value;
}

void Material::SetVector3(const std::string& name, const Math::Vector3& value) {
    m_Vector3Properties[name] = value;
}

void Material::SetVector4(const std::string& name, const Math::Vector4& value) {
    m_Vector4Properties[name] = value;
}

void Material::SetColor(const std::string& name, const Math::Vector3& color) {
    m_Vector3Properties[name] = color;
}

float Material::GetFloat(const std::string& name) const {
    auto it = m_FloatProperties.find(name);
    if (it != m_FloatProperties.end()) {
        return it->second;
    }
    return 0.0f;
}

Math::Vector3 Material::GetVector3(const std::string& name) const {
    auto it = m_Vector3Properties.find(name);
    if (it != m_Vector3Properties.end()) {
        return it->second;
    }
    return Math::Vector3(0.0f, 0.0f, 0.0f);
}

Math::Vector4 Material::GetVector4(const std::string& name) const {
    auto it = m_Vector4Properties.find(name);
    if (it != m_Vector4Properties.end()) {
        return it->second;
    }
    return Math::Vector4(0.0f, 0.0f, 0.0f, 1.0f);
}

void Material::Bind() const {
    if (!m_Shader) {
        return;
    }

    m_Shader->Bind();

    // Upload legacy float properties
    for (const auto& prop : m_FloatProperties) {
        m_Shader->SetUniform1f(prop.first, prop.second);
    }

    // Upload legacy Vector3 properties
    for (const auto& prop : m_Vector3Properties) {
        m_Shader->SetUniform3f(prop.first, prop.second.x, prop.second.y, prop.second.z);
    }

    // Upload legacy Vector4 properties
    for (const auto& prop : m_Vector4Properties) {
        m_Shader->SetUniform4f(prop.first, prop.second.x, prop.second.y, prop.second.z, prop.second.w);
    }

    // Upload PBR properties
    m_Shader->SetUniform3f("u_BaseColor", m_BaseColor.x, m_BaseColor.y, m_BaseColor.z);
    m_Shader->SetUniform1f("u_Metallic", m_Metallic);
    m_Shader->SetUniform1f("u_Roughness", m_Roughness);
    m_Shader->SetUniform1f("u_Specular", m_Specular);
    m_Shader->SetUniform3f("u_EmissiveColor", m_EmissiveColor.x, m_EmissiveColor.y, m_EmissiveColor.z);
    m_Shader->SetUniform1f("u_Opacity", m_Opacity);
    
    // Advanced PBR properties
    m_Shader->SetUniform1f("u_ClearCoat", m_ClearCoat);
    m_Shader->SetUniform1f("u_ClearCoatRoughness", m_ClearCoatRoughness);
    m_Shader->SetUniform1f("u_Subsurface", m_Subsurface);
    m_Shader->SetUniform3f("u_SubsurfaceColor", m_SubsurfaceColor.x, m_SubsurfaceColor.y, m_SubsurfaceColor.z);
    m_Shader->SetUniform1f("u_IOR", m_IOR);
    m_Shader->SetUniform1f("u_Anisotropy", m_Anisotropy);
    m_Shader->SetUniform3f("u_AnisotropyDirection", m_AnisotropyDirection.x, m_AnisotropyDirection.y, m_AnisotropyDirection.z);
    m_Shader->SetUniform1f("u_Transmission", m_Transmission);

    // Bind textures to their slots
    int textureSlot = 0;
    if (m_BaseColorTexture) {
        m_BaseColorTexture->Bind(textureSlot);
        m_Shader->SetUniform1i("u_AlbedoMap", textureSlot);
        m_Shader->SetUniform1i("u_HasAlbedoMap", 1);
        textureSlot++;
    } else {
        m_Shader->SetUniform1i("u_HasAlbedoMap", 0);
    }

    if (m_MetallicTexture) {
        m_MetallicTexture->Bind(textureSlot);
        m_Shader->SetUniform1i("u_MetallicMap", textureSlot);
        m_Shader->SetUniform1i("u_HasMetallicMap", 1);
        textureSlot++;
    } else {
        m_Shader->SetUniform1i("u_HasMetallicMap", 0);
    }

    if (m_RoughnessTexture) {
        m_RoughnessTexture->Bind(textureSlot);
        m_Shader->SetUniform1i("u_RoughnessMap", textureSlot);
        m_Shader->SetUniform1i("u_HasRoughnessMap", 1);
        textureSlot++;
    } else {
        m_Shader->SetUniform1i("u_HasRoughnessMap", 0);
    }

    if (m_NormalTexture) {
        m_NormalTexture->Bind(textureSlot);
        m_Shader->SetUniform1i("u_NormalMap", textureSlot);
        m_Shader->SetUniform1i("u_HasNormalMap", 1);
        textureSlot++;
    } else {
        m_Shader->SetUniform1i("u_HasNormalMap", 0);
    }

    if (m_AOTexture) {
        m_AOTexture->Bind(textureSlot);
        m_Shader->SetUniform1i("u_AOMap", textureSlot);
        m_Shader->SetUniform1i("u_HasAOMap", 1);
        textureSlot++;
    } else {
        m_Shader->SetUniform1i("u_HasAOMap", 0);
    }

    if (m_EmissiveTexture) {
        m_EmissiveTexture->Bind(textureSlot);
        m_Shader->SetUniform1i("u_EmissiveMap", textureSlot);
        m_Shader->SetUniform1i("u_HasEmissiveMap", 1);
        textureSlot++;
    } else {
        m_Shader->SetUniform1i("u_HasEmissiveMap", 0);
    }

    if (m_OpacityTexture) {
        m_OpacityTexture->Bind(textureSlot);
        m_Shader->SetUniform1i("u_OpacityMap", textureSlot);
        m_Shader->SetUniform1i("u_HasOpacityMap", 1);
        textureSlot++;
    } else {
        m_Shader->SetUniform1i("u_HasOpacityMap", 0);
    }

    if (m_HeightTexture) {
        m_HeightTexture->Bind(textureSlot);
        m_Shader->SetUniform1i("u_HeightMap", textureSlot);
        m_Shader->SetUniform1i("u_HasHeightMap", 1);
        textureSlot++;
    } else {
        m_Shader->SetUniform1i("u_HasHeightMap", 0);
    }
}

void Material::Unbind() const {
    if (m_Shader) {
        m_Shader->Unbind();
    }
}

std::shared_ptr<Material> Material::CreateDefaultGridMaterial() {
    auto material = std::make_shared<Material>("DefaultGridMaterial");
    
    // Load grid shader
    std::string vertSource = FileSystem::ReadFile("assets/shaders/GridMaterial.vert");
    std::string fragSource = FileSystem::ReadFile("assets/shaders/GridMaterial.frag");
    
    if (vertSource.empty()) {
        Log::Error("Failed to load GridMaterial.vert shader file!");
        return nullptr;
    }
    if (fragSource.empty()) {
        Log::Error("Failed to load GridMaterial.frag shader file!");
        return nullptr;
    }
    
    auto shader = std::make_shared<Shader>(vertSource, fragSource);
    if (shader->GetRendererID() == 0) {
        Log::Error("Failed to compile GridMaterial shader!");
        return nullptr;
    }
    
    material->SetShader(shader);
    
    // Set default grid material properties (Unreal Engine style)
    material->SetColor("u_GridColor1", Math::Vector3(0.2f, 0.2f, 0.2f));  // Dark grid lines
    material->SetColor("u_GridColor2", Math::Vector3(0.4f, 0.4f, 0.4f));  // Light grid lines
    material->SetColor("u_BaseColor", Math::Vector3(0.15f, 0.15f, 0.15f)); // Base color
    material->SetFloat("u_GridSize", 1.0f);      // Grid cell size
    material->SetFloat("u_GridThickness", 0.02f); // Grid line thickness
    material->SetFloat("u_GridIntensity", 0.8f);  // Grid visibility
    
    return material;
}

std::shared_ptr<Material> Material::CreateDefaultLitMaterial() {
    auto material = std::make_shared<Material>("DefaultLitMaterial");
    
    // Load basic lit shader
    std::string vertSource = FileSystem::ReadFile("assets/shaders/Basic.vert");
    std::string fragSource = FileSystem::ReadFile("assets/shaders/Basic.frag");
    
    if (vertSource.empty()) {
        Log::Error("Failed to load Basic.vert shader file!");
        return nullptr;
    }
    if (fragSource.empty()) {
        Log::Error("Failed to load Basic.frag shader file!");
        return nullptr;
    }
    
    auto shader = std::make_shared<Shader>(vertSource, fragSource);
    if (shader->GetRendererID() == 0) {
        Log::Error("Failed to compile Basic shader!");
        return nullptr;
    }
    
    material->SetShader(shader);
    
    // Set default material color (medium gray - darker to appear gray with bright lighting)
    material->SetColor("u_MaterialColor", Math::Vector3(0.5f, 0.5f, 0.5f));
    material->SetFloat("u_UseVertexColor", 0.0f); // Use material color, not vertex color
    
    return material;
}

// PBR Property Implementations

// Base Color
void Material::SetBaseColor(const Math::Vector3& color) {
    m_BaseColor = color;
}

void Material::SetBaseColorTexture(std::shared_ptr<Texture> texture) {
    m_BaseColorTexture = texture;
    SetTexture(TextureSlot::Albedo, texture);
}

Math::Vector3 Material::GetBaseColor() const {
    return m_BaseColor;
}

std::shared_ptr<Texture> Material::GetBaseColorTexture() const {
    return m_BaseColorTexture;
}

// Metallic
void Material::SetMetallic(float value) {
    m_Metallic = value;
}

void Material::SetMetallicTexture(std::shared_ptr<Texture> texture) {
    m_MetallicTexture = texture;
    SetTexture(TextureSlot::Metallic, texture);
}

float Material::GetMetallic() const {
    return m_Metallic;
}

std::shared_ptr<Texture> Material::GetMetallicTexture() const {
    return m_MetallicTexture;
}

// Roughness
void Material::SetRoughness(float value) {
    m_Roughness = value;
}

void Material::SetRoughnessTexture(std::shared_ptr<Texture> texture) {
    m_RoughnessTexture = texture;
    SetTexture(TextureSlot::Roughness, texture);
}

float Material::GetRoughness() const {
    return m_Roughness;
}

std::shared_ptr<Texture> Material::GetRoughnessTexture() const {
    return m_RoughnessTexture;
}

// Specular
void Material::SetSpecular(float value) {
    m_Specular = value;
}

float Material::GetSpecular() const {
    return m_Specular;
}

// Normal Map
void Material::SetNormalTexture(std::shared_ptr<Texture> texture) {
    m_NormalTexture = texture;
    SetTexture(TextureSlot::Normal, texture);
}

std::shared_ptr<Texture> Material::GetNormalTexture() const {
    return m_NormalTexture;
}

// Ambient Occlusion
void Material::SetAOTexture(std::shared_ptr<Texture> texture) {
    m_AOTexture = texture;
    SetTexture(TextureSlot::AmbientOcclusion, texture);
}

std::shared_ptr<Texture> Material::GetAOTexture() const {
    return m_AOTexture;
}

// Emissive
void Material::SetEmissiveColor(const Math::Vector3& color) {
    m_EmissiveColor = color;
}

void Material::SetEmissiveTexture(std::shared_ptr<Texture> texture) {
    m_EmissiveTexture = texture;
    SetTexture(TextureSlot::Emissive, texture);
}

Math::Vector3 Material::GetEmissiveColor() const {
    return m_EmissiveColor;
}

std::shared_ptr<Texture> Material::GetEmissiveTexture() const {
    return m_EmissiveTexture;
}

// Opacity
void Material::SetOpacity(float value) {
    m_Opacity = value;
}

void Material::SetOpacityTexture(std::shared_ptr<Texture> texture) {
    m_OpacityTexture = texture;
    SetTexture(TextureSlot::Opacity, texture);
}

float Material::GetOpacity() const {
    return m_Opacity;
}

std::shared_ptr<Texture> Material::GetOpacityTexture() const {
    return m_OpacityTexture;
}

// Advanced PBR Properties
void Material::SetClearCoat(float value) {
    m_ClearCoat = value;
}

void Material::SetClearCoatRoughness(float value) {
    m_ClearCoatRoughness = value;
}

float Material::GetClearCoat() const {
    return m_ClearCoat;
}

float Material::GetClearCoatRoughness() const {
    return m_ClearCoatRoughness;
}

void Material::SetSubsurface(float value) {
    m_Subsurface = value;
}

void Material::SetSubsurfaceColor(const Math::Vector3& color) {
    m_SubsurfaceColor = color;
}

float Material::GetSubsurface() const {
    return m_Subsurface;
}

Math::Vector3 Material::GetSubsurfaceColor() const {
    return m_SubsurfaceColor;
}

void Material::SetIOR(float value) {
    m_IOR = value;
}

float Material::GetIOR() const {
    return m_IOR;
}

void Material::SetAnisotropy(float value) {
    m_Anisotropy = value;
}

void Material::SetAnisotropyDirection(const Math::Vector3& direction) {
    m_AnisotropyDirection = direction;
}

float Material::GetAnisotropy() const {
    return m_Anisotropy;
}

Math::Vector3 Material::GetAnisotropyDirection() const {
    return m_AnisotropyDirection;
}

void Material::SetTransmission(float value) {
    m_Transmission = value;
}

float Material::GetTransmission() const {
    return m_Transmission;
}

// Height / Displacement
void Material::SetHeightTexture(std::shared_ptr<Texture> texture) {
    m_HeightTexture = texture;
    SetTexture(TextureSlot::Height, texture);
}

std::shared_ptr<Texture> Material::GetHeightTexture() const {
    return m_HeightTexture;
}

// Texture slot management
void Material::SetTexture(TextureSlot slot, std::shared_ptr<Texture> texture) {
    int slotIndex = static_cast<int>(slot);
    if (slotIndex >= 0 && slotIndex < static_cast<int>(TextureSlot::MaxSlots)) {
        m_TextureSlots[slotIndex] = texture;
    }
}

std::shared_ptr<Texture> Material::GetTexture(TextureSlot slot) const {
    int slotIndex = static_cast<int>(slot);
    if (slotIndex >= 0 && slotIndex < static_cast<int>(TextureSlot::MaxSlots)) {
        return m_TextureSlots[slotIndex];
    }
    return nullptr;
}

// Default PBR Material factory
std::shared_ptr<Material> Material::CreateDefaultPBRMaterial() {
    auto material = std::make_shared<Material>("DefaultPBRMaterial");
    
    // Load PBR shader
    std::string vertSource = FileSystem::ReadFile("assets/shaders/PBR.vert");
    std::string fragSource = FileSystem::ReadFile("assets/shaders/PBR.frag");
    
    if (vertSource.empty()) {
        Log::Error("Failed to load PBR.vert shader file!");
        return nullptr;
    }
    if (fragSource.empty()) {
        Log::Error("Failed to load PBR.frag shader file!");
        return nullptr;
    }
    
    auto shader = std::make_shared<Shader>(vertSource, fragSource);
    if (shader->GetRendererID() == 0) {
        Log::Error("Failed to compile PBR shader!");
        return nullptr;
    }
    
    material->SetShader(shader);
    
    // Set default PBR values (medium gray, non-metallic, medium roughness)
    material->SetBaseColor(Math::Vector3(0.5f, 0.5f, 0.5f));
    material->SetMetallic(0.0f);
    material->SetRoughness(0.5f);
    material->SetSpecular(0.5f);
    material->SetEmissiveColor(Math::Vector3(0.0f, 0.0f, 0.0f));
    material->SetOpacity(1.0f);
    
    return material;
}

} // namespace LGE

