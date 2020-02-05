#!/bin/sh

basepath="$(cd "$(dirname "$0")" && pwd)"
builddir="$basepath/../build"
mkdir -p "$builddir"
cd "$builddir"
cmake .. -G "Visual Studio 14 2015 Win64" "$@" -Dgtest_force_shared_crt=ON
cd "$basepath"
