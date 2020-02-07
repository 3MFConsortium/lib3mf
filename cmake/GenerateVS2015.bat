@echo off
set startingDir="%CD%"

set basepath="%~dp0"
set builddir=%basepath%\..\build
if not exist %builddir% (mkdir %builddir%)
cd %builddir%
cmake -G "Visual Studio 14 2015 Win64" .. %*  -Dgtest_force_shared_crt=ON

cd %startingDir%
