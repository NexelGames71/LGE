#include "LGE/assets/AssetManager.h"
#include "LGE/rendering/Texture.h"
#include "LGE/core/Log.h"
#include <filesystem>
#include <algorithm>
#include <mutex>
#include <chrono>

namespace LGE {

AssetManager::AssetManager()
    : m_AssetsDirectory("Assets")
{
    m_FileWatcher = std::make_unique<FileSystemWatcher>();
    m_ThumbnailGenerator = std::make_unique<ThumbnailGenerator>();
}

AssetManager::~AssetManager() {
    Shutdown();
}

bool AssetManager::Initialize(const std::string& assetsDirectory) {
    m_AssetsDirectory = assetsDirectory;
    
    // Create assets directory if it doesn't exist
    if (!std::filesystem::exists(m_AssetsDirectory)) {
        std::filesystem::create_directories(m_AssetsDirectory);
        Log::Info("AssetManager: Created assets directory: " + m_AssetsDirectory);
    }
    
    // Set up file watcher
    m_FileWatcher->SetEventCallback([this](const FileSystemEvent& event) {
        OnFileSystemEvent(event);
    });
    
    if (!m_FileWatcher->WatchDirectory(m_AssetsDirectory)) {
        Log::Error("AssetManager: Failed to start file watcher");
        return false;
    }
    
    // Set thumbnail cache directory
    m_ThumbnailGenerator->SetCacheDirectory(m_AssetsDirectory + "/Cache/Thumbnails");
    
    // Scan existing assets
    ScanAssets();
    
    Log::Info("AssetManager: Initialized with assets directory: " + m_AssetsDirectory);
    return true;
}

void AssetManager::Shutdown() {
    if (m_FileWatcher && m_FileWatcher->IsWatching()) {
        m_FileWatcher->StopWatching();
    }
    
    std::lock_guard<std::mutex> lock(m_AssetsMutex);
    m_Assets.clear();
}

void AssetManager::Update() {
    // Update file watcher
    if (m_FileWatcher) {
        m_FileWatcher->Update();
    }
}

void AssetManager::ScanAssets() {
    std::lock_guard<std::mutex> lock(m_AssetsMutex);
    m_Assets.clear();
    
    try {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(m_AssetsDirectory)) {
            if (entry.is_regular_file()) {
                std::string filePath = entry.path().string();
                
                // Skip metadata files
                if (filePath.find(".lumameta") != std::string::npos) {
                    continue;
                }
                
                // Skip cache files
                if (filePath.find("/Cache/") != std::string::npos ||
                    filePath.find("\\Cache\\") != std::string::npos) {
                    continue;
                }
                
                ProcessFile(filePath);
            }
        }
        
        Log::Info("AssetManager: Scanned " + std::to_string(m_Assets.size()) + " assets");
    } catch (const std::exception& e) {
        Log::Error("AssetManager: Error scanning assets: " + std::string(e.what()));
    }
}

bool AssetManager::ImportFile(const std::string& sourcePath, const std::string& outputPath) {
    if (!std::filesystem::exists(sourcePath)) {
        Log::Error("AssetManager: Source file does not exist: " + sourcePath);
        return false;
    }
    
    // Get importer for file
    auto importer = ImporterRegistry::Get().GetImporterForFile(sourcePath);
    if (!importer) {
        Log::Error("AssetManager: No importer found for file: " + sourcePath);
        return false;
    }
    
    // Determine output path
    std::string finalOutputPath = outputPath;
    if (finalOutputPath.empty()) {
        std::filesystem::path source(sourcePath);
        std::string fileName = source.stem().string();
        std::string extension = source.extension().string();
        
        // Create output path in assets directory
        finalOutputPath = m_AssetsDirectory + "/" + fileName + extension;
    }
    
    // Import file
    LxAssetMeta meta;
    if (!importer->Import(sourcePath, finalOutputPath, meta)) {
        Log::Error("AssetManager: Failed to import file: " + sourcePath);
        return false;
    }
    
    // Set relative path
    meta.relativePath = GetRelativePath(finalOutputPath);
    
    // Save metadata
    if (!AssetMetadata::SaveMetadata(finalOutputPath, meta)) {
        Log::Error("AssetManager: Failed to save metadata for: " + finalOutputPath);
        return false;
    }
    
        // Add to assets map
        {
            std::lock_guard<std::mutex> lock(m_AssetsMutex);
            m_Assets[finalOutputPath] = meta;
        }
        
        // Generate thumbnail for imported asset
        if (m_ThumbnailGenerator) {
            m_ThumbnailGenerator->GenerateThumbnail(finalOutputPath, meta);
        }
        
        Log::Info("AssetManager: Imported file: " + sourcePath + " -> " + finalOutputPath);
        return true;
}

