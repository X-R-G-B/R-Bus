#!/bin/pwsh
# Format c++ files and exit with error if files are not formatted

$argClangFormat = @()

if ($args.Count -gt 0 -and $args[0] -eq '--dry-run') {
    $argClangFormat = @('--dry-run', '--Werror')
}

$files = Get-Childitem -Path src -Include *.cpp,*.hpp -Recurse -ErrorAction SilentlyContinue

clang-format --style=file $argClangFormat -i $files

# $found = Select-String -Path $files -Pattern '"raylib.h"' -CaseSensitive
#
# if ($found) {
#     echo $found
#     exit 1
# }
#
# $found = Select-String -Path $files -Pattern '"raylib.hpp"' -CaseSensitive
#
# if ($found) {
#     echo $found
#     exit 1
# }
