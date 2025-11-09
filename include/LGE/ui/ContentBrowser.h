#pragma once

#include <memory>

namespace LGE {

class Texture;

class ContentBrowser {
public:
    ContentBrowser();
    ~ContentBrowser();

    void OnUIRender();

private:
    void LoadFolderIcon();
    
    std::shared_ptr<Texture> m_FolderIcon;
    bool m_FolderIconLoaded;
};

} // namespace LGE


