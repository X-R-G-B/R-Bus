#!/bin/pwsh
# Compil using cmake

$ErrorActionPreference = 'Stop'

cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

cmake --build build

New-Item -Name "build/_deps/raylib-build/raylib/Release" -Type Directory -Force
Copy-Item "build/_deps/raylib-build/raylib/Debug/raylib.lib" "build/_deps/raylib-build/raylib/Release/raylib.lib"
