#pragma once

#include "LGE/assets/Importer.h"
#include <string>
#include <vector>

namespace LGE {

// Texture importer
class TextureImporter : public Importer {
public:
    TextureImporter();
    virtual ~TextureImporter() = default;
    
    bool Import(const std::string& sourcePath, const std::string& outputPath, LxAssetMeta& meta) override;
    bool Reimport(const std::string& assetPath, LxAssetMeta& meta) override;
    std::vector<std::string> GetSupportedExtensions() const override;
    std::string GetName() const override { return "TextureImporter"; }
    bool CanImport(const std::string& filePath) const override;
};

} // namespace LGE

