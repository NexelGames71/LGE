#include "LGE/assets/ImporterRegistry.h"
#include <algorithm>
#include <filesystem>

namespace LGE {

ImporterRegistry& ImporterRegistry::Get() {
    static ImporterRegistry instance;
    return instance;
}

void ImporterRegistry::RegisterImporter(std::shared_ptr<Importer> importer) {
    if (!importer) {
        return;
    }
    
    // Check if already registered
    auto it = std::find_if(m_Importers.begin(), m_Importers.end(),
        [&importer](const std::shared_ptr<Importer>& existing) {
            return existing->GetName() == importer->GetName();
        });
    
    if (it != m_Importers.end()) {
        return; // Already registered
    }
    
    m_Importers.push_back(importer);
    
    // Register all supported extensions
    auto extensions = importer->GetSupportedExtensions();
    for (const auto& ext : extensions) {
        std::string lowerExt = ext;
        std::transform(lowerExt.begin(), lowerExt.end(), lowerExt.begin(), ::tolower);
        m_ExtensionMap[lowerExt] = importer;
    }
}

std::shared_ptr<Importer> ImporterRegistry::GetImporterForExtension(const std::string& extension) {
    std::string lowerExt = extension;
    std::transform(lowerExt.begin(), lowerExt.end(), lowerExt.begin(), ::tolower);
    
    auto it = m_ExtensionMap.find(lowerExt);
    if (it != m_ExtensionMap.end()) {
        return it->second;
    }
    
    return nullptr;
}

std::shared_ptr<Importer> ImporterRegistry::GetImporterForFile(const std::string& filePath) {
    std::string extension = GetExtension(filePath);
    return GetImporterForExtension(extension);
}

std::vector<std::shared_ptr<Importer>> ImporterRegistry::GetAllImporters() const {
    return m_Importers;
}

bool ImporterRegistry::CanImport(const std::string& filePath) const {
    std::string extension = GetExtension(filePath);
    std::string lowerExt = extension;
    std::transform(lowerExt.begin(), lowerExt.end(), lowerExt.begin(), ::tolower);
    
    return m_ExtensionMap.find(lowerExt) != m_ExtensionMap.end();
}

std::string ImporterRegistry::GetExtension(const std::string& filePath) const {
    std::filesystem::path path(filePath);
    std::string ext = path.extension().string();
    return ext;
}

} // namespace LGE

