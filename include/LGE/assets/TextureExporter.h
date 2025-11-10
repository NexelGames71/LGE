#pragma once

#include "LGE/assets/Exporter.h"
#include <string>
#include <vector>

namespace LGE {

// Texture exporter - exports textures to various formats
class TextureExporter : public Exporter {
public:
    TextureExporter();
    virtual ~TextureExporter() = default;
    
    bool Export(const std::string& assetPath, const std::string& outputPath, const LxAssetMeta& meta) override;
    std::vector<std::string> GetSupportedFormats() const override;
    std::string GetName() const override { return "TextureExporter"; }
    bool CanExport(AssetType type) const override;
};

} // namespace LGE

