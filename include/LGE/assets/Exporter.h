#pragma once

#include "LGE/assets/AssetMeta.h"
#include <string>
#include <vector>
#include <memory>

namespace LGE {

// Base exporter interface
class Exporter {
public:
    virtual ~Exporter() = default;
    
    // Export an asset to a specific format
    virtual bool Export(const std::string& assetPath, const std::string& outputPath, const LxAssetMeta& meta) = 0;
    
    // Get supported export formats
    virtual std::vector<std::string> GetSupportedFormats() const = 0;
    
    // Get exporter name
    virtual std::string GetName() const = 0;
    
    // Check if this exporter can handle the given asset type
    virtual bool CanExport(AssetType type) const = 0;
};

} // namespace LGE

