#!/bin/pwsh
# Compil using cmake

$ErrorActionPreference = 'Stop'

cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

if ($args.Count -gt 0 -and $args[0] -eq '--dry-run') {
    $files = Get-Childitem -Path src -Include *.cpp -Recurse -ErrorAction SilentlyContinue
    clang-tidy -format-style=file --fix --fix-notes -p build $files
}

cmake --build build
