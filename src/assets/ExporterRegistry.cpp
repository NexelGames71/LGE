#include "LGE/assets/ExporterRegistry.h"
#include <algorithm>

namespace LGE {

ExporterRegistry& ExporterRegistry::Get() {
    static ExporterRegistry instance;
    return instance;
}

void ExporterRegistry::RegisterExporter(std::shared_ptr<Exporter> exporter) {
    if (!exporter) {
        return;
    }
    
    // Check if already registered
    auto it = std::find_if(m_Exporters.begin(), m_Exporters.end(),
        [&exporter](const std::shared_ptr<Exporter>& existing) {
            return existing->GetName() == exporter->GetName();
        });
    
    if (it != m_Exporters.end()) {
        return; // Already registered
    }
    
    m_Exporters.push_back(exporter);
    
    // Register all supported formats
    auto formats = exporter->GetSupportedFormats();
    for (const auto& format : formats) {
        std::string lowerFormat = format;
        std::transform(lowerFormat.begin(), lowerFormat.end(), lowerFormat.begin(), ::tolower);
        m_FormatMap[lowerFormat] = exporter;
    }
    
    // Register by asset type
    AssetType types[] = {
        AssetType::Unknown, AssetType::Texture2D, AssetType::TextureCube,
        AssetType::Model, AssetType::Material, AssetType::Shader,
        AssetType::Script, AssetType::Scene, AssetType::Sound,
        AssetType::Animation, AssetType::Blueprint, AssetType::Folder
    };
    for (AssetType type : types) {
        if (exporter->CanExport(type)) {
            m_TypeMap[type].push_back(exporter);
        }
    }
}

std::shared_ptr<Exporter> ExporterRegistry::GetExporterForFormat(const std::string& format) {
    std::string lowerFormat = format;
    std::transform(lowerFormat.begin(), lowerFormat.end(), lowerFormat.begin(), ::tolower);
    
    auto it = m_FormatMap.find(lowerFormat);
    if (it != m_FormatMap.end()) {
        return it->second;
    }
    
    return nullptr;
}

std::shared_ptr<Exporter> ExporterRegistry::GetExporterForAsset(AssetType type, const std::string& format) {
    auto exporters = GetExportersForAssetType(type);
    
    std::string lowerFormat = format;
    std::transform(lowerFormat.begin(), lowerFormat.end(), lowerFormat.begin(), ::tolower);
    
    for (auto& exporter : exporters) {
        auto formats = exporter->GetSupportedFormats();
        for (const auto& expFormat : formats) {
            std::string lowerExpFormat = expFormat;
            std::transform(lowerExpFormat.begin(), lowerExpFormat.end(), lowerExpFormat.begin(), ::tolower);
            if (lowerExpFormat == lowerFormat) {
                return exporter;
            }
        }
    }
    
    return nullptr;
}

std::vector<std::shared_ptr<Exporter>> ExporterRegistry::GetExportersForAssetType(AssetType type) const {
    auto it = m_TypeMap.find(type);
    if (it != m_TypeMap.end()) {
        return it->second;
    }
    
    return {};
}

std::vector<std::shared_ptr<Exporter>> ExporterRegistry::GetAllExporters() const {
    return m_Exporters;
}

bool ExporterRegistry::CanExport(AssetType type, const std::string& format) const {
    auto exporters = GetExportersForAssetType(type);
    
    std::string lowerFormat = format;
    std::transform(lowerFormat.begin(), lowerFormat.end(), lowerFormat.begin(), ::tolower);
    
    for (auto& exporter : exporters) {
        auto formats = exporter->GetSupportedFormats();
        for (const auto& expFormat : formats) {
            std::string lowerExpFormat = expFormat;
            std::transform(lowerExpFormat.begin(), lowerExpFormat.end(), lowerExpFormat.begin(), ::tolower);
            if (lowerExpFormat == lowerFormat) {
                return true;
            }
        }
    }
    
    return false;
}

} // namespace LGE

