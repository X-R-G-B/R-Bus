#!/bin/pwsh
# Run cpack

$ErrorActionPreference = "SilentlyContinue"

./scripts/compil.ps1

Copy-Item build/_deps/raylib-build/raylib/raylib.lib build/_deps/raylib-build/raylib/Release/raylib.lib

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_CONFIGURATION_TYPES="Release;Release"

cmake --build build

cd build

cpack --config CPackConfig.cmake -G NSIS

if ($LASTEXITCODE -ne 0) {
    type _CPack_Packages/win64/NSIS/NSISOutput.log
}

Copy-Item R-Type-*-win64.exe ../r-type-windows.exe

cpack --config CPackSourceConfig.cmake -G ZIP

Copy-Item R-Type-*-Source.zip ../r-type-windows.zip
