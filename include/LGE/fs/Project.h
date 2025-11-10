#pragma once

#include <string>
#include <filesystem>

namespace LGE {

// Project paths structure - canonical paths for a Luma Engine project
struct ProjectPaths {
    std::string root;        // Project root directory
    std::string assets;      // Assets/ directory (user content)
    std::string packages;    // Packages/ directory (engine packages, read-only)
    std::string cache;       // Cache/ directory (derived data)
    std::string settings;    // Settings/ directory (project + editor settings)
    std::string recycleBin;  // Cache/RecycleBin directory
    std::string projectFile; // Project.lproj manifest file path
};

// Project manifest structure
struct ProjectManifest {
    int version;             // Manifest version
    std::string name;        // Project name
    std::string engineVersion; // Engine version
    std::string guid;        // Project GUID (UUIDv4)
    
    // Default constructor
    ProjectManifest() : version(1), name(""), engineVersion("0.1.0-dev"), guid("") {}
};

namespace LxFS {

// Initialize a project at the given root directory
// Creates the folder structure if it doesn't exist
// Returns true on success, false on failure
bool InitializeProject(const std::string& root);

// Get the current project paths
// Returns empty paths if no project is initialized
const ProjectPaths& Paths();

// Validate that the current project structure is correct
// Checks for required directories and Project.lproj manifest
// Returns true if valid, false otherwise
bool ValidateProject();

// Ensure the project scaffold exists (create missing folders)
// Never deletes existing folders
// If Packages/ is missing, creates empty and logs WARN (not error)
void EnsureScaffold();

// Load project manifest from Project.lproj
// Returns true on success, false on failure
bool LoadManifest(ProjectManifest& manifest);

// Save project manifest to Project.lproj
// Returns true on success, false on failure
bool SaveManifest(const ProjectManifest& manifest);

// Check if a project is initialized
bool IsInitialized();

// Clear the current project (for cleanup/testing)
void ClearProject();

} // namespace LxFS

} // namespace LGE

