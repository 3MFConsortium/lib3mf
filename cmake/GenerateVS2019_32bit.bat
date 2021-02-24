@echo off
set startingDir="%CD%"

set basepath="%~dp0"
set builddir=%basepath%\..\build_32bit
if not exist %builddir% (mkdir %builddir%)
cd %builddir%
cmake -G "Visual Studio 16 2019" -A Win32 .. %*

cd %startingDir%
