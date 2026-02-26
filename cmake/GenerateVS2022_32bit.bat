@echo off
set startingDir="%CD%"

set basepath="%~dp0"
set builddir=%basepath%\..\build_32bit
if not exist %builddir% (mkdir %builddir%)
cd %builddir%
cmake -G "Visual Studio 17 2022" -A Win32 .. %*

cd %startingDir%