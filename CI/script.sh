#!/bin/sh

sh cmake/GenerateMake.sh
cd build
cmake --build .
ctest -V .
