#include "LGE/assets/AssetFactory.h"
#include "LGE/assets/AssetMetadata.h"
#include "LGE/core/Log.h"
#include <filesystem>
#include <fstream>
#include <chrono>
#include <ctime>

namespace LGE {

AssetFactory::AssetFactory() {
}

AssetFactory::~AssetFactory() {
}

bool AssetFactory::CreateAsset(AssetType type, const std::string& path, const std::string& name) {
    switch (type) {
        case AssetType::Material:
            return CreateMaterial(path, name);
        case AssetType::Shader:
            return CreateShader(path, name);
        case AssetType::Script:
            return CreateScript(path, name);
        case AssetType::Scene:
            return CreateScene(path, name);
        case AssetType::Folder:
            return CreateFolder(path, name);
        default:
            Log::Error("AssetFactory: Unsupported asset type for creation");
            return false;
    }
}

bool AssetFactory::CreateMaterial(const std::string& path, const std::string& name) {
    try {
        // Normalize path to absolute path
        std::filesystem::path filePath(path);
        if (!filePath.is_absolute()) {
            // Convert to absolute path
            filePath = std::filesystem::absolute(filePath);
        }
        
        // Create directory if it doesn't exist
        std::filesystem::path dir = filePath.parent_path();
        if (!dir.empty() && !std::filesystem::exists(dir)) {
            std::filesystem::create_directories(dir);
        }
        
        // Generate unique filename if file exists
        std::string finalPath = GenerateUniqueFilename(filePath.string());
        
        // Generate default material content
        std::string materialName = name.empty() ? std::filesystem::path(finalPath).stem().string() : name;
        std::string content = GenerateDefaultContent(AssetType::Material, materialName);
        
        // Write material file
        std::ofstream file(finalPath);
        if (!file.is_open()) {
            Log::Error("AssetFactory: Failed to create material file: " + finalPath);
            return false;
        }
        
        file << content;
        file.flush();
        file.close();
        
        // Verify file was written
        if (!std::filesystem::exists(finalPath)) {
            Log::Error("AssetFactory: Material file was not created: " + finalPath);
            return false;
        }
        
        // Create metadata
        LxAssetMeta meta = CreateAssetMeta(AssetType::Material, finalPath, materialName);
        meta.importer = "MaterialFactory";
        
        if (!AssetMetadata::SaveMetadata(finalPath, meta)) {
            Log::Error("AssetFactory: Failed to save material metadata: " + finalPath);
            return false;
        }
        
        Log::Info("AssetFactory: Created material: " + finalPath);
        return true;
        
    } catch (const std::exception& e) {
        Log::Error("AssetFactory: Failed to create material: " + std::string(e.what()));
        return false;
    }
}

bool AssetFactory::CreateShader(const std::string& path, const std::string& name) {
    try {
        // Normalize path to absolute path
        std::filesystem::path filePath(path);
        if (!filePath.is_absolute()) {
            // Convert to absolute path
            filePath = std::filesystem::absolute(filePath);
        }
        
        // Create directory if it doesn't exist
        std::filesystem::path dir = filePath.parent_path();
        if (!dir.empty() && !std::filesystem::exists(dir)) {
            std::filesystem::create_directories(dir);
        }
        
        // Generate unique filename if file exists
        std::string finalPath = GenerateUniqueFilename(filePath.string());
        
        // Generate default shader content
        std::string shaderName = name.empty() ? std::filesystem::path(finalPath).stem().string() : name;
        std::string content = GenerateDefaultContent(AssetType::Shader, shaderName);
        
        // Write shader file
        std::ofstream file(finalPath);
        if (!file.is_open()) {
            Log::Error("AssetFactory: Failed to create shader file: " + finalPath);
            return false;
        }
        
        file << content;
        file.flush();
        file.close();
        
        // Verify file was written
        if (!std::filesystem::exists(finalPath)) {
            Log::Error("AssetFactory: Shader file was not created: " + finalPath);
            return false;
        }
        
        // Create metadata
        LxAssetMeta meta = CreateAssetMeta(AssetType::Shader, finalPath, shaderName);
        meta.importer = "ShaderFactory";
        
        if (!AssetMetadata::SaveMetadata(finalPath, meta)) {
            Log::Error("AssetFactory: Failed to save shader metadata: " + finalPath);
            return false;
        }
        
        Log::Info("AssetFactory: Created shader: " + finalPath);
        return true;
        
    } catch (const std::exception& e) {
        Log::Error("AssetFactory: Failed to create shader: " + std::string(e.what()));
        return false;
    }
}

bool AssetFactory::CreateScript(const std::string& path, const std::string& name) {
    try {
        // Normalize path to absolute path
        std::filesystem::path filePath(path);
        if (!filePath.is_absolute()) {
            // Convert to absolute path
            filePath = std::filesystem::absolute(filePath);
        }
        
        // Create directory if it doesn't exist
        std::filesystem::path dir = filePath.parent_path();
        if (!dir.empty() && !std::filesystem::exists(dir)) {
            std::filesystem::create_directories(dir);
        }
        
        // Generate unique filename if file exists
        std::string finalPath = GenerateUniqueFilename(filePath.string());
        
        // Generate default script content
        std::string scriptName = name.empty() ? std::filesystem::path(finalPath).stem().string() : name;
        std::string content = GenerateDefaultContent(AssetType::Script, scriptName);
        
        // Write script file
        std::ofstream file(finalPath);
        if (!file.is_open()) {
            Log::Error("AssetFactory: Failed to create script file: " + finalPath);
            return false;
        }
        
        file << content;
        file.flush();
        file.close();
        
        // Verify file was written
        if (!std::filesystem::exists(finalPath)) {
            Log::Error("AssetFactory: Script file was not created: " + finalPath);
            return false;
        }
        
        // Create metadata
        LxAssetMeta meta = CreateAssetMeta(AssetType::Script, finalPath, scriptName);
        meta.importer = "ScriptFactory";
        
        if (!AssetMetadata::SaveMetadata(finalPath, meta)) {
            Log::Error("AssetFactory: Failed to save script metadata: " + finalPath);
            return false;
        }
        
        Log::Info("AssetFactory: Created script: " + finalPath);
        return true;
        
    } catch (const std::exception& e) {
        Log::Error("AssetFactory: Failed to create script: " + std::string(e.what()));
        return false;
    }
}

bool AssetFactory::CreateScene(const std::string& path, const std::string& name) {
    try {
        // Normalize path to absolute path
        std::filesystem::path filePath(path);
        if (!filePath.is_absolute()) {
            // Convert to absolute path
            filePath = std::filesystem::absolute(filePath);
        }
        
        // Create directory if it doesn't exist
        std::filesystem::path dir = filePath.parent_path();
        if (!dir.empty() && !std::filesystem::exists(dir)) {
            std::filesystem::create_directories(dir);
        }
        
        // Generate unique filename if file exists
        std::string finalPath = GenerateUniqueFilename(filePath.string());
        
        // Generate default scene content
        std::string sceneName = name.empty() ? std::filesystem::path(finalPath).stem().string() : name;
        std::string content = GenerateDefaultContent(AssetType::Scene, sceneName);
        
        // Write scene file
        std::ofstream file(finalPath);
        if (!file.is_open()) {
            Log::Error("AssetFactory: Failed to create scene file: " + finalPath);
            return false;
        }
        
        file << content;
        file.flush();
        file.close();
        
        // Verify file was written
        if (!std::filesystem::exists(finalPath)) {
            Log::Error("AssetFactory: Scene file was not created: " + finalPath);
            return false;
        }
        
        // Create metadata
        LxAssetMeta meta = CreateAssetMeta(AssetType::Scene, finalPath, sceneName);
        meta.importer = "SceneFactory";
        
        if (!AssetMetadata::SaveMetadata(finalPath, meta)) {
            Log::Error("AssetFactory: Failed to save scene metadata: " + finalPath);
            return false;
        }
        
        Log::Info("AssetFactory: Created scene: " + finalPath);
        return true;
        
    } catch (const std::exception& e) {
        Log::Error("AssetFactory: Failed to create scene: " + std::string(e.what()));
        return false;
    }
}

bool AssetFactory::CreateFolder(const std::string& path, const std::string& name) {
    try {
        // Normalize path to absolute path
        std::filesystem::path folderPath(path);
        if (!folderPath.is_absolute()) {
            // Convert to absolute path
            folderPath = std::filesystem::absolute(folderPath);
        }
        
        if (!name.empty()) {
            folderPath = folderPath / name;
        }
        
        // Generate unique folder name if folder exists
        std::string finalPath = folderPath.string();
        std::string basePath = folderPath.parent_path().string();
        std::string baseName = folderPath.filename().string();
        
        int counter = 1;
        while (std::filesystem::exists(finalPath)) {
            std::string newName = baseName + "_" + std::to_string(counter);
            finalPath = basePath + "/" + newName;
            counter++;
        }
        folderPath = finalPath;
        
        // Create folder
        if (!std::filesystem::exists(folderPath)) {
            std::filesystem::create_directories(folderPath);
        }
        
        // Create metadata for folder
        LxAssetMeta meta = CreateAssetMeta(AssetType::Folder, folderPath.string(), name.empty() ? folderPath.filename().string() : name);
        meta.importer = "FolderFactory";
        
        // Save folder metadata (optional - folders might not need metadata)
        std::string metaPath = folderPath.string() + ".lumameta";
        if (!AssetMetadata::SaveMetadata(metaPath, meta)) {
            Log::Warn("AssetFactory: Failed to save folder metadata: " + metaPath);
        }
        
        Log::Info("AssetFactory: Created folder: " + folderPath.string());
        return true;
        
    } catch (const std::exception& e) {
        Log::Error("AssetFactory: Failed to create folder: " + std::string(e.what()));
        return false;
    }
}

LxAssetMeta AssetFactory::CreateAssetMeta(AssetType type, const std::string& path, const std::string& name) {
    LxAssetMeta meta;
    meta.name = name.empty() ? std::filesystem::path(path).stem().string() : name;
    meta.path = path;
    meta.type = type;
    
    auto now = std::chrono::system_clock::now();
    meta.importTime = std::chrono::system_clock::to_time_t(now);
    meta.modifyTime = meta.importTime;
    
    // Generate GUID
    meta.guid = std::to_string(std::hash<std::string>{}(path + std::to_string(meta.importTime)));
    
    return meta;
}

std::string AssetFactory::GenerateDefaultContent(AssetType type, const std::string& name) {
    switch (type) {
        case AssetType::Material: {
            return "{\n"
                   "  \"Name\": \"" + name + "\",\n"
                   "  \"Type\": \"Material\",\n"
                   "  \"BaseColor\": [0.5, 0.5, 0.5],\n"
                   "  \"Metallic\": 0.0,\n"
                   "  \"Roughness\": 0.5,\n"
                   "  \"Specular\": 0.5,\n"
                   "  \"Emissive\": [0.0, 0.0, 0.0],\n"
                   "  \"Opacity\": 1.0\n"
                   "}";
        }
        
        case AssetType::Shader: {
            return "#version 330 core\n"
                   "// " + name + " Shader\n"
                   "\n"
                   "// Vertex Shader\n"
                   "void main() {\n"
                   "    // Vertex shader code here\n"
                   "}\n"
                   "\n"
                   "// Fragment Shader\n"
                   "out vec4 FragColor;\n"
                   "\n"
                   "void main() {\n"
                   "    FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
                   "}";
        }
        
        case AssetType::Script: {
            return "// " + name + " Script\n"
                   "#include \"LGE/core/GameObject.h\"\n"
                   "#include \"LGE/core/Component.h\"\n"
                   "\n"
                   "namespace LGE {\n"
                   "\n"
                   "class " + name + "Component : public Component {\n"
                   "public:\n"
                   "    void OnUpdate(float deltaTime) override {\n"
                   "        // Update logic here\n"
                   "    }\n"
                   "};\n"
                   "\n"
                   "} // namespace LGE";
        }
        
        case AssetType::Scene: {
            return "{\n"
                   "  \"Name\": \"" + name + "\",\n"
                   "  \"Type\": \"Scene\",\n"
                   "  \"GameObjects\": [],\n"
                   "  \"Camera\": {\n"
                   "    \"Position\": [0.0, 0.0, 3.0],\n"
                   "    \"Target\": [0.0, 0.0, 0.0],\n"
                   "    \"FOV\": 45.0\n"
                   "  },\n"
                   "  \"Lighting\": {\n"
                   "    \"DirectionalLight\": {\n"
                   "      \"Direction\": [0.3, -0.8, 0.5],\n"
                   "      \"Color\": [1.0, 0.98, 0.95],\n"
                   "      \"Intensity\": 0.8\n"
                   "    }\n"
                   "  }\n"
                   "}";
        }
        
        default:
            return "";
    }
}

std::string AssetFactory::GenerateUniqueFilename(const std::string& path) {
    std::filesystem::path filePath(path);
    std::string finalPath = path;
    
    if (!std::filesystem::exists(finalPath)) {
        return finalPath;
    }
    
    std::string basePath = filePath.parent_path().string();
    std::string baseName = filePath.stem().string();
    std::string extension = filePath.extension().string();
    
    int counter = 1;
    while (std::filesystem::exists(finalPath)) {
        std::string newName = baseName + "_" + std::to_string(counter);
        finalPath = basePath + "/" + newName + extension;
        counter++;
    }
    
    return finalPath;
}

} // namespace LGE

