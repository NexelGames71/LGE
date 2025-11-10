# LGE Build Script
# This script simplifies building the LGE project

param(
    [string]$Preset = "default",
    [switch]$Clean = $false,
    [switch]$Configure = $false,
    [switch]$Build = $true,
    [switch]$Help = $false
)

if ($Help) {
    Write-Host "LGE Build Script"
    Write-Host ""
    Write-Host "Usage: .\build.ps1 [options]"
    Write-Host ""
    Write-Host "Options:"
    Write-Host "  -Preset <name>    Build preset to use (default, debug, release, ninja-release, ninja-debug)"
    Write-Host "  -Clean            Clean build directory before building"
    Write-Host "  -Configure        Only configure, don't build"
    Write-Host "  -Build            Build after configuring (default: true)"
    Write-Host "  -Help             Show this help message"
    Write-Host ""
    Write-Host "Examples:"
    Write-Host "  .\build.ps1                    # Build using default preset"
    Write-Host "  .\build.ps1 -Preset debug      # Build debug configuration"
    Write-Host "  .\build.ps1 -Preset release    # Build release configuration"
    Write-Host "  .\build.ps1 -Clean             # Clean and rebuild"
    Write-Host "  .\build.ps1 -Configure         # Only configure, don't build"
    exit 0
}

# Check if CMake is available
$cmake = Get-Command cmake -ErrorAction SilentlyContinue
if (-not $cmake) {
    Write-Host "Error: CMake is not installed or not in PATH" -ForegroundColor Red
    Write-Host "Please install CMake from https://cmake.org/download/" -ForegroundColor Yellow
    exit 1
}

# Check if CMakePresets.json exists
if (-not (Test-Path "CMakePresets.json")) {
    Write-Host "Error: CMakePresets.json not found" -ForegroundColor Red
    Write-Host "Please ensure you're in the LGE project root directory" -ForegroundColor Yellow
    exit 1
}

Write-Host "LGE Build Script" -ForegroundColor Cyan
Write-Host "Using preset: $Preset" -ForegroundColor Cyan
Write-Host ""

# Clean build directory if requested
if ($Clean) {
    Write-Host "Cleaning build directory..." -ForegroundColor Yellow
    if (Test-Path "build") {
        Remove-Item -Recurse -Force "build"
        Write-Host "Build directory cleaned" -ForegroundColor Green
    }
}

# Configure
Write-Host "Configuring CMake with preset: $Preset..." -ForegroundColor Yellow
$configureResult = cmake --preset $Preset
if ($LASTEXITCODE -ne 0) {
    Write-Host "Error: CMake configuration failed" -ForegroundColor Red
    exit 1
}
Write-Host "Configuration successful" -ForegroundColor Green

# Build if requested
if ($Build) {
    Write-Host ""
    Write-Host "Building project..." -ForegroundColor Yellow
    $buildResult = cmake --build --preset $Preset
    if ($LASTEXITCODE -ne 0) {
        Write-Host "Error: Build failed" -ForegroundColor Red
        exit 1
    }
    Write-Host "Build successful!" -ForegroundColor Green
    Write-Host ""
    Write-Host "Executable location: build\bin\Release\LGE_Example.exe" -ForegroundColor Cyan
}

