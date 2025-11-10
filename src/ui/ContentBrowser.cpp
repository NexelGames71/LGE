#include "LGE/ui/ContentBrowser.h"
#include "LGE/assets/AssetManager.h"
#include "LGE/assets/AssetMeta.h"
#include "LGE/assets/AssetFactory.h"
#include "LGE/assets/ExporterRegistry.h"
#include "LGE/rendering/Texture.h"
#include "LGE/core/FileSystem.h"
#include "LGE/core/Log.h"
#include "imgui.h"
#include "imgui_internal.h"
#include <vector>
#include <string>
#include <filesystem>
#include <algorithm>
#include <cctype>

namespace LGE {

ContentBrowser::ContentBrowser()
    : m_AssetManager(nullptr)
    , m_AssetFactory(std::make_unique<AssetFactory>())
    , m_FolderIconLoaded(false)
    , m_CurrentDirectory("Assets")
    , m_TypeFilter(AssetType::Unknown)
    , m_ShowTypeFilter(false)
    , m_SelectedAsset(nullptr)
    , m_ShowRenameDialog(false)
    , m_PendingAssetType(AssetType::Unknown)
    , m_PendingAssetPath("")
{
    memset(m_RenameBuffer, 0, sizeof(m_RenameBuffer));
}

ContentBrowser::~ContentBrowser() {
}

void ContentBrowser::LoadFolderIcon() {
    if (m_FolderIconLoaded) return;
    
    std::vector<std::string> folderPaths = {
        "unreal-engine-editor-icons/imgs/Icons/Folders/Folder_Base_512x.png",
        "assets/icons/Folder_Base_512x.png",
        "assets/icons/folder.png"
    };
    
    m_FolderIcon = std::make_shared<Texture>();
    bool folderLoaded = false;
    for (const auto& path : folderPaths) {
        if (m_FolderIcon->LoadImageFile(path)) {
            folderLoaded = true;
            break;
        }
    }
    if (!folderLoaded) {
        m_FolderIcon.reset();
    }
    
    m_FolderIconLoaded = true;
}

void ContentBrowser::OnUpdate(float deltaTime) {
    // Update asset manager if available
    if (m_AssetManager) {
        m_AssetManager->Update();
    }
}

void ContentBrowser::OnUIRender() {
    // Load folder icon on first render
    if (!m_FolderIconLoaded) {
        LoadFolderIcon();
    }
    
    // Modern window styling
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12.0f, 8.0f));
    ImGui::Begin("Content Browser", nullptr, ImGuiWindowFlags_None);
    ImGui::PopStyleVar();
    
    // Modern toolbar
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4.0f, 4.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8.0f, 4.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
    
    if (ImGui::Button("Import")) {
        // TODO: Open file dialog to import asset
        // For now, just log
        Log::Info("ContentBrowser: Import button clicked - File dialog not yet implemented");
    }
    ImGui::SameLine();
    
    // Create dropdown
    if (ImGui::Button("Create")) {
        ImGui::OpenPopup("CreateAssetPopup");
    }
    if (ImGui::BeginPopup("CreateAssetPopup")) {
        if (ImGui::MenuItem("Material")) {
            std::string assetsDir = m_AssetManager ? m_AssetManager->GetAssetsDirectory() : "Assets";
            std::filesystem::path currentDir(m_CurrentDirectory);
            if (m_CurrentDirectory == "Assets") {
                currentDir = std::filesystem::path(assetsDir);
            } else if (!currentDir.is_absolute()) {
                currentDir = std::filesystem::path(assetsDir) / currentDir;
            }
            if (!currentDir.is_absolute()) {
                currentDir = std::filesystem::absolute(currentDir);
            }
            m_PendingAssetPath = (currentDir / "NewMaterial.material").string();
            m_PendingAssetType = AssetType::Material;
            strncpy_s(m_RenameBuffer, "NewMaterial", sizeof(m_RenameBuffer) - 1);
            m_ShowRenameDialog = true;
        }
        if (ImGui::MenuItem("Shader")) {
            std::string assetsDir = m_AssetManager ? m_AssetManager->GetAssetsDirectory() : "Assets";
            std::filesystem::path currentDir(m_CurrentDirectory);
            if (m_CurrentDirectory == "Assets") {
                currentDir = std::filesystem::path(assetsDir);
            } else if (!currentDir.is_absolute()) {
                currentDir = std::filesystem::path(assetsDir) / currentDir;
            }
            if (!currentDir.is_absolute()) {
                currentDir = std::filesystem::absolute(currentDir);
            }
            m_PendingAssetPath = (currentDir / "NewShader.shader").string();
            m_PendingAssetType = AssetType::Shader;
            strncpy_s(m_RenameBuffer, "NewShader", sizeof(m_RenameBuffer) - 1);
            m_ShowRenameDialog = true;
        }
        if (ImGui::MenuItem("Script")) {
            std::string assetsDir = m_AssetManager ? m_AssetManager->GetAssetsDirectory() : "Assets";
            std::filesystem::path currentDir(m_CurrentDirectory);
            if (m_CurrentDirectory == "Assets") {
                currentDir = std::filesystem::path(assetsDir);
            } else if (!currentDir.is_absolute()) {
                currentDir = std::filesystem::path(assetsDir) / currentDir;
            }
            if (!currentDir.is_absolute()) {
                currentDir = std::filesystem::absolute(currentDir);
            }
            m_PendingAssetPath = (currentDir / "NewScript.cpp").string();
            m_PendingAssetType = AssetType::Script;
            strncpy_s(m_RenameBuffer, "NewScript", sizeof(m_RenameBuffer) - 1);
            m_ShowRenameDialog = true;
        }
        if (ImGui::MenuItem("Scene")) {
            std::string assetsDir = m_AssetManager ? m_AssetManager->GetAssetsDirectory() : "Assets";
            std::filesystem::path currentDir(m_CurrentDirectory);
            if (m_CurrentDirectory == "Assets") {
                currentDir = std::filesystem::path(assetsDir);
            } else if (!currentDir.is_absolute()) {
                currentDir = std::filesystem::path(assetsDir) / currentDir;
            }
            if (!currentDir.is_absolute()) {
                currentDir = std::filesystem::absolute(currentDir);
            }
            m_PendingAssetPath = (currentDir / "NewScene.scene").string();
            m_PendingAssetType = AssetType::Scene;
            strncpy_s(m_RenameBuffer, "NewScene", sizeof(m_RenameBuffer) - 1);
            m_ShowRenameDialog = true;
        }
        if (ImGui::MenuItem("Folder")) {
            std::string assetsDir = m_AssetManager ? m_AssetManager->GetAssetsDirectory() : "Assets";
            std::filesystem::path currentDir(m_CurrentDirectory);
            if (m_CurrentDirectory == "Assets") {
                currentDir = std::filesystem::path(assetsDir);
            } else if (!currentDir.is_absolute()) {
                currentDir = std::filesystem::path(assetsDir) / currentDir;
            }
            if (!currentDir.is_absolute()) {
                currentDir = std::filesystem::absolute(currentDir);
            }
            m_PendingAssetPath = currentDir.string();
            m_PendingAssetType = AssetType::Folder;
            strncpy_s(m_RenameBuffer, "NewFolder", sizeof(m_RenameBuffer) - 1);
            m_ShowRenameDialog = true;
        }
        ImGui::EndPopup();
    }
    
    // Rename dialog
    if (m_ShowRenameDialog) {
        ImGui::OpenPopup("RenameAssetDialog");
        m_ShowRenameDialog = false;
    }
    
    if (ImGui::BeginPopupModal("RenameAssetDialog", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Enter asset name:");
        ImGui::Spacing();
        
        ImGui::PushItemWidth(300.0f);
        bool enterPressed = ImGui::InputText("##RenameInput", m_RenameBuffer, sizeof(m_RenameBuffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll);
        ImGui::PopItemWidth();
        
        ImGui::Spacing();
        
        if (ImGui::Button("Create", ImVec2(100, 0)) || enterPressed) {
            if (strlen(m_RenameBuffer) > 0 && m_AssetFactory && m_AssetManager) {
                std::string assetName = std::string(m_RenameBuffer);
                std::string finalPath = m_PendingAssetPath;
                
                // Update path with new name
                std::filesystem::path pathObj(m_PendingAssetPath);
                if (!pathObj.is_absolute()) {
                    // Convert to absolute path
                    pathObj = std::filesystem::absolute(pathObj);
                }
                
                std::string extension = pathObj.extension().string();
                std::filesystem::path parentDir = pathObj.parent_path();
                
                if (m_PendingAssetType == AssetType::Folder) {
                    finalPath = (parentDir / assetName).string();
                } else {
                    finalPath = (parentDir / (assetName + extension)).string();
                }
                
                bool success = false;
                switch (m_PendingAssetType) {
                    case AssetType::Material:
                        success = m_AssetFactory->CreateMaterial(finalPath, assetName);
                        break;
                    case AssetType::Shader:
                        success = m_AssetFactory->CreateShader(finalPath, assetName);
                        break;
                    case AssetType::Script:
                        success = m_AssetFactory->CreateScript(finalPath, assetName);
                        break;
                    case AssetType::Scene:
                        success = m_AssetFactory->CreateScene(finalPath, assetName);
                        break;
                    case AssetType::Folder:
                        success = m_AssetFactory->CreateFolder(finalPath, assetName);
                        break;
                    default:
                        break;
                }
                
                if (success) {
                    m_AssetManager->ScanAssets();
                }
            }
            ImGui::CloseCurrentPopup();
        }
        
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(100, 0))) {
            ImGui::CloseCurrentPopup();
        }
        
        ImGui::EndPopup();
    }
    ImGui::SameLine();
    
    // Search bar
    char searchBuffer[256] = "";
    strncpy_s(searchBuffer, m_SearchFilter.c_str(), sizeof(searchBuffer) - 1);
    ImGui::PushItemWidth(200.0f);
    if (ImGui::InputTextWithHint("##Search", "Search...", searchBuffer, sizeof(searchBuffer))) {
        m_SearchFilter = std::string(searchBuffer);
        UpdateSearchFilter();
    }
    ImGui::PopItemWidth();
    
    ImGui::SameLine();
    
    // Type filter dropdown
    const char* typeFilters[] = { "All", "Texture2D", "Model", "Material", "Shader", "Script", "Scene", "Sound" };
    static int currentTypeFilter = 0;
    ImGui::SetNextItemWidth(120.0f);
    if (ImGui::Combo("##TypeFilter", &currentTypeFilter, typeFilters, IM_ARRAYSIZE(typeFilters))) {
        if (currentTypeFilter == 0) {
            m_TypeFilter = AssetType::Unknown;
        } else {
            m_TypeFilter = StringToAssetType(typeFilters[currentTypeFilter]);
        }
    }
    
    ImGui::PopStyleVar(3);
    
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    
    // Split view: folders on left, content on right
    float panelWidth = ImGui::GetContentRegionAvail().x;
    float folderPanelWidth = panelWidth * 0.25f;
    
    // Folder tree
    RenderFolderTree();
    
    ImGui::SameLine();
    
    // Content area
    RenderContentArea();
    
    ImGui::End();
}

