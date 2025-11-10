#include "LGE/assets/TextureImporter.h"
#include "LGE/assets/AssetMeta.h"
#include "LGE/core/Log.h"
#include <filesystem>
#include <chrono>
#include <ctime>

namespace LGE {

TextureImporter::TextureImporter() {
}

bool TextureImporter::Import(const std::string& sourcePath, const std::string& outputPath, LxAssetMeta& meta) {
    if (!std::filesystem::exists(sourcePath)) {
        Log::Error("TextureImporter: Source file does not exist: " + sourcePath);
        return false;
    }
    
    try {
        // Create output directory if it doesn't exist
        std::filesystem::path outputDir = std::filesystem::path(outputPath).parent_path();
        if (!outputDir.empty() && !std::filesystem::exists(outputDir)) {
            std::filesystem::create_directories(outputDir);
        }
        
        // For now, just copy the file (in the future, this would convert to .ltex format)
        std::filesystem::copy_file(sourcePath, outputPath, std::filesystem::copy_options::overwrite_existing);
        
        // Set metadata
        meta.name = std::filesystem::path(outputPath).stem().string();
        meta.path = outputPath;
        meta.type = AssetType::Texture2D;
        meta.importer = GetName();
        meta.sourceFile = sourcePath;
        
        // Set timestamps
        auto now = std::chrono::system_clock::now();
        meta.importTime = std::chrono::system_clock::to_time_t(now);
        meta.modifyTime = std::chrono::system_clock::to_time_t(now);
        
        // Generate GUID (simple hash for now)
        meta.guid = std::to_string(std::hash<std::string>{}(outputPath));
        
        Log::Info("TextureImporter: Imported texture: " + sourcePath + " -> " + outputPath);
        return true;
        
    } catch (const std::exception& e) {
        Log::Error("TextureImporter: Failed to import texture: " + std::string(e.what()));
        return false;
    }
}

bool TextureImporter::Reimport(const std::string& assetPath, LxAssetMeta& meta) {
    if (meta.sourceFile.empty()) {
        Log::Error("TextureImporter: Cannot reimport - no source file specified");
        return false;
    }
    
    return Import(meta.sourceFile, assetPath, meta);
}

std::vector<std::string> TextureImporter::GetSupportedExtensions() const {
    return {".png", ".jpg", ".jpeg", ".tga", ".bmp", ".dds", ".hdr", ".exr"};
}

bool TextureImporter::CanImport(const std::string& filePath) const {
    std::filesystem::path path(filePath);
    std::string ext = path.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    
    auto extensions = GetSupportedExtensions();
    return std::find(extensions.begin(), extensions.end(), ext) != extensions.end();
}

} // namespace LGE

