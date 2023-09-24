#!/bin/pwsh
# Install deps

winget install --accept-package-agreements --accept-source-agreements LLVM -e
winget install --accept-package-agreements --accept-source-agreements CMake -e
