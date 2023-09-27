# MacOs

- You need to have cmake, and a c++ compiler

*hint: you can use https://github.com/aminya/setup-cpp to install it*

- Git clone the project or download the .zip

`git clone -b main https://github.com/X-R-G-B/R-Bus.git R-Bus-main`

**OR**

`wget https://github.com/X-R-G-B/R-Bus/archive/main.zip && unzip R-Bus-main.zip`

- Change directory

`cd R-Bus-main || "Failed to cd to R-Type-Linux, please open an issue: 'https://github.com/X-R-G-B/R-Bus/issues/new?assignees=&labels=bug&projects=&template=install-failed.yml&title=%5BFAIL+INSTALL%5D+-+Title'"`

- Install dependencies

`./scripts/install-deps-macos.sh`

- Build the project

`./scripts/compil.sh`

Well Done you have your binaries!

Now, you can run it but dont move it anywhere