void ContentBrowser::RenderFolderTree() {
    float folderPanelWidth = ImGui::GetContentRegionAvail().x * 0.25f;
    
    // Folder tree with modern styling
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 4.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
    ImGui::BeginChild("FolderTree", ImVec2(folderPanelWidth, 0), true, ImGuiWindowFlags_None);
    ImGui::PopStyleVar(2);
    
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2.0f, 4.0f));
    
    if (!m_AssetManager) {
        ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "No Asset Manager");
    } else {
        // Render folder tree
        std::string assetsDir = m_AssetManager->GetAssetsDirectory();
        RenderFolderTreeRecursive(assetsDir, "Assets", ImGuiTreeNodeFlags_DefaultOpen);
    }
    
    ImGui::PopStyleVar();
    ImGui::EndChild();
}

void ContentBrowser::RenderFolderTreeRecursive(const std::string& dirPath, const std::string& displayName, ImGuiTreeNodeFlags flags) {
    if (!m_AssetManager) return;
    
    std::string assetsDir = m_AssetManager->GetAssetsDirectory();
    std::filesystem::path path(dirPath);
    
    // Get all subdirectories
    std::vector<std::filesystem::path> subdirs;
    if (std::filesystem::exists(path) && std::filesystem::is_directory(path)) {
        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            if (entry.is_directory()) {
                subdirs.push_back(entry.path());
            }
        }
    }
    
    // Sort directories
    std::sort(subdirs.begin(), subdirs.end());
    
    // Render this folder
    bool isOpen = ImGui::TreeNodeEx(displayName.c_str(), flags);
    
    // Handle click to change directory
    if (ImGui::IsItemClicked()) {
        m_CurrentDirectory = dirPath;
    }
    
    // Render subdirectories if open
    if (isOpen) {
        for (const auto& subdir : subdirs) {
            std::string subdirName = subdir.filename().string();
            std::string subdirPath = subdir.string();
            RenderFolderTreeRecursive(subdirPath, subdirName, 0);
        }
        ImGui::TreePop();
    }
}

