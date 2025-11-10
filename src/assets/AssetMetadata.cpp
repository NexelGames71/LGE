#include "LGE/assets/AssetMetadata.h"
#include "LGE/core/Log.h"
#include "LGE/core/FileSystem.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iomanip>

namespace LGE {

bool AssetMetadata::SaveMetadata(const std::string& assetPath, const LxAssetMeta& meta) {
    std::string metaPath = GetMetadataPath(assetPath);
    
    try {
        // Create directory if it doesn't exist
        std::filesystem::path metaDir = std::filesystem::path(metaPath).parent_path();
        if (!metaDir.empty() && !std::filesystem::exists(metaDir)) {
            std::filesystem::create_directories(metaDir);
        }
        
        // Serialize to JSON
        std::string json = SerializeToJSON(meta);
        
        // Write to file
        std::ofstream file(metaPath);
        if (!file.is_open()) {
            Log::Error("AssetMetadata: Failed to open metadata file for writing: " + metaPath);
            return false;
        }
        
        file << json;
        file.close();
        
        Log::Info("AssetMetadata: Saved metadata: " + metaPath);
        return true;
        
    } catch (const std::exception& e) {
        Log::Error("AssetMetadata: Failed to save metadata: " + std::string(e.what()));
        return false;
    }
}

bool AssetMetadata::LoadMetadata(const std::string& assetPath, LxAssetMeta& meta) {
    std::string metaPath = GetMetadataPath(assetPath);
    
    if (!std::filesystem::exists(metaPath)) {
        Log::Info("AssetMetadata: Metadata file does not exist: " + metaPath);
        return false;
    }
    
    try {
        // Read from file
        std::ifstream file(metaPath);
        if (!file.is_open()) {
            Log::Error("AssetMetadata: Failed to open metadata file for reading: " + metaPath);
            return false;
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string json = buffer.str();
        file.close();
        
        // Deserialize from JSON
        if (!DeserializeFromJSON(json, meta)) {
            Log::Error("AssetMetadata: Failed to parse metadata JSON: " + metaPath);
            return false;
        }
        
        Log::Info("AssetMetadata: Loaded metadata: " + metaPath);
        return true;
        
    } catch (const std::exception& e) {
        Log::Error("AssetMetadata: Failed to load metadata: " + std::string(e.what()));
        return false;
    }
}

std::string AssetMetadata::GetMetadataPath(const std::string& assetPath) {
    std::filesystem::path path(assetPath);
    std::string metaPath = assetPath + ".lumameta";
    return metaPath;
}

bool AssetMetadata::MetadataExists(const std::string& assetPath) {
    std::string metaPath = GetMetadataPath(assetPath);
    return std::filesystem::exists(metaPath);
}

bool AssetMetadata::DeleteMetadata(const std::string& assetPath) {
    std::string metaPath = GetMetadataPath(assetPath);
    
    if (!std::filesystem::exists(metaPath)) {
        return true; // Already deleted
    }
    
    try {
        std::filesystem::remove(metaPath);
        Log::Info("AssetMetadata: Deleted metadata: " + metaPath);
        return true;
    } catch (const std::exception& e) {
        Log::Error("AssetMetadata: Failed to delete metadata: " + std::string(e.what()));
        return false;
    }
}

std::string AssetMetadata::SerializeToJSON(const LxAssetMeta& meta) {
    std::stringstream json;
    json << "{\n";
    json << "  \"Name\": \"" << meta.name << "\",\n";
    json << "  \"Path\": \"" << meta.path << "\",\n";
    json << "  \"RelativePath\": \"" << meta.relativePath << "\",\n";
    json << "  \"Type\": \"" << AssetTypeToString(meta.type) << "\",\n";
    json << "  \"ImportTime\": " << meta.importTime << ",\n";
    json << "  \"ModifyTime\": " << meta.modifyTime << ",\n";
    json << "  \"Importer\": \"" << meta.importer << "\",\n";
    json << "  \"SourceFile\": \"" << meta.sourceFile << "\",\n";
    json << "  \"GUID\": \"" << meta.guid << "\"\n";
    json << "}";
    return json.str();
}

bool AssetMetadata::DeserializeFromJSON(const std::string& json, LxAssetMeta& meta) {
    // Simple JSON parser (for now - in production, use a proper JSON library)
    // This is a basic implementation that handles the expected format
    
    try {
        // Find each field in the JSON
        auto findField = [&json](const std::string& fieldName) -> std::string {
            std::string searchStr = "\"" + fieldName + "\":";
            size_t pos = json.find(searchStr);
            if (pos == std::string::npos) {
                return "";
            }
            
            pos += searchStr.length();
            while (pos < json.length() && (json[pos] == ' ' || json[pos] == '\t')) {
                pos++;
            }
            
            if (pos >= json.length()) {
                return "";
            }
            
            if (json[pos] == '"') {
                // String value
                pos++;
                size_t endPos = json.find('"', pos);
                if (endPos == std::string::npos) {
                    return "";
                }
                return json.substr(pos, endPos - pos);
            } else {
                // Number value
                size_t endPos = pos;
                while (endPos < json.length() && 
                       (std::isdigit(json[endPos]) || json[endPos] == '.' || json[endPos] == '-')) {
                    endPos++;
                }
                return json.substr(pos, endPos - pos);
            }
        };
        
        meta.name = findField("Name");
        meta.path = findField("Path");
        meta.relativePath = findField("RelativePath");
        meta.type = StringToAssetType(findField("Type"));
        meta.importer = findField("Importer");
        meta.sourceFile = findField("SourceFile");
        meta.guid = findField("GUID");
        
        // Parse timestamps
        std::string importTimeStr = findField("ImportTime");
        std::string modifyTimeStr = findField("ModifyTime");
        if (!importTimeStr.empty()) {
            meta.importTime = std::stoull(importTimeStr);
        }
        if (!modifyTimeStr.empty()) {
            meta.modifyTime = std::stoull(modifyTimeStr);
        }
        
        return true;
        
    } catch (const std::exception& e) {
        Log::Error("AssetMetadata: JSON parsing error: " + std::string(e.what()));
        return false;
    }
}

} // namespace LGE

