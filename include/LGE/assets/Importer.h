#pragma once

#include "LGE/assets/AssetMeta.h"
#include <string>
#include <vector>
#include <memory>

namespace LGE {

// Forward declaration
class LxAssetMeta;

// Base importer interface
class Importer {
public:
    virtual ~Importer() = default;
    
    // Import a file and return metadata
    virtual bool Import(const std::string& sourcePath, const std::string& outputPath, LxAssetMeta& meta) = 0;
    
    // Reimport an existing asset
    virtual bool Reimport(const std::string& assetPath, LxAssetMeta& meta) = 0;
    
    // Get supported file extensions
    virtual std::vector<std::string> GetSupportedExtensions() const = 0;
    
    // Get importer name
    virtual std::string GetName() const = 0;
    
    // Check if this importer can handle the given file
    virtual bool CanImport(const std::string& filePath) const = 0;
};

} // namespace LGE

