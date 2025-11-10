#pragma once

#include <string>
#include <vector>
#include <memory>

namespace LGE {

class Texture;

// Project template category
enum class ProjectCategory {
    Recent,
    Games
};

// Project template
struct ProjectTemplate {
    std::string name;
    std::string description;
    ProjectCategory category;
    std::string thumbnailPath;
    std::string previewPath; // Preview image path
    std::vector<std::string> assetTypeReferences;
    bool hasStarterContent;
    std::string templatePath; // Path to template files
    
    ProjectTemplate()
        : category(ProjectCategory::Games)
        , hasStarterContent(false)
    {
    }
};

// Project Browser - similar to Unreal Engine's project browser
class ProjectBrowser {
public:
    ProjectBrowser();
    ~ProjectBrowser();

    void OnUIRender();
    
    bool IsOpen() const { return m_IsOpen; }
    void SetOpen(bool open) { m_IsOpen = open; }
    
    // Get created project path (if project was created)
    std::string GetCreatedProjectPath() const { return m_CreatedProjectPath; }
    bool WasProjectCreated() const { return m_ProjectCreated; }

private:
    void LoadTemplates();
    void UpdateFilteredTemplates();
    void LoadCategoryThumbnails();
    void RenderTopBar();
    void RenderLeftNavigation();
    void RenderTemplateGrid();
    void RenderDetailsPanel();
    void RenderBottomBar();
    void CreateProject();
    
    bool m_IsOpen;
    bool m_ProjectCreated;
    std::string m_CreatedProjectPath;
    
    // UI State
    ProjectCategory m_SelectedCategory;
    int m_SelectedTemplateIndex;
    
    // Project Settings
    char m_ProjectLocationBuffer[512];
    char m_ProjectNameBuffer[256];
    bool m_IncludeStarterContent;
    
    // Templates
    std::vector<ProjectTemplate> m_Templates;
    std::vector<ProjectTemplate> m_FilteredTemplates;
    
    // Category Thumbnails
    std::shared_ptr<Texture> m_RecentThumbnail;
    std::shared_ptr<Texture> m_GamesThumbnail;
    
    // Template Thumbnails
    std::vector<std::shared_ptr<Texture>> m_TemplateThumbnails;
    std::vector<std::shared_ptr<Texture>> m_TemplatePreviews; // Preview images
    bool m_ThumbnailsLoaded;
};

} // namespace LGE

