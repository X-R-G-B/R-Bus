#!/bin/pwsh
# Run cpack

cmake -S . -B build

cmake --build build

cd build

cpack --config CPackConfig.cmake -G NSIS

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
