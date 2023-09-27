#!/bin/bash
# Run cpack

set -ex

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

cmake --build build

cd build || exit 14

cpack --config CPackConfig.cmake -G STGZ

mv R-Type-Linux.sh ../r-type-linux.sh

cpack --config CPackSourceConfig.cmake -G TGZ

mv R-Type-Source.tar.gz ../r-type-linux.tar.gz
