# CICD

## Compilation test

When you modify the `src` directory, the workflow <https://github.com/X-R-G-B/R-Bus/actions/workflows/compil.yml> will run

## Documentation

When you modify the `docs` directory or `book.tml`, the workflow <https://github.com/X-R-G-B/R-Bus/actions/workflows/documentation.yml> will run

## Format

The code is automatically formatted by the workflow <https://github.com/X-R-G-B/R-Bus/actions/workflows/format.yml> when your create a pull request.

If it can't be automatically formated, please use clang-format with the script `./scripts/format.sh` or `./scripts/format.ps1`

## Release

On the main branch, and the dev branch, the workflow <https://github.com/X-R-G-B/R-Bus/actions/workflows/release.yml> will run

In dev branch it don't push the packaged binary or the source code to a release but in the workflow artifact
