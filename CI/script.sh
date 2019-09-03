#!/bin/sh

ls -ahl
pwd
git status
sh cmake/GenerateMake.sh
cd build
# cmake --build .
# ctest -V .
