## Enum ConfigFlags

You can use the following flags to configure the window:

- `Raylib::ConfigFlags::ConfigFlags::FLAG_FULLSCREEN_MODE`: Set fullscreen mode.
- `Raylib::ConfigFlags::FLAG_WINDOW_RESIZABLE`: Allow the window to be resized.
- `Raylib::ConfigFlags::FLAG_WINDOW_UNDECORATED`: Disable window decoration (frame and buttons).
- `Raylib::ConfigFlags::FLAG_WINDOW_TRANSPARENT`: Allow the window to be transparent.
- `Raylib::ConfigFlags::FLAG_WINDOW_HIDDEN`: Hide the window.
- `Raylib::ConfigFlags::FLAG_WINDOW_MINIMIZED`: Minimize the window.
- `Raylib::ConfigFlags::FLAG_WINDOW_MAXIMIZED`: Maximize the window.
- `Raylib::ConfigFlags::FLAG_WINDOW_UNFOCUSED`: Disable window focus.
- `Raylib::ConfigFlags::FLAG_WINDOW_TOPMOST`: Set the window to be always on top.
- `Raylib::ConfigFlags::FLAG_WINDOW_HIGHDPI`: Enable high-DPI mode.
- `Raylib::ConfigFlags::FLAG_WINDOW_ALWAYS_RUN`: Allow the window to run in the background.
- `Raylib::ConfigFlags::FLAG_MSAA_4X_HINT`: Enable 4x MSAA.
- `Raylib::ConfigFlags::FLAG_VSYNC_HINT`: Enable V-Sync.

## Example

```cpp
Raylib::setWindowState(Raylib::ConfigFlags::WINDOW_RESIZABLE);
Raylib::setWindowState(Raylib::ConfigFlags::WINDOW_RESIZABLE | Raylib::ConfigFlags::FLAG_VSYNC_HINT);
```
