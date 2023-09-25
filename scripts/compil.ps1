#!/bin/pwsh
# Compil using cmake

$ErrorActionPreference = 'Stop'

cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

cmake --build build