void ContentBrowser::RenderContentArea() {
    // Content area with modern styling
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 4.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
    ImGui::BeginChild("ContentArea", ImVec2(0, 0), true, ImGuiWindowFlags_None);
    ImGui::PopStyleVar(2);
    
    if (!m_AssetManager) {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.6f, 0.6f, 1.0f));
        ImGui::TextWrapped("No Asset Manager - assets will be displayed here");
        ImGui::PopStyleColor();
    } else {
        // Get assets in current directory
        std::string assetsDir = m_AssetManager->GetAssetsDirectory();
        std::string currentDir = m_CurrentDirectory;
        
        // Normalize the current directory path
        if (currentDir == "Assets") {
            currentDir = assetsDir;
        } else {
            // Convert to absolute path if relative
            std::filesystem::path currentDirPath(currentDir);
            if (!currentDirPath.is_absolute()) {
                currentDirPath = std::filesystem::path(assetsDir) / currentDir;
            }
            if (!currentDirPath.is_absolute()) {
                currentDirPath = std::filesystem::absolute(currentDirPath);
            }
            currentDir = currentDirPath.string();
        }
        
        // Normalize path separators for comparison
        std::replace(currentDir.begin(), currentDir.end(), '\\', '/');
        std::string normalizedAssetsDir = assetsDir;
        std::replace(normalizedAssetsDir.begin(), normalizedAssetsDir.end(), '\\', '/');
        
        // If currentDir is the assets directory, use it as-is
        if (currentDir == normalizedAssetsDir) {
            currentDir = normalizedAssetsDir;
        }
        
        std::vector<LxAssetMeta*> assets = m_AssetManager->GetAssetsInDirectory(currentDir);
        
        // Filter assets
        std::vector<LxAssetMeta*> filteredAssets;
        for (auto* asset : assets) {
            if (MatchesSearchFilter(*asset) && MatchesTypeFilter(*asset)) {
                filteredAssets.push_back(asset);
            }
        }
        
        // Display assets in grid view
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
        
        float thumbnailSize = 64.0f;
        float padding = 8.0f;
        float availableWidth = ImGui::GetContentRegionAvail().x;
        int columns = (int)(availableWidth / (thumbnailSize + padding));
        if (columns < 1) columns = 1;
        
        ImGui::Columns(columns, nullptr, false);
        
        for (auto* asset : filteredAssets) {
            RenderAssetThumbnail(asset);
            ImGui::NextColumn();
        }
        
        ImGui::Columns(1);
        ImGui::PopStyleVar();
        
        // Right-click on empty space
        if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(1)) {
            ImGui::OpenPopup("EmptySpaceContextMenu");
        }
    }
    
    // Render context menu
    if (ImGui::BeginPopup("AssetContextMenu")) {
        RenderContextMenu(m_SelectedAsset);
        ImGui::EndPopup();
    }
    
    if (ImGui::BeginPopup("EmptySpaceContextMenu")) {
        RenderContextMenu(nullptr);
        ImGui::EndPopup();
    }
    
    ImGui::EndChild();
}

