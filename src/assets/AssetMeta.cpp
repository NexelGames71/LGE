#include "LGE/assets/AssetMeta.h"
#include <algorithm>
#include <cctype>

namespace LGE {

std::string AssetTypeToString(AssetType type) {
    switch (type) {
        case AssetType::Texture2D: return "Texture2D";
        case AssetType::TextureCube: return "TextureCube";
        case AssetType::Model: return "Model";
        case AssetType::Material: return "Material";
        case AssetType::Shader: return "Shader";
        case AssetType::Script: return "Script";
        case AssetType::Scene: return "Scene";
        case AssetType::Sound: return "Sound";
        case AssetType::Animation: return "Animation";
        case AssetType::Blueprint: return "Blueprint";
        case AssetType::Folder: return "Folder";
        default: return "Unknown";
    }
}

AssetType StringToAssetType(const std::string& str) {
    if (str == "Texture2D") return AssetType::Texture2D;
    if (str == "TextureCube") return AssetType::TextureCube;
    if (str == "Model") return AssetType::Model;
    if (str == "Material") return AssetType::Material;
    if (str == "Shader") return AssetType::Shader;
    if (str == "Script") return AssetType::Script;
    if (str == "Scene") return AssetType::Scene;
    if (str == "Sound") return AssetType::Sound;
    if (str == "Animation") return AssetType::Animation;
    if (str == "Blueprint") return AssetType::Blueprint;
    if (str == "Folder") return AssetType::Folder;
    return AssetType::Unknown;
}

AssetType GetAssetTypeFromExtension(const std::string& extension) {
    std::string ext = extension;
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    
    // Texture formats
    if (ext == ".png" || ext == ".jpg" || ext == ".jpeg" || ext == ".tga" || 
        ext == ".bmp" || ext == ".dds" || ext == ".hdr" || ext == ".exr") {
        return AssetType::Texture2D;
    }
    
    // Model formats
    if (ext == ".fbx" || ext == ".obj" || ext == ".dae" || ext == ".gltf" || 
        ext == ".glb" || ext == ".3ds" || ext == ".blend") {
        return AssetType::Model;
    }
    
    // Shader formats
    if (ext == ".glsl" || ext == ".hlsl" || ext == ".vert" || ext == ".frag" || 
        ext == ".comp" || ext == ".shader") {
        return AssetType::Shader;
    }
    
    // Material format
    if (ext == ".material" || ext == ".mat") {
        return AssetType::Material;
    }
    
    // Script formats
    if (ext == ".cs" || ext == ".cpp" || ext == ".h" || ext == ".hpp") {
        return AssetType::Script;
    }
    
    // Scene format
    if (ext == ".scene" || ext == ".lscene") {
        return AssetType::Scene;
    }
    
    // Sound formats
    if (ext == ".wav" || ext == ".ogg" || ext == ".mp3" || ext == ".flac") {
        return AssetType::Sound;
    }
    
    return AssetType::Unknown;
}

} // namespace LGE

