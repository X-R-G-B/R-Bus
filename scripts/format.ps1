#!powershell
# Format c++ files and exit with error if files are not formatted

$argClangFormat = @()

if ($args.Count -gt 0 -and $args[0] -eq '--dry-run') {
    $argClangFormat = @('--dry-run', '--Werror')
}

$files = Get-Childitem –Path src -Include *.cpp,*.hpp -Recurse -ErrorAction SilentlyContinue

clang-format --style=file $argClangFormat -i $files
