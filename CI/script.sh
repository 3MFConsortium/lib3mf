#!/bin/sh

cd lib3mf-repo
sh cmake/GenerateMake.sh
cd build
make -j2
ctest -V .
