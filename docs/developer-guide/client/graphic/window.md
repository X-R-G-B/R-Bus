## Window-related functions

- `void Raylib::initWindow(int width, int height, const std::string &title);`
  Initialize the game window with the specified width, height, and title.
  
- `bool Raylib::windowShouldClose();`
  Check if the window should close (user pressed close button or escape key).

- `void Raylib::closeWindow();`
  Close the game window.

- `bool Raylib::isWindowReady();`
  Check if the window has been initialized successfully.

- `bool Raylib::isWindowFullscreen();`
  Check if the window is in fullscreen mode.

- `bool Raylib::isWindowHidden();`
  Check if the window is currently hidden.

- `bool Raylib::isWindowMinimized();`
  Check if the window is currently minimized.

- `bool Raylib::isWindowMaximized();`
  Check if the window is currently maximized.

- `bool Raylib::isWindowFocused();`
  Check if the window is currently focused.

- `void Raylib::setConfigFlags(ConfigFlags flags);`
  Set configuration flags for the window.

- `bool Raylib::isWindowResized();`
  Check if the window has been resized.

- `bool Raylib::isWindowState(ConfigFlags flag);`
  Check if a specific window state flag is set.

- `void Raylib::setWindowState(ConfigFlags flag);`
  Set a specific window state flag.

- `void Raylib::clearWindowState(ConfigFlags flags);`
  Clear specific window state flags.

- `void Raylib::toggleFullscreen();`
  Toggle fullscreen mode.

- `void Raylib::maximizeWindow();`
  Maximize the window.

- `void Raylib::minimizeWindow();`
  Minimize the window.

- `void Raylib::setWindowTitle(const std::string &title);`
  Set the title of the window.

- `int Raylib::getScreenWidth();`
  Get the current screen width.

- `int Raylib::getScreenHeight();`
  Get the current screen height.

- `int Raylib::getRenderWidth();`
  Get the current rendering width

- `int Raylib::getRenderHeight();`
  Get the current rendering height

- `int Raylib::getMonitorWidth(int monitor);`
  Get the width of the specified monitor.

- `int Raylib::getMonitorHeight(int monitor);`
  Get the height of the specified monitor.

- `int Raylib::getMonitorRefreshRate(int monitor);`
  Get the refresh rate of the specified monitor.

- `int Raylib::getCurrentMonitor();`
  Get the index of the current monitor.

- `void Raylib::setClipboardText(const std::string &text);`
  Set the text to be copied to the clipboard.

- `std::string Raylib::getClipboardText();`
  Get the text currently copied to the clipboard.

- `void Raylib::setWindowIcon(Image image);`
  Set the window icon.
