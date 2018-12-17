#!/bin/sh

basepath="$(cd "$(dirname "$0")" && pwd)"
builddir="$basepath/../build"
mkdir -p "$builddir"
cd "$builddir"
echo "SET(CMAKE_SYSTEM_NAME Windows)" > toolchain.cmake &&
echo "SET(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)" >> toolchain.cmake &&
echo "SET(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)" >> toolchain.cmake &&
echo "SET(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)" >> toolchain.cmake &&
echo "SET(CMAKE_FIND_ROOT_PATH /usr/mingw64)" >> toolchain.cmake &&
echo "SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)" >> toolchain.cmake &&
echo "SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)" >> toolchain.cmake &&
echo "SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)" >> toolchain.cmake &&
cmake .. -DCMAKE_TOOLCHAIN_FILE=toolchain.cmake "$@"
