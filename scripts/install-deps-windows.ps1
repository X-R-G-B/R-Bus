#!/bin/pwsh
# Install deps

winget list -e LLVM
if ($LASTEXITCODE -ne 0) {
    winget install --accept-package-agreements --accept-source-agreements LLVM -e
}

winget list -e CMake
if ($LASTEXITCODE -ne 0) {
    winget install --accept-package-agreements --accept-source-agreements CMake -e
}
