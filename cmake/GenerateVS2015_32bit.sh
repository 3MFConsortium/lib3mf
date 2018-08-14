#!/bin/sh

basepath="$(cd "$(dirname "$0")" && pwd)"
builddir="$basepath/../build_32bit"
mkdir -p $builddir
cd $builddir
cmake .. -G "Visual Studio 14 2015" "$@"
cd $basepath
