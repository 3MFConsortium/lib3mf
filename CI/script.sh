#!/bin/sh

ls -ahl
pwd
cd lib3mf-repo
ls -ahl
pwd
git status
sh cmake/GenerateMake.sh
cd build
# cmake --build .
# ctest -V .