void ContentBrowser::RenderAssetThumbnail(LxAssetMeta* asset) {
    if (!asset) return;
    
    ImGui::PushID(asset->path.c_str());
    
    bool isSelected = (m_SelectedAsset == asset);
    
    // Get thumbnail - ensure it's generated
    std::shared_ptr<Texture> thumbnail = nullptr;
    if (m_AssetManager) {
        thumbnail = m_AssetManager->GetThumbnail(asset->path);
        // If thumbnail is null, try to generate it
        if (!thumbnail && asset->type != AssetType::Folder) {
            // Thumbnail will be generated on next scan
            m_AssetManager->ScanAssets();
            thumbnail = m_AssetManager->GetThumbnail(asset->path);
        }
    }
    
    float thumbnailSize = 64.0f;
    ImVec2 cursorPos = ImGui::GetCursorPos();
    
    // Draw thumbnail or folder icon
    if (asset->type == AssetType::Folder) {
        // Draw folder icon
        if (m_FolderIcon && m_FolderIcon->GetRendererID() != 0) {
            ImVec2 imageSize(thumbnailSize, thumbnailSize);
            ImGui::Image(
                reinterpret_cast<void*>(static_cast<intptr_t>(m_FolderIcon->GetRendererID())),
                imageSize,
                ImVec2(0, 0), ImVec2(1, 1),
                ImVec4(0.95f, 0.85f, 0.55f, 1.0f),
                ImVec4(0.0f, 0.0f, 0.0f, 0.0f)
            );
        }
    } else if (thumbnail && thumbnail->GetRendererID() != 0) {
        // Draw asset thumbnail
        ImVec2 imageSize(thumbnailSize, thumbnailSize);
        ImGui::Image(
            reinterpret_cast<void*>(static_cast<intptr_t>(thumbnail->GetRendererID())),
            imageSize,
            ImVec2(0, 0), ImVec2(1, 1)
        );
    } else {
        // Draw placeholder
        ImVec2 imageSize(thumbnailSize, thumbnailSize);
        ImGui::Button("##Placeholder", imageSize);
    }
    
    // Make it clickable
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip(asset->name.c_str());
    }
    if (ImGui::IsItemClicked()) {
        m_SelectedAsset = asset;
    }
    if (ImGui::IsItemClicked(1)) { // Right click
        m_SelectedAsset = asset;
        ImGui::OpenPopupOnItemClick("AssetContextMenu", ImGuiPopupFlags_MouseButtonRight);
    }
    
    // Asset name below thumbnail
    ImVec2 textSize = ImGui::CalcTextSize(asset->name.c_str());
    ImGui::SetCursorPosX(cursorPos.x + (thumbnailSize - textSize.x) * 0.5f);
    ImGui::Text(asset->name.c_str());
    
    ImGui::PopID();
}

