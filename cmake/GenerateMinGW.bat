@echo off
set startingDir="%CD%"

set basepath="%~dp0"
set builddir=%basepath%\..\build
if not exist %builddir% (mkdir %builddir%)
cd %builddir%
cmake -G "MinGW Makefiles" .. %*

cd %startingDir%
