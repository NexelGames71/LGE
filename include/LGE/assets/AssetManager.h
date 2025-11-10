#pragma once

#include "LGE/assets/AssetMeta.h"
#include "LGE/assets/FileSystemWatcher.h"
#include "LGE/assets/ImporterRegistry.h"
#include "LGE/assets/AssetMetadata.h"
#include "LGE/assets/ThumbnailGenerator.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>

namespace LGE {

class Texture;

// Asset manager - central manager for all assets
class AssetManager {
public:
    AssetManager();
    ~AssetManager();
    
    // Initialize asset manager
    bool Initialize(const std::string& assetsDirectory);
    
    // Shutdown asset manager
    void Shutdown();
    
    // Scan assets directory and build asset tree
    void ScanAssets();
    
    // Import a file
    bool ImportFile(const std::string& sourcePath, const std::string& outputPath = "");
    
    // Reimport an asset
    bool ReimportAsset(const std::string& assetPath);
    
    // Get asset metadata
    LxAssetMeta* GetAssetMeta(const std::string& assetPath);
    
    // Get all assets in a directory
    std::vector<LxAssetMeta*> GetAssetsInDirectory(const std::string& directory);
    
    // Get thumbnail for an asset
    std::shared_ptr<Texture> GetThumbnail(const std::string& assetPath);
    
    // Get assets directory
    std::string GetAssetsDirectory() const { return m_AssetsDirectory; }
    
    // Set callback for asset changes
    void SetAssetChangeCallback(std::function<void(const FileSystemEvent&)> callback);
    
    // Update (called from main thread)
    void Update();
    
private:
    void OnFileSystemEvent(const FileSystemEvent& event);
    void ProcessFile(const std::string& filePath);
    void RemoveAsset(const std::string& filePath);
    std::string GetRelativePath(const std::string& fullPath) const;
    
    std::string m_AssetsDirectory;
    std::unique_ptr<FileSystemWatcher> m_FileWatcher;
    std::unique_ptr<ThumbnailGenerator> m_ThumbnailGenerator;
    
    // Asset storage
    std::unordered_map<std::string, LxAssetMeta> m_Assets;
    std::mutex m_AssetsMutex;
    
    // Callback for asset changes
    std::function<void(const FileSystemEvent&)> m_AssetChangeCallback;
};

} // namespace LGE

