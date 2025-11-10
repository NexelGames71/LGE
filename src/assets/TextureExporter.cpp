#include "LGE/assets/TextureExporter.h"
#include "LGE/assets/AssetMeta.h"
#include "LGE/core/Log.h"
#include <filesystem>

namespace LGE {

TextureExporter::TextureExporter() {
}

bool TextureExporter::Export(const std::string& assetPath, const std::string& outputPath, const LxAssetMeta& meta) {
    if (!std::filesystem::exists(assetPath)) {
        Log::Error("TextureExporter: Asset file does not exist: " + assetPath);
        return false;
    }
    
    try {
        // Create output directory if it doesn't exist
        std::filesystem::path outputDir = std::filesystem::path(outputPath).parent_path();
        if (!outputDir.empty() && !std::filesystem::exists(outputDir)) {
            std::filesystem::create_directories(outputDir);
        }
        
        // For now, just copy the file (in the future, this would convert formats)
        std::filesystem::copy_file(assetPath, outputPath, std::filesystem::copy_options::overwrite_existing);
        
        Log::Info("TextureExporter: Exported texture: " + assetPath + " -> " + outputPath);
        return true;
        
    } catch (const std::exception& e) {
        Log::Error("TextureExporter: Failed to export texture: " + std::string(e.what()));
        return false;
    }
}

std::vector<std::string> TextureExporter::GetSupportedFormats() const {
    return {".png", ".jpg", ".jpeg", ".tga", ".bmp", ".dds"};
}

bool TextureExporter::CanExport(AssetType type) const {
    return type == AssetType::Texture2D || type == AssetType::TextureCube;
}

} // namespace LGE

