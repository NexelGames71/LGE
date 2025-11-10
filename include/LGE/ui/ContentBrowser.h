#pragma once

#include "LGE/assets/AssetMeta.h"
#include <memory>
#include <string>
#include <vector>

namespace LGE {

class Texture;
class AssetManager;
class AssetFactory;
class LxAssetMeta;

class ContentBrowser {
public:
    ContentBrowser();
    ~ContentBrowser();

    void OnUIRender();
    void OnUpdate(float deltaTime);
    
    // Set asset manager
    void SetAssetManager(AssetManager* assetManager) { m_AssetManager = assetManager; }

private:
    void LoadFolderIcon();
    void RenderFolderTree();
    void RenderFolderTreeRecursive(const std::string& dirPath, const std::string& displayName, int flags);
    void RenderContentArea();
    void RenderAssetThumbnail(LxAssetMeta* asset);
    void RenderContextMenu(LxAssetMeta* asset);
    
    // Search and filter
    void UpdateSearchFilter();
    bool MatchesSearchFilter(const LxAssetMeta& asset) const;
    bool MatchesTypeFilter(const LxAssetMeta& asset) const;
    
    AssetManager* m_AssetManager;
    std::unique_ptr<AssetFactory> m_AssetFactory;
    std::shared_ptr<Texture> m_FolderIcon;
    bool m_FolderIconLoaded;
    
    // UI state
    std::string m_CurrentDirectory;
    std::string m_SearchFilter;
    AssetType m_TypeFilter;
    bool m_ShowTypeFilter;
    
    // Selected asset
    LxAssetMeta* m_SelectedAsset;
    
    // Rename dialog state
    bool m_ShowRenameDialog;
    AssetType m_PendingAssetType;
    std::string m_PendingAssetPath;
    char m_RenameBuffer[256];
};

} // namespace LGE