bool AssetManager::ReimportAsset(const std::string& assetPath) {
    std::lock_guard<std::mutex> lock(m_AssetsMutex);
    
    auto it = m_Assets.find(assetPath);
    if (it == m_Assets.end()) {
        Log::Error("AssetManager: Asset not found: " + assetPath);
        return false;
    }
    
    LxAssetMeta& meta = it->second;
    
    // Get importer
    auto importer = ImporterRegistry::Get().GetImporterForFile(assetPath);
    if (!importer) {
        Log::Error("AssetManager: No importer found for asset: " + assetPath);
        return false;
    }
    
    // Reimport
    if (!importer->Reimport(assetPath, meta)) {
        Log::Error("AssetManager: Failed to reimport asset: " + assetPath);
        return false;
    }
    
    // Update metadata
    AssetMetadata::SaveMetadata(assetPath, meta);
    
    Log::Info("AssetManager: Reimported asset: " + assetPath);
    return true;
}

LxAssetMeta* AssetManager::GetAssetMeta(const std::string& assetPath) {
    std::lock_guard<std::mutex> lock(m_AssetsMutex);
    
    auto it = m_Assets.find(assetPath);
    if (it != m_Assets.end()) {
        return &it->second;
    }
    
    return nullptr;
}

std::vector<LxAssetMeta*> AssetManager::GetAssetsInDirectory(const std::string& directory) {
    std::vector<LxAssetMeta*> assets;
    
    std::lock_guard<std::mutex> lock(m_AssetsMutex);
    
    // Normalize the input directory path
    std::filesystem::path dirPath(directory);
    if (!dirPath.is_absolute()) {
        dirPath = std::filesystem::path(m_AssetsDirectory) / dirPath;
    }
    if (!dirPath.is_absolute()) {
        dirPath = std::filesystem::absolute(dirPath);
    }
    std::string normalizedDir = dirPath.string();
    std::replace(normalizedDir.begin(), normalizedDir.end(), '\\', '/');
    
    // Also normalize assets directory for comparison
    std::string normalizedAssetsDir = m_AssetsDirectory;
    std::replace(normalizedAssetsDir.begin(), normalizedAssetsDir.end(), '\\', '/');
    
    // If directory is the assets directory, match assets directly in it
    bool isRootDir = (normalizedDir == normalizedAssetsDir);
    
    for (auto& pair : m_Assets) {
        std::filesystem::path assetPath(pair.first);
        std::string assetDir = assetPath.parent_path().string();
        
        // Normalize asset directory path
        std::string normalizedAssetDir = assetDir;
        std::replace(normalizedAssetDir.begin(), normalizedAssetDir.end(), '\\', '/');
        
        // Check if asset is in the requested directory
        bool matches = false;
        if (isRootDir) {
            // For root directory, check if asset is directly in assets directory
            matches = (normalizedAssetDir == normalizedAssetsDir);
        } else {
            // For subdirectories, check exact match or if it's a subdirectory
            matches = (normalizedAssetDir == normalizedDir) || 
                      (normalizedAssetDir.find(normalizedDir + "/") == 0);
        }
        
        if (matches) {
            assets.push_back(&pair.second);
        }
    }
    
    return assets;
}

