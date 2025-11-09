#version 330 core

in vec3 v_Color;
in vec3 v_Normal;
in vec3 v_FragPos;
in vec2 v_TexCoord;
in vec3 v_Tangent;
in vec3 v_ViewDir;

out vec4 FragColor;

// Material Properties
uniform vec3 u_BaseColor;
uniform float u_Metallic;
uniform float u_Roughness;
uniform float u_Specular;
uniform vec3 u_EmissiveColor;
uniform float u_Opacity;

// Advanced PBR Properties
uniform float u_ClearCoat;
uniform float u_ClearCoatRoughness;
uniform float u_Subsurface;
uniform vec3 u_SubsurfaceColor;
uniform float u_IOR;
uniform float u_Anisotropy;
uniform vec3 u_AnisotropyDirection;
uniform float u_Transmission;

// Texture Maps
uniform sampler2D u_AlbedoMap;
uniform sampler2D u_MetallicMap;
uniform sampler2D u_RoughnessMap;
uniform sampler2D u_NormalMap;
uniform sampler2D u_AOMap;
uniform sampler2D u_EmissiveMap;
uniform sampler2D u_OpacityMap;
uniform sampler2D u_HeightMap;

// Texture flags
uniform int u_HasAlbedoMap;
uniform int u_HasMetallicMap;
uniform int u_HasRoughnessMap;
uniform int u_HasNormalMap;
uniform int u_HasAOMap;
uniform int u_HasEmissiveMap;
uniform int u_HasOpacityMap;
uniform int u_HasHeightMap;

// Lighting
uniform vec3 u_LightDirection;
uniform vec3 u_LightColor;
uniform float u_LightIntensity;
uniform vec3 u_ViewPos;

// Constants
const float PI = 3.14159265359;

// Cook-Torrance BRDF Functions

// Normal Distribution Function (GGX/Trowbridge-Reitz)
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / max(denom, 0.0000001);
}

// Geometry Function (Schlick-GGX)
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / max(denom, 0.0000001);
}

// Geometry Function (Smith)
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

// Fresnel-Schlick approximation
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

// Calculate normal from normal map
vec3 CalculateNormal()
{
    vec3 normal = v_Normal;
    
    if (u_HasNormalMap > 0)
    {
        // Sample normal map
        vec3 normalMap = texture(u_NormalMap, v_TexCoord).rgb;
        normalMap = normalMap * 2.0 - 1.0; // Transform from [0,1] to [-1,1]
        
        // Calculate bitangent
        vec3 bitangent = cross(v_Normal, v_Tangent);
        
        // Create TBN matrix
        mat3 TBN = mat3(normalize(v_Tangent), normalize(bitangent), normalize(v_Normal));
        
        // Transform normal from tangent space to world space
        normal = normalize(TBN * normalMap);
    }
    
    return normal;
}

void main()
{
    // Sample textures
    vec3 albedo = u_BaseColor;
    if (u_HasAlbedoMap > 0)
    {
        albedo = texture(u_AlbedoMap, v_TexCoord).rgb;
    }
    
    float metallic = u_Metallic;
    if (u_HasMetallicMap > 0)
    {
        metallic = texture(u_MetallicMap, v_TexCoord).r;
    }
    
    float roughness = u_Roughness;
    if (u_HasRoughnessMap > 0)
    {
        roughness = texture(u_RoughnessMap, v_TexCoord).r;
    }
    
    float ao = 1.0;
    if (u_HasAOMap > 0)
    {
        ao = texture(u_AOMap, v_TexCoord).r;
    }
    
    vec3 emissive = u_EmissiveColor;
    if (u_HasEmissiveMap > 0)
    {
        emissive += texture(u_EmissiveMap, v_TexCoord).rgb;
    }
    
    float opacity = u_Opacity;
    if (u_HasOpacityMap > 0)
    {
        opacity *= texture(u_OpacityMap, v_TexCoord).r;
    }
    
    // Calculate normal (with normal mapping if available)
    vec3 N = CalculateNormal();
    vec3 V = normalize(v_ViewDir);
    vec3 L = normalize(-u_LightDirection);
    vec3 H = normalize(V + L);
    
    // Calculate F0 (base specular reflectance)
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);
    
    // Cook-Torrance BRDF
    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);
    
    // Specular component
    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular = numerator / denominator;
    
    // Diffuse component
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic; // Metals have no diffuse component
    
    float NdotL = max(dot(N, L), 0.0);
    vec3 radiance = u_LightColor * u_LightIntensity;
    
    // Combine diffuse and specular
    vec3 Lo = (kD * albedo / PI + specular) * radiance * NdotL;
    
    // Ambient lighting
    vec3 ambient = vec3(0.03) * albedo * ao;
    
    // Final color
    vec3 color = ambient + Lo + emissive;
    
    // Apply tone mapping (simple Reinhard)
    color = color / (color + vec3(1.0));
    
    // Gamma correction
    color = pow(color, vec3(1.0/2.2));
    
    FragColor = vec4(color, opacity);
}

