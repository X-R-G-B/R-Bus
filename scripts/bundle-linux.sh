#!/bin/bash
# Run cpack

set -ex

cmake -S . -B build

cmake --build build

cd build || exit 14

cpack --config CPackConfig.cmake -G STGZ

mv R-Type-*-Linux.sh ../r-type-linux.sh

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

tar -cvf r-type-linux.tar bundle

rm -rf bundle