std::shared_ptr<Texture> AssetManager::GetThumbnail(const std::string& assetPath) {
    std::lock_guard<std::mutex> lock(m_AssetsMutex);
    
    auto it = m_Assets.find(assetPath);
    if (it == m_Assets.end()) {
        return nullptr;
    }
    
    return m_ThumbnailGenerator->GenerateThumbnail(assetPath, it->second);
}

void AssetManager::SetAssetChangeCallback(std::function<void(const FileSystemEvent&)> callback) {
    m_AssetChangeCallback = callback;
}

void AssetManager::OnFileSystemEvent(const FileSystemEvent& event) {
    switch (event.type) {
        case FileSystemEventType::Created:
        case FileSystemEventType::Modified:
            ProcessFile(event.path);
            break;
            
        case FileSystemEventType::Deleted:
            RemoveAsset(event.path);
            break;
            
        case FileSystemEventType::Renamed:
            RemoveAsset(event.oldPath);
            ProcessFile(event.path);
            break;
    }
    
    // Call user callback
    if (m_AssetChangeCallback) {
        m_AssetChangeCallback(event);
    }
}

void AssetManager::ProcessFile(const std::string& filePath) {
    // Skip metadata and cache files
    if (filePath.find(".lumameta") != std::string::npos ||
        filePath.find("/Cache/") != std::string::npos ||
        filePath.find("\\Cache\\") != std::string::npos) {
        return;
    }
    
    // Check if file is in assets directory
    std::string normalizedPath = filePath;
    std::string normalizedAssetsDir = m_AssetsDirectory;
    std::replace(normalizedPath.begin(), normalizedPath.end(), '\\', '/');
    std::replace(normalizedAssetsDir.begin(), normalizedAssetsDir.end(), '\\', '/');
    
    if (normalizedPath.find(normalizedAssetsDir) != 0) {
        return; // File is not in assets directory
    }
    
    // Try to load existing metadata
    LxAssetMeta meta;
    if (AssetMetadata::LoadMetadata(filePath, meta)) {
        // Metadata exists, update it
        std::lock_guard<std::mutex> lock(m_AssetsMutex);
        m_Assets[filePath] = meta;
        Log::Info("AssetManager: Loaded asset metadata: " + filePath);
    } else {
        // No metadata, create new asset entry
        std::filesystem::path path(filePath);
        meta.name = path.stem().string();
        meta.path = filePath;
        meta.relativePath = GetRelativePath(filePath);
        meta.type = GetAssetTypeFromExtension(path.extension().string());
        
        auto now = std::chrono::system_clock::now();
        meta.importTime = std::chrono::system_clock::to_time_t(now);
        meta.modifyTime = std::chrono::system_clock::to_time_t(now);
        meta.guid = std::to_string(std::hash<std::string>{}(filePath));
        
        // Save metadata
        AssetMetadata::SaveMetadata(filePath, meta);
        
        std::lock_guard<std::mutex> lock(m_AssetsMutex);
        m_Assets[filePath] = meta;
        
        // Generate thumbnail for new asset
        if (m_ThumbnailGenerator) {
            m_ThumbnailGenerator->GenerateThumbnail(filePath, meta);
        }
        
        Log::Info("AssetManager: Created new asset entry: " + filePath);
    }
}

void AssetManager::RemoveAsset(const std::string& filePath) {
    std::lock_guard<std::mutex> lock(m_AssetsMutex);
    
    auto it = m_Assets.find(filePath);
    if (it != m_Assets.end()) {
        m_Assets.erase(it);
        Log::Info("AssetManager: Removed asset: " + filePath);
    }
}

std::string AssetManager::GetRelativePath(const std::string& fullPath) const {
    std::string normalizedFull = fullPath;
    std::string normalizedAssets = m_AssetsDirectory;
    std::replace(normalizedFull.begin(), normalizedFull.end(), '\\', '/');
    std::replace(normalizedAssets.begin(), normalizedAssets.end(), '\\', '/');
    
    if (normalizedFull.find(normalizedAssets) == 0) {
        std::string relative = normalizedFull.substr(normalizedAssets.length());
        if (relative[0] == '/') {
            relative = relative.substr(1);
        }
        return relative;
    }
    
    return fullPath;
}

} // namespace LGE

