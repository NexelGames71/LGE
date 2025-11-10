#pragma once

#include "LGE/assets/AssetMeta.h"
#include <string>
#include <memory>

namespace LGE {

// Asset factory - creates new engine-native assets
class AssetFactory {
public:
    AssetFactory();
    ~AssetFactory();
    
    // Create a new asset of the specified type
    bool CreateAsset(AssetType type, const std::string& path, const std::string& name = "");
    
    // Create a new material
    bool CreateMaterial(const std::string& path, const std::string& name = "");
    
    // Create a new shader
    bool CreateShader(const std::string& path, const std::string& name = "");
    
    // Create a new script
    bool CreateScript(const std::string& path, const std::string& name = "");
    
    // Create a new scene
    bool CreateScene(const std::string& path, const std::string& name = "");
    
    // Create a new folder
    bool CreateFolder(const std::string& path, const std::string& name = "");
    
private:
    // Helper function to create asset metadata
    LxAssetMeta CreateAssetMeta(AssetType type, const std::string& path, const std::string& name);
    
    // Helper function to generate default content for asset type
    std::string GenerateDefaultContent(AssetType type, const std::string& name);
    
    // Helper function to generate unique filename if file exists
    std::string GenerateUniqueFilename(const std::string& path);
};

} // namespace LGE

