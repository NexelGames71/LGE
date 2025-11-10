#pragma once

#include "LGE/assets/AssetMeta.h"
#include <string>

namespace LGE {

// Asset metadata manager - handles .lumameta files
class AssetMetadata {
public:
    // Save metadata to .lumameta file
    static bool SaveMetadata(const std::string& assetPath, const LxAssetMeta& meta);
    
    // Load metadata from .lumameta file
    static bool LoadMetadata(const std::string& assetPath, LxAssetMeta& meta);
    
    // Get .lumameta file path for an asset
    static std::string GetMetadataPath(const std::string& assetPath);
    
    // Check if metadata file exists
    static bool MetadataExists(const std::string& assetPath);
    
    // Delete metadata file
    static bool DeleteMetadata(const std::string& assetPath);
    
private:
    // Helper functions for JSON serialization/deserialization
    static std::string SerializeToJSON(const LxAssetMeta& meta);
    static bool DeserializeFromJSON(const std::string& json, LxAssetMeta& meta);
};

} // namespace LGE

