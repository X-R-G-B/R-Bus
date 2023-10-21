#!/bin/bash
# Compil using cmake

set -ex

if [[ "$1" == "--no-tidy" ]]; then
    cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug -DNO_USE_CLANG_TIDY=true
else
    cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug
fi

cmake --build build --parallel
