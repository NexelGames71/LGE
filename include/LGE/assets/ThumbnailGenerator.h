#pragma once

#include "LGE/assets/AssetMeta.h"
#include <string>
#include <memory>

namespace LGE {

class Texture;

// Thumbnail generator - creates and caches thumbnails for assets
class ThumbnailGenerator {
public:
    ThumbnailGenerator();
    ~ThumbnailGenerator();
    
    // Generate thumbnail for an asset
    std::shared_ptr<Texture> GenerateThumbnail(const std::string& assetPath, const LxAssetMeta& meta);
    
    // Get cached thumbnail path
    std::string GetThumbnailPath(const std::string& assetPath) const;
    
    // Check if thumbnail exists
    bool ThumbnailExists(const std::string& assetPath) const;
    
    // Delete thumbnail
    bool DeleteThumbnail(const std::string& assetPath);
    
    // Set thumbnail cache directory
    void SetCacheDirectory(const std::string& cacheDir);
    
    // Get thumbnail cache directory
    std::string GetCacheDirectory() const { return m_CacheDirectory; }
    
    // Set thumbnail size
    void SetThumbnailSize(uint32_t width, uint32_t height);
    
private:
    // Generate thumbnail for texture
    std::shared_ptr<Texture> GenerateTextureThumbnail(const std::string& assetPath);
    
    // Generate thumbnail for model (placeholder)
    std::shared_ptr<Texture> GenerateModelThumbnail(const std::string& assetPath);
    
    // Generate thumbnail for material (renders sphere with material)
    std::shared_ptr<Texture> GenerateMaterialThumbnail(const std::string& assetPath, const LxAssetMeta& meta);
    
    // Generate default thumbnail based on asset type
    std::shared_ptr<Texture> GenerateDefaultThumbnail(AssetType type);
    
    // Save thumbnail to cache
    bool SaveThumbnailToCache(const std::string& assetPath, std::shared_ptr<Texture> thumbnail);
    
    // Load thumbnail from cache
    std::shared_ptr<Texture> LoadThumbnailFromCache(const std::string& assetPath);
    
    std::string m_CacheDirectory;
    uint32_t m_ThumbnailWidth;
    uint32_t m_ThumbnailHeight;
};

} // namespace LGE

