#pragma once

#include "LGE/assets/Importer.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace LGE {

// Importer registry - manages all importers
class ImporterRegistry {
public:
    static ImporterRegistry& Get();
    
    // Register an importer
    void RegisterImporter(std::shared_ptr<Importer> importer);
    
    // Get importer for a file extension
    std::shared_ptr<Importer> GetImporterForExtension(const std::string& extension);
    
    // Get importer for a file path
    std::shared_ptr<Importer> GetImporterForFile(const std::string& filePath);
    
    // Get all registered importers
    std::vector<std::shared_ptr<Importer>> GetAllImporters() const;
    
    // Check if a file can be imported
    bool CanImport(const std::string& filePath) const;
    
private:
    ImporterRegistry() = default;
    ~ImporterRegistry() = default;
    ImporterRegistry(const ImporterRegistry&) = delete;
    ImporterRegistry& operator=(const ImporterRegistry&) = delete;
    
    std::vector<std::shared_ptr<Importer>> m_Importers;
    std::unordered_map<std::string, std::shared_ptr<Importer>> m_ExtensionMap;
    
    std::string GetExtension(const std::string& filePath) const;
};

} // namespace LGE

