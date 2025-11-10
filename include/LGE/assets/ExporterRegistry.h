#pragma once

#include "LGE/assets/Exporter.h"
#include "LGE/assets/AssetMeta.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace LGE {

// Exporter registry - manages all exporters
class ExporterRegistry {
public:
    static ExporterRegistry& Get();
    
    // Register an exporter
    void RegisterExporter(std::shared_ptr<Exporter> exporter);
    
    // Get exporter for a format
    std::shared_ptr<Exporter> GetExporterForFormat(const std::string& format);
    
    // Get exporter for an asset type and format
    std::shared_ptr<Exporter> GetExporterForAsset(AssetType type, const std::string& format);
    
    // Get all exporters for an asset type
    std::vector<std::shared_ptr<Exporter>> GetExportersForAssetType(AssetType type) const;
    
    // Get all registered exporters
    std::vector<std::shared_ptr<Exporter>> GetAllExporters() const;
    
    // Check if an asset can be exported to a format
    bool CanExport(AssetType type, const std::string& format) const;
    
private:
    ExporterRegistry() = default;
    ~ExporterRegistry() = default;
    ExporterRegistry(const ExporterRegistry&) = delete;
    ExporterRegistry& operator=(const ExporterRegistry&) = delete;
    
    // Hash function for AssetType
    struct AssetTypeHash {
        std::size_t operator()(AssetType type) const {
            return std::hash<int>{}(static_cast<int>(type));
        }
    };
    
    std::vector<std::shared_ptr<Exporter>> m_Exporters;
    std::unordered_map<std::string, std::shared_ptr<Exporter>> m_FormatMap;
    std::unordered_map<AssetType, std::vector<std::shared_ptr<Exporter>>, AssetTypeHash> m_TypeMap;
};

} // namespace LGE

