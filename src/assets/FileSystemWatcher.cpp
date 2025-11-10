#include "LGE/assets/FileSystemWatcher.h"
#include "LGE/core/Log.h"
#include <filesystem>
#include <chrono>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#include <fileapi.h>
#else
#include <sys/stat.h>
#include <unistd.h>
#endif

namespace LGE {

FileSystemWatcher::FileSystemWatcher()
    : m_IsWatching(false)
    , m_ShouldStop(false)
{
}

FileSystemWatcher::~FileSystemWatcher() {
    StopWatching();
}

bool FileSystemWatcher::WatchDirectory(const std::string& directory) {
    if (m_IsWatching) {
        StopWatching();
    }
    
    if (!std::filesystem::exists(directory) || !std::filesystem::is_directory(directory)) {
        Log::Error("FileSystemWatcher: Directory does not exist: " + directory);
        return false;
    }
    
    m_WatchedDirectory = directory;
    m_IsWatching = true;
    m_ShouldStop = false;
    
    // Build initial file cache
    m_FileCache.clear();
    ProcessDirectory(directory);
    
    // Start watching thread
    m_WatchThread = std::thread(&FileSystemWatcher::WatchThread, this);
    
    Log::Info("FileSystemWatcher: Started watching directory: " + directory);
    return true;
}

void FileSystemWatcher::StopWatching() {
    if (!m_IsWatching) {
        return;
    }
    
    m_ShouldStop = true;
    m_IsWatching = false;
    
    if (m_WatchThread.joinable()) {
        m_WatchThread.join();
    }
    
    m_FileCache.clear();
    Log::Info("FileSystemWatcher: Stopped watching directory");
}

void FileSystemWatcher::SetEventCallback(FileSystemEventCallback callback) {
    std::lock_guard<std::mutex> lock(m_Mutex);
    m_EventCallback = callback;
}

void FileSystemWatcher::Update() {
    // This is called from the main thread
    // The actual watching happens in the background thread
}

void FileSystemWatcher::WatchThread() {
    const auto pollInterval = std::chrono::milliseconds(500); // Poll every 500ms
    
    while (!m_ShouldStop && m_IsWatching) {
        std::this_thread::sleep_for(pollInterval);
        
        if (!m_IsWatching) {
            break;
        }
        
        try {
            // Check for deleted files
            auto it = m_FileCache.begin();
            while (it != m_FileCache.end()) {
                if (!std::filesystem::exists(it->path)) {
                    // File was deleted
                    if (m_EventCallback) {
                        FileSystemEvent event(FileSystemEventType::Deleted, it->path);
                        m_EventCallback(event);
                    }
                    it = m_FileCache.erase(it);
                } else {
                    ++it;
                }
            }
            
            // Check for new/modified files
            ProcessDirectory(m_WatchedDirectory);
            
        } catch (const std::exception& e) {
            Log::Error("FileSystemWatcher: Exception in watch thread: " + std::string(e.what()));
        }
    }
}

void FileSystemWatcher::ProcessDirectory(const std::string& path) {
    try {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
            if (m_ShouldStop) {
                break;
            }
            
            if (ShouldIgnorePath(entry.path().string())) {
                continue;
            }
            
            std::string filePath = entry.path().string();
            uint64_t modTime = GetFileModificationTime(filePath);
            
            // Check if file is already in cache
            auto it = std::find_if(m_FileCache.begin(), m_FileCache.end(),
                [&filePath](const FileInfo& info) {
                    return info.path == filePath;
                });
            
            if (it == m_FileCache.end()) {
                // New file
                m_FileCache.push_back({filePath, modTime});
                if (m_EventCallback) {
                    FileSystemEvent event(FileSystemEventType::Created, filePath);
                    m_EventCallback(event);
                }
            } else {
                // Check if modified
                if (it->lastModified != modTime) {
                    it->lastModified = modTime;
                    if (m_EventCallback) {
                        FileSystemEvent event(FileSystemEventType::Modified, filePath);
                        m_EventCallback(event);
                    }
                }
            }
        }
    } catch (const std::exception& e) {
        Log::Error("FileSystemWatcher: Exception processing directory: " + std::string(e.what()));
    }
}

uint64_t FileSystemWatcher::GetFileModificationTime(const std::string& path) {
    try {
        auto fileTime = std::filesystem::last_write_time(path);
        auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
            fileTime - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now());
        return std::chrono::system_clock::to_time_t(sctp);
    } catch (...) {
        return 0;
    }
}

bool FileSystemWatcher::ShouldIgnorePath(const std::string& path) {
    // Ignore hidden files and directories
    std::string fileName = std::filesystem::path(path).filename().string();
    if (fileName.empty() || fileName[0] == '.') {
        return true;
    }
    
    // Ignore cache and meta directories
    if (path.find("/Cache/") != std::string::npos ||
        path.find("\\Cache\\") != std::string::npos ||
        path.find("/.lumameta") != std::string::npos ||
        path.find("\\.lumameta") != std::string::npos) {
        return true;
    }
    
    return false;
}

} // namespace LGE

