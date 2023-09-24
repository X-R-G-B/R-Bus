#!/bin/bash
# Run cpack

set -ex

cmake -S . -B build

cd build || exit 14

cpack --config CPackConfig.cmake -G DragNDrop

cd ..

mkdir bundle

cp -r assets bundle
cp -r scripts bundle
cp -r src bundle
cp -r deps bundle
cp -r CMakeLists.txt bundle
cp -r LICENSE.txt bundle
cp -r LICENSE bundle
cp -r README.md bundle

zip -r -9 r-type-macos.zip bundle