void ContentBrowser::RenderContextMenu(LxAssetMeta* asset) {
    if (ImGui::BeginPopupContextItem()) {
        if (asset) {
            // Asset-specific menu items
            if (ImGui::MenuItem("Reimport")) {
                if (m_AssetManager) {
                    m_AssetManager->ReimportAsset(asset->path);
                }
            }
            
            ImGui::Separator();
            
            // Export submenu
            if (ImGui::BeginMenu("Export As...")) {
                auto exporters = ExporterRegistry::Get().GetExportersForAssetType(asset->type);
                if (exporters.empty()) {
                    ImGui::TextDisabled("No exporters available");
                } else {
                    for (auto& exporter : exporters) {
                        auto formats = exporter->GetSupportedFormats();
                        for (const auto& format : formats) {
                            if (ImGui::MenuItem(format.c_str())) {
                                // TODO: Open file dialog to choose export path
                                std::string exportPath = asset->path + format;
                                if (exporter->Export(asset->path, exportPath, *asset)) {
                                    Log::Info("ContentBrowser: Exported asset: " + asset->path + " -> " + exportPath);
                                }
                            }
                        }
                    }
                }
                ImGui::EndMenu();
            }
            
            ImGui::Separator();
            
            if (ImGui::MenuItem("Delete")) {
                // TODO: Delete asset
                Log::Info("ContentBrowser: Delete asset: " + asset->path);
            }
            if (ImGui::MenuItem("Show in Explorer")) {
                // TODO: Open file in explorer
                Log::Info("ContentBrowser: Show in Explorer: " + asset->path);
            }
        } else {
            // Empty space context menu - create new assets
            if (ImGui::BeginMenu("Create")) {
                if (ImGui::MenuItem("Material")) {
                    if (m_AssetFactory && m_AssetManager) {
                        std::string assetsDir = m_AssetManager->GetAssetsDirectory();
                        std::string currentDir = m_CurrentDirectory;
                        if (currentDir == "Assets") {
                            currentDir = assetsDir;
                        } else if (currentDir.find(assetsDir) != 0) {
                            currentDir = assetsDir + "/" + currentDir;
                        }
                        std::string path = currentDir + "/NewMaterial.material";
                        if (m_AssetFactory->CreateMaterial(path, "NewMaterial")) {
                            Log::Info("ContentBrowser: Created material: " + path);
                            m_AssetManager->ScanAssets();
                        }
                    }
                }
                if (ImGui::MenuItem("Shader")) {
                    if (m_AssetFactory && m_AssetManager) {
                        std::string assetsDir = m_AssetManager->GetAssetsDirectory();
                        std::string currentDir = m_CurrentDirectory;
                        if (currentDir == "Assets") {
                            currentDir = assetsDir;
                        } else if (currentDir.find(assetsDir) != 0) {
                            currentDir = assetsDir + "/" + currentDir;
                        }
                        std::string path = currentDir + "/NewShader.shader";
                        if (m_AssetFactory->CreateShader(path, "NewShader")) {
                            Log::Info("ContentBrowser: Created shader: " + path);
                            m_AssetManager->ScanAssets();
                        }
                    }
                }
                if (ImGui::MenuItem("Script")) {
                    if (m_AssetFactory && m_AssetManager) {
                        std::string assetsDir = m_AssetManager->GetAssetsDirectory();
                        std::string currentDir = m_CurrentDirectory;
                        if (currentDir == "Assets") {
                            currentDir = assetsDir;
                        } else if (currentDir.find(assetsDir) != 0) {
                            currentDir = assetsDir + "/" + currentDir;
                        }
                        std::string path = currentDir + "/NewScript.cpp";
                        if (m_AssetFactory->CreateScript(path, "NewScript")) {
                            Log::Info("ContentBrowser: Created script: " + path);
                            m_AssetManager->ScanAssets();
                        }
                    }
                }
                if (ImGui::MenuItem("Scene")) {
                    if (m_AssetFactory && m_AssetManager) {
                        std::string assetsDir = m_AssetManager->GetAssetsDirectory();
                        std::string currentDir = m_CurrentDirectory;
                        if (currentDir == "Assets") {
                            currentDir = assetsDir;
                        } else if (currentDir.find(assetsDir) != 0) {
                            currentDir = assetsDir + "/" + currentDir;
                        }
                        std::string path = currentDir + "/NewScene.scene";
                        if (m_AssetFactory->CreateScene(path, "NewScene")) {
                            Log::Info("ContentBrowser: Created scene: " + path);
                            m_AssetManager->ScanAssets();
                        }
                    }
                }
                if (ImGui::MenuItem("Folder")) {
                    if (m_AssetFactory && m_AssetManager) {
                        std::string assetsDir = m_AssetManager->GetAssetsDirectory();
                        std::string currentDir = m_CurrentDirectory;
                        if (currentDir == "Assets") {
                            currentDir = assetsDir;
                        } else if (currentDir.find(assetsDir) != 0) {
                            currentDir = assetsDir + "/" + currentDir;
                        }
                        std::string path = currentDir;
                        if (m_AssetFactory->CreateFolder(path, "NewFolder")) {
                            Log::Info("ContentBrowser: Created folder: " + path);
                            m_AssetManager->ScanAssets();
                        }
                    }
                }
                ImGui::EndMenu();
            }
        }
        ImGui::EndPopup();
    }
}

void ContentBrowser::UpdateSearchFilter() {
    // Search filter is updated in OnUIRender
}

bool ContentBrowser::MatchesSearchFilter(const LxAssetMeta& asset) const {
    if (m_SearchFilter.empty()) {
        return true;
    }
    
    std::string assetName = asset.name;
    std::string searchLower = m_SearchFilter;
    std::transform(assetName.begin(), assetName.end(), assetName.begin(), ::tolower);
    std::transform(searchLower.begin(), searchLower.end(), searchLower.begin(), ::tolower);
    
    return assetName.find(searchLower) != std::string::npos;
}

bool ContentBrowser::MatchesTypeFilter(const LxAssetMeta& asset) const {
    if (m_TypeFilter == AssetType::Unknown) {
        return true; // Show all types
    }
    
    return asset.type == m_TypeFilter;
}

} // namespace LGE

