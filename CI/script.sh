#!/bin/sh

cd lib3mf-repo
sh cmake/GenerateMake.sh
cd build
cmake --build .
ctest -V .
