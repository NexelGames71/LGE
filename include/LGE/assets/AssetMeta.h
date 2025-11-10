#pragma once

#include <string>
#include <cstdint>

namespace LGE {

// Asset types
enum class AssetType {
    Unknown,
    Texture2D,
    TextureCube,
    Model,
    Material,
    Shader,
    Script,
    Scene,
    Sound,
    Animation,
    Blueprint,
    Folder
};

// Asset metadata structure
struct LxAssetMeta {
    std::string name;
    std::string path;           // Full path to the asset file
    std::string relativePath;   // Path relative to Assets folder
    AssetType type;
    uint64_t importTime;        // Unix timestamp when imported
    uint64_t modifyTime;        // Unix timestamp of last modification
    std::string importer;       // Name of the importer used
    std::string sourceFile;     // Original source file path (if imported)
    std::string guid;           // Unique identifier for the asset
    
    LxAssetMeta()
        : type(AssetType::Unknown)
        , importTime(0)
        , modifyTime(0)
    {
    }
};

// Helper functions
std::string AssetTypeToString(AssetType type);
AssetType StringToAssetType(const std::string& str);
AssetType GetAssetTypeFromExtension(const std::string& extension);

} // namespace LGE

