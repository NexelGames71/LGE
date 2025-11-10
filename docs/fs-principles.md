# Luma Engine Filesystem Principles

## Phase 0 - Ground Rules

### Root Layout

Every Luma Engine project follows this canonical structure:

```
/<Project>/
  Assets/         # User content (mutable)
  Packages/       # Engine packages, read-only from the editor
  Cache/          # Derived data (thumbnails, compiled shaders)
  Settings/       # Project + editor settings
  Project.lproj   # Manifest (JSON/TOML)
```

### Core Principles

#### 1. Path Handling
- **UTF-8 everywhere**: All paths are UTF-8 encoded
- **Normalize separators**: Internally normalize all path separators to `/`
- **Absolute paths**: Store absolute paths internally; convert to relative only for display/export

#### 2. Asset Identity
- **UUIDv4 for identity**: Never rely on filenames for asset identity
- **GUID-based references**: All asset references use GUIDs, not paths
- **Stable across renames**: Asset identity persists through rename/move operations

#### 3. Safety & Recovery
- **Recycle Bin**: All destructive operations go through a Recycle Bin (soft delete) first
- **Atomic operations**: File writes use atomic operations (write to `.tmp` then rename)
- **Crash recovery**: On startup, clean `.tmp` files and reconcile partial metadata

#### 4. Package Protection
- **Packages/ is sacred**: No delete/rename/move operations allowed from the Content Browser
- **Read-only enforcement**: Packages/ is mounted as read-only at the API level
- **Copy-only**: Assets can be copied from Packages to Assets, but not moved

#### 5. Cross-Platform Support
- **Windows**: Handle long paths with `\\?\` prefix when needed
- **Linux**: Use `inotify` for file watching
- **macOS**: Use `FSEvents` for file watching
- **Case sensitivity**: Treat filesystem as case-sensitive logically (even on Windows)

#### 6. Technology Stack
- **C++20**: Use modern C++ features
- **std::filesystem**: Primary filesystem API
- **Platform layer**: Thin abstraction for file watching and platform-specific operations

### Security & Safety

#### Path Validation
- **Sanitize filenames**: Strip control characters, `:`, `*`, `?`, `<`, `>`, `|`
- **Reserved names**: Block Windows reserved names (CON, PRN, AUX, etc.)
- **Trailing spaces**: Remove trailing spaces and dots from filenames

#### Symlink Protection
- **Never follow symlinks outside project root**: Prevent escape hatch risks
- **Validate paths**: Ensure all resolved paths stay within project boundaries

#### Export Safety
- **User-chosen paths only**: When exporting, write only to paths the user explicitly chose
- **No implicit directories**: Never create directories during export without user consent

### Metadata System

#### .lumameta Files
- **Sibling metadata**: Every engine-native asset has a sibling `.lumameta` file
- **Versioned schema**: Metadata includes a version field for future migrations
- **Source tracking**: Track original source files and import settings

#### Metadata Schema (v1)
```json
{
  "version": 1,
  "guid": "uuidv4-here",
  "type": "Texture2D",
  "sourceFile": "brick.png",
  "importer": "TextureImporter",
  "dependencies": ["Packages:/Luminite/Shaders/PBR.lshader"],
  "tags": ["albedo", "env"],
  "timestamp": 1731123301
}
```

### Threading Model

#### Main Thread
- **UI operations**: All UI work stays on the main thread
- **Event pumping**: File system events are pumped on the main thread

#### Background Threads
- **ThreadPool**: Small thread pool (N threads) for imports, thumbnails, indexing
- **Non-blocking**: Long-running operations (imports, scans) don't freeze the UI
- **Progress events**: Background operations emit progress events via event bus

### Event System

#### EditorBus
- **Thread-safe queue**: Events are queued thread-safely and pumped on main thread
- **Topics**: FS, Import, Registry, Thumbnail, Error
- **Event structure**: Topic, verb, payload (GUID or path), timestamp

### Performance Targets

#### Benchmarks
- **Initial crawl**: < 3s for 10k files (depends on disk)
- **Search**: < 50ms for common queries
- **Thumbnails**: Async generation, progressive loading

#### Scalability
- **10k assets**: Smooth browsing and search
- **50k files**: Acceptable cold scan time and RAM usage
- **Thumbnails**: Async generation with persistent cache

### Migration & Compatibility

#### Versioning
- **Project manifest**: Includes version field
- **Metadata versioning**: Each `.lumameta` includes version
- **In-memory upgrades**: On load, upgrade metadata in-memory and save

#### Backward Compatibility
- **Migration functions**: Keep `migrations/` folder with tiny functions per version bump
- **Graceful degradation**: Handle missing or outdated metadata gracefully

### Definition of Done

Phase 0 is complete when:
- ✅ Everyone agrees "Packages is sacred"
- ✅ UUIDs are the identity mechanism
- ✅ UTF-8 is used everywhere
- ✅ Recycle Bin is the standard for deletes
- ✅ Cross-platform support is planned
- ✅ Security constraints are documented

