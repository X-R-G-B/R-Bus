#!/bin/bash
# Compil using cmake

set -ex

cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

cmake --build build
