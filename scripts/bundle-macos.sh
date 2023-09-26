#!/bin/bash
# Run cpack

set -ex

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

cmake --build build

cd build || exit 14

cpack --config CPackConfig.cmake -G DragNDrop || (cat "./_CPack_Packages/Darwin/DragNDrop/PreinstallOutput.log" && exit 1)

cpack --config CPackSourceConfig.cmake -G ZIP

ls -l
