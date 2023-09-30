# GNU/Linux

```bash
curl -Lo 'r-type-linux.sh' 'https://github.com/X-R-G-B/R-Bus/releases/latest/download/r-type-linux.sh'
# The following line will Accepts the license, extract the packaged binary + its dependencies
yes y | bash 'r-type-linux.sh'
cd R-Type-Linux || echo "Failed to cd to R-Type-Linux, please open an issue: 'https://github.com/X-R-G-B/R-Bus/issues/new?assignees=&labels=bug&projects=&template=install-failed.yml&title=%5BFAIL+INSTALL%5D+-+Title'"
# your binaries are in ./bin/
ls ./bin/
```

:warnings: When you want to move this binaries, please move all the folder R-Type-Linux
