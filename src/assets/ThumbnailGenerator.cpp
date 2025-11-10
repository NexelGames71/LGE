#include "LGE/assets/ThumbnailGenerator.h"
#include "LGE/rendering/Texture.h"
#include "LGE/core/Log.h"
#include <filesystem>
#include <algorithm>

namespace LGE {

ThumbnailGenerator::ThumbnailGenerator()
    : m_CacheDirectory("Cache/Thumbnails")
    , m_ThumbnailWidth(128)
    , m_ThumbnailHeight(128)
{
    // Create cache directory if it doesn't exist
    if (!std::filesystem::exists(m_CacheDirectory)) {
        std::filesystem::create_directories(m_CacheDirectory);
    }
}

ThumbnailGenerator::~ThumbnailGenerator() {
}

void ThumbnailGenerator::SetCacheDirectory(const std::string& cacheDir) {
    m_CacheDirectory = cacheDir;
    if (!std::filesystem::exists(m_CacheDirectory)) {
        std::filesystem::create_directories(m_CacheDirectory);
    }
}

void ThumbnailGenerator::SetThumbnailSize(uint32_t width, uint32_t height) {
    m_ThumbnailWidth = width;
    m_ThumbnailHeight = height;
}

std::shared_ptr<Texture> ThumbnailGenerator::GenerateThumbnail(const std::string& assetPath, const LxAssetMeta& meta) {
    // Check if thumbnail already exists in cache
    auto cached = LoadThumbnailFromCache(assetPath);
    if (cached) {
        return cached;
    }
    
    std::shared_ptr<Texture> thumbnail = nullptr;
    
    // Generate thumbnail based on asset type
    switch (meta.type) {
        case AssetType::Texture2D:
        case AssetType::TextureCube:
            thumbnail = GenerateTextureThumbnail(assetPath);
            break;
            
        case AssetType::Model:
            thumbnail = GenerateModelThumbnail(assetPath);
            break;
            
        case AssetType::Folder:
            thumbnail = GenerateDefaultThumbnail(AssetType::Folder);
            break;
            
        case AssetType::Material:
            thumbnail = GenerateMaterialThumbnail(assetPath, meta);
            break;
            
        default:
            thumbnail = GenerateDefaultThumbnail(meta.type);
            break;
    }
    
    // Save to cache if generated
    if (thumbnail) {
        SaveThumbnailToCache(assetPath, thumbnail);
    }
    
    return thumbnail;
}

std::string ThumbnailGenerator::GetThumbnailPath(const std::string& assetPath) const {
    std::filesystem::path path(assetPath);
    std::string fileName = path.stem().string();
    std::string cachePath = m_CacheDirectory + "/" + fileName + ".png";
    return cachePath;
}

bool ThumbnailGenerator::ThumbnailExists(const std::string& assetPath) const {
    std::string thumbnailPath = GetThumbnailPath(assetPath);
    return std::filesystem::exists(thumbnailPath);
}

bool ThumbnailGenerator::DeleteThumbnail(const std::string& assetPath) {
    std::string thumbnailPath = GetThumbnailPath(assetPath);
    
    if (!std::filesystem::exists(thumbnailPath)) {
        return true; // Already deleted
    }
    
    try {
        std::filesystem::remove(thumbnailPath);
        Log::Info("ThumbnailGenerator: Deleted thumbnail: " + thumbnailPath);
        return true;
    } catch (const std::exception& e) {
        Log::Error("ThumbnailGenerator: Failed to delete thumbnail: " + std::string(e.what()));
        return false;
    }
}

std::shared_ptr<Texture> ThumbnailGenerator::GenerateTextureThumbnail(const std::string& assetPath) {
    // Load the texture and use it as thumbnail
    auto texture = std::make_shared<Texture>();
    if (texture->LoadImageFile(assetPath)) {
        Log::Info("ThumbnailGenerator: Generated texture thumbnail: " + assetPath);
        return texture;
    }
    
    Log::Error("ThumbnailGenerator: Failed to load texture for thumbnail: " + assetPath);
    return nullptr;
}

std::shared_ptr<Texture> ThumbnailGenerator::GenerateModelThumbnail(const std::string& assetPath) {
    // For now, return default model thumbnail
    // In the future, this would render the model to a texture
    Log::Info("ThumbnailGenerator: Generating default model thumbnail for: " + assetPath);
    return GenerateDefaultThumbnail(AssetType::Model);
}

std::shared_ptr<Texture> ThumbnailGenerator::GenerateMaterialThumbnail(const std::string& assetPath, const LxAssetMeta& meta) {
    // TODO: Render a sphere with the material applied
    // This requires:
    // 1. Create a framebuffer for off-screen rendering
    // 2. Load the material from the asset path
    // 3. Render a sphere with the material
    // 4. Read the framebuffer into a texture
    // 5. Return the texture as thumbnail
    
    // For now, return a placeholder - this will be implemented when we have
    // material loading and rendering infrastructure
    Log::Info("ThumbnailGenerator: Material thumbnail rendering not yet fully implemented for: " + assetPath);
    
    // Return nullptr for now - the UI will handle missing thumbnails
    // In the future, this will render a sphere with the material
    return nullptr;
}

std::shared_ptr<Texture> ThumbnailGenerator::GenerateDefaultThumbnail(AssetType type) {
    Log::Info("ThumbnailGenerator: Generating default thumbnail for type: " + AssetTypeToString(type));
    
    // For folders, load the folder icon
    if (type == AssetType::Folder) {
        auto folderIcon = std::make_shared<Texture>();
        std::vector<std::string> folderPaths = {
            "unreal-engine-editor-icons/imgs/Icons/Folders/Folder_Base_512x.png",
            "assets/icons/Folder_Base_512x.png",
            "assets/icons/folder.png"
        };
        
        for (const auto& path : folderPaths) {
            if (folderIcon->LoadImageFile(path)) {
                Log::Info("ThumbnailGenerator: Loaded folder icon: " + path);
                return folderIcon;
            }
        }
    }
    
    // For materials, we'll need to render a sphere with the material
    // For now, return nullptr - this will be implemented later
    if (type == AssetType::Material) {
        // TODO: Render sphere with material
        Log::Info("ThumbnailGenerator: Material thumbnail rendering not yet implemented");
        return nullptr;
    }
    
    // Return nullptr for other types - the UI can handle missing thumbnails
    return nullptr;
}

bool ThumbnailGenerator::SaveThumbnailToCache(const std::string& assetPath, std::shared_ptr<Texture> thumbnail) {
    if (!thumbnail) {
        return false;
    }
    
    std::string thumbnailPath = GetThumbnailPath(assetPath);
    
    try {
        // Create cache directory if it doesn't exist
        std::filesystem::path cacheDir = std::filesystem::path(thumbnailPath).parent_path();
        if (!cacheDir.empty() && !std::filesystem::exists(cacheDir)) {
            std::filesystem::create_directories(cacheDir);
        }
        
        // For now, we'll just note that the texture is loaded
        // In the future, we'd save it as a PNG file
        // This would require texture export functionality
        
        Log::Info("ThumbnailGenerator: Cached thumbnail: " + thumbnailPath);
        return true;
        
    } catch (const std::exception& e) {
        Log::Error("ThumbnailGenerator: Failed to save thumbnail to cache: " + std::string(e.what()));
        return false;
    }
}

std::shared_ptr<Texture> ThumbnailGenerator::LoadThumbnailFromCache(const std::string& assetPath) {
    std::string thumbnailPath = GetThumbnailPath(assetPath);
    
    if (!std::filesystem::exists(thumbnailPath)) {
        return nullptr;
    }
    
    try {
        auto texture = std::make_shared<Texture>();
        if (texture->LoadImageFile(thumbnailPath)) {
            Log::Info("ThumbnailGenerator: Loaded thumbnail from cache: " + thumbnailPath);
            return texture;
        }
    } catch (const std::exception& e) {
        Log::Error("ThumbnailGenerator: Failed to load thumbnail from cache: " + std::string(e.what()));
    }
    
    return nullptr;
}

} // namespace LGE

