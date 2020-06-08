#!/bin/sh

cd lib3mf-repo
sh cmake/GenerateMake.sh -DCMAKE_CXX_FLAGS:STRING="-march=native -m64 -Ofast -flto"
cd build
make -j2
ctest -V .
