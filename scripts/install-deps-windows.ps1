#!/bin/pwsh
# Install deps

winget install --accept-package-agreements --accept-source-agreements LLVM -e
winget install --accept-package-agreements --accept-source-agreements CMake -e

./deps/vcpkg/bootstrap-vcpkg.bat

./deps/vcpkg/vcpkg.exe integrate install

./scripts/install-package.ps1
