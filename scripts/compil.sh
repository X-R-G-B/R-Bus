#!/bin/bash
# Compil using cmake

set -ex

cmake --build build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
