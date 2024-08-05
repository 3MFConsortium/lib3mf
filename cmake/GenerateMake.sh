#!/bin/sh

basepath="$(cd "$(dirname "$0")" && pwd)"
builddir="$basepath/../build"
mkdir -p "$builddir"
cd "$builddir"

# Check if we are on macOS
if [ "$(uname)" = "Darwin" ]; then
    # Check if the first argument is Debug mode
    if [ "$1" = "Debug" ]; then
        shift # Remove the first argument to pass remaining arguments
        printf "DARWIN+DEBUG\n"
        cmake .. -G "Unix Makefiles" -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" -DCMAKE_BUILD_TYPE=Debug "$@"
    else
        printf "DARWIN+RELEASE\n"
        cmake .. -G "Unix Makefiles" -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" -DCMAKE_BUILD_TYPE=Release "$@"
    fi
else
    # For Linux and other OSes
    if [ "$1" = "Debug" ]; then
        shift # Remove the first argument to pass remaining arguments
        printf "LINUX+DEBUG\n"
        cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug "$@"
    else
        printf "LINUX+RELEASE\n"
        cmake .. -G "Unix Makefiles" "$@"
    fi
fi