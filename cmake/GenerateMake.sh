#!/bin/sh

basepath="$(cd "$(dirname "$0")" && pwd)"
builddir="$basepath/../build"
mkdir -p "$builddir"
cd "$builddir"
cmake .. -G "Unix Makefiles" $@
