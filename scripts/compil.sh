#!/bin/bash
# Compil using cmake

set -ex

if [[ "$1" == "--no-tidy" ]] || [[ "$2" == "--no-tidy" ]]; then
    cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug -DNO_USE_CLANG_TIDY=true
else
    cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug
fi

if [[ "$1" == "--no-parallel" ]] || [[ "$2" == "--no-parallel" ]]; then
    cmake --build build
else
    NB_PROC=$(nproc)
    NB_THREADS=$(($NB_PROC - 2))
    if [[ "$NB_PROC" -lt "1" ]]; then
        NB_THREADS=1
    fi
    cmake --build build --parallel $NB_THREADS
fi
