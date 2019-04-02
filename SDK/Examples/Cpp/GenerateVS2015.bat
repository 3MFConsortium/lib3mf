@echo off
set startingDir=%CD%

set basepath=%~dp0
set builddir=%basepath%\build
if not exist %builddir% (mkdir %builddir%)
cd %builddir%
cmake -G "Visual Studio 15 2015 Win64" .. %*

cd %startingDir%
