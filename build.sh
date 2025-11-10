#!/bin/bash
# LGE Build Script
# This script simplifies building the LGE project

PRESET="default"
CLEAN=false
CONFIGURE_ONLY=false
BUILD=true

# Parse arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -p|--preset)
            PRESET="$2"
            shift 2
            ;;
        -c|--clean)
            CLEAN=true
            shift
            ;;
        --configure-only)
            CONFIGURE_ONLY=true
            BUILD=false
            shift
            ;;
        --no-build)
            BUILD=false
            shift
            ;;
        -h|--help)
            echo "LGE Build Script"
            echo ""
            echo "Usage: ./build.sh [options]"
            echo ""
            echo "Options:"
            echo "  -p, --preset <name>    Build preset to use (default, debug, release, ninja-release, ninja-debug)"
            echo "  -c, --clean            Clean build directory before building"
            echo "  --configure-only       Only configure, don't build"
            echo "  --no-build             Don't build after configuring"
            echo "  -h, --help             Show this help message"
            echo ""
            echo "Examples:"
            echo "  ./build.sh                    # Build using default preset"
            echo "  ./build.sh -p debug            # Build debug configuration"
            echo "  ./build.sh -p release          # Build release configuration"
            echo "  ./build.sh -c                  # Clean and rebuild"
            echo "  ./build.sh --configure-only    # Only configure, don't build"
            exit 0
            ;;
        *)
            echo "Unknown option: $1"
            echo "Use -h or --help for usage information"
            exit 1
            ;;
    esac
done

# Check if CMake is available
if ! command -v cmake &> /dev/null; then
    echo "Error: CMake is not installed or not in PATH"
    echo "Please install CMake from https://cmake.org/download/"
    exit 1
fi

# Check if CMakePresets.json exists
if [ ! -f "CMakePresets.json" ]; then
    echo "Error: CMakePresets.json not found"
    echo "Please ensure you're in the LGE project root directory"
    exit 1
fi

echo "LGE Build Script"
echo "Using preset: $PRESET"
echo ""

# Clean build directory if requested
if [ "$CLEAN" = true ]; then
    echo "Cleaning build directory..."
    if [ -d "build" ]; then
        rm -rf build
        echo "Build directory cleaned"
    fi
fi

# Configure
echo "Configuring CMake with preset: $PRESET..."
if ! cmake --preset "$PRESET"; then
    echo "Error: CMake configuration failed"
    exit 1
fi
echo "Configuration successful"

# Build if requested
if [ "$BUILD" = true ]; then
    echo ""
    echo "Building project..."
    if ! cmake --build --preset "$PRESET"; then
        echo "Error: Build failed"
        exit 1
    fi
    echo "Build successful!"
    echo ""
    echo "Executable location: build/bin/Release/LGE_Example"
fi

