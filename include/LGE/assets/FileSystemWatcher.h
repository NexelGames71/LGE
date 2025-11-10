#pragma once

#include <string>
#include <functional>
#include <memory>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>

namespace LGE {

// File system event types
enum class FileSystemEventType {
    Created,
    Modified,
    Deleted,
    Renamed
};

// File system event structure
struct FileSystemEvent {
    FileSystemEventType type;
    std::string path;
    std::string oldPath;  // For rename events
    
    FileSystemEvent(FileSystemEventType t, const std::string& p)
        : type(t), path(p) {}
    
    FileSystemEvent(FileSystemEventType t, const std::string& p, const std::string& old)
        : type(t), path(p), oldPath(old) {}
};

// Callback function type for file system events
using FileSystemEventCallback = std::function<void(const FileSystemEvent&)>;

// File system watcher class
class FileSystemWatcher {
public:
    FileSystemWatcher();
    ~FileSystemWatcher();
    
    // Start watching a directory
    bool WatchDirectory(const std::string& directory);
    
    // Stop watching
    void StopWatching();
    
    // Set callback for file system events
    void SetEventCallback(FileSystemEventCallback callback);
    
    // Check if watching
    bool IsWatching() const { return m_IsWatching; }
    
    // Get watched directory
    std::string GetWatchedDirectory() const { return m_WatchedDirectory; }
    
    // Update (poll for changes - called from main thread)
    void Update();
    
private:
    void WatchThread();
    void ProcessDirectory(const std::string& path);
    
    std::string m_WatchedDirectory;
    std::atomic<bool> m_IsWatching;
    std::atomic<bool> m_ShouldStop;
    std::thread m_WatchThread;
    std::mutex m_Mutex;
    
    FileSystemEventCallback m_EventCallback;
    
    // Store file modification times for comparison
    struct FileInfo {
        std::string path;
        uint64_t lastModified;
    };
    std::vector<FileInfo> m_FileCache;
    
    // Helper function to get file modification time
    uint64_t GetFileModificationTime(const std::string& path);
    
    // Helper function to check if path should be ignored
    bool ShouldIgnorePath(const std::string& path);
};

} // namespace LGE

