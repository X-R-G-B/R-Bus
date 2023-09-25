#!/bin/pwsh
# Run cpack

$ErrorActionPreference = "SilentlyContinue"

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

cmake --build build

cd build

cpack --config CPackConfig.cmake -G NSIS

if ($LASTEXITCODE -ne 0) {
    type _CPack_Packages/win64/NSIS/NSISOutput.log
}

Copy-Item R-Type-*-win64.exe ../r-type-windows.exe

cd ..

New-Item -Name "bundle" -Type Directory

Copy-Item assets bundle -Recurse -Force
Copy-Item scripts bundle -Recurse -Force
Copy-Item src bundle -Recurse -Force
Copy-Item deps bundle -Recurse -Force
Copy-Item CMakeLists.txt bundle -Recurse -Force
Copy-Item LICENSE bundle -Recurse -Force
Copy-Item LICENSE.txt bundle -Recurse -Force
Copy-Item README.md bundle -Recurse -Force

Compress-Archive -Path bundle -DestinationPath r-type-windows.zip

Remove-Item -Recurse -Force bundle
