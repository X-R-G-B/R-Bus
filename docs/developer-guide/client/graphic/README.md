# Using Graphics in our game client

This guide explains how to work in your game client with images, sprites, text, ... using our Raylib wrapper classes.

## Sprite Class

### Constructors

```cpp
Raylib::Sprite(const std::string &fileName, float width, float height);
Raylib::Sprite(Image image, float width, float height);
```

Create a sprite object from the specified file and set its width and height or create a sprite from an image object with the given width and height. The width and height are in percentage of the screen.

### Methods

```cpp
unsigned int getId() const;
float getWidth() const;
float getHeight() const;
int getTextureWidth() const;
int getTextureHeight() const;
int getMipmaps() const;
int getFormat() const;
void unloadSprite();
```

### Example usage

```cpp
Raylib::Sprite mySprite("path/to/spritefile.png", 100.0f, 150.0f);
float spriteWidth = mySprite.getWidth();
std::cout << "Sprite width: " << spriteWidth << std::endl;
mySprite.unloadSprite();
```

## Text Class

### Constructors

```cpp
Raylib::Text(
    std::string text,
    Raylib::Vector2 position = {0, 0},
    float fontSize = 5.0f,
    Raylib::Color color = Raylib::BLACK);
```

Create a text object with the given text, position, font size, and color.

### Methods

```cpp
void draw();
void drawEx(float spacing);
void drawPro(Raylib::Vector2 origin, float rotation, float spacing);

float x() const;
float y() const;
float getFontSize() const;
void setFontSize(float fontSize);
Raylib::Vector2 getPosition() const;
void setPixelPosition(Raylib::Vector2 position);
void setCurrentFontSize(float fontSize);
```

getPosition is in percentage of the screen size. For each draw, we recommand to compute and set the position in pixel with setPixelPosition to have a responsive text.

### Example usage

```cpp
Raylib::Text myText("Hello, World!", {100, 200}, 20.0f, Raylib::BLUE);
myText.setPixelPosition({300, 400});
myText.draw();
```

## Image Class

### Constructors

```cpp
Raylib::Image(const std::string& fileName);
Raylib::Image(int width, int height, Raylib::Color color);
```
Create an image object from the specified file or create a blank image with the given width, height, and color.

### Methods

```cpp
bool isImageReady() const;
void unloadImage();
int getWidth() const;
int getHeight() const;
int getMipmaps() const;
int getFormat() const;
void* getData() const;
```

### Example usage

```cpp
Raylib::Image myImage("path/to/imagefile.png");
int width = myImage.getWidth();
std::cout << "Image width: " << width << std::endl;
myImage.unloadImage();
```

## Shapes and drawing functions

```cpp
void drawPixel(int posX, int posY, Raylib::Color color);
void drawCircle(int centerX, int centerY, float radius, Raylib::Color color);
void drawRectangle(int posX, int posY, int width, int height, Raylib::Color color);
```

### Example usage

```cpp
Raylib::drawPixel(100, 100, Raylib::RED);
Raylib::drawCircle(200, 200, 50, Raylib::GREEN);
Raylib::drawRectangle(300, 300, 80, 120, Raylib::BLUE);
```

## Window-related functions

- `void initWindow(int width, int height, const std::string &title);`
  Initialize the game window with the specified width, height, and title.
  
- `bool windowShouldClose();`
  Check if the window should close (user pressed close button or escape key).

- `void closeWindow();`
  Close the game window.

- `bool isWindowReady();`
  Check if the window has been initialized successfully.

- `bool isWindowFullscreen();`
  Check if the window is in fullscreen mode.

- `bool isWindowHidden();`
  Check if the window is currently hidden.

- `bool isWindowMinimized();`
  Check if the window is currently minimized.

- `bool isWindowMaximized();`
  Check if the window is currently maximized.

- `bool isWindowFocused();`
  Check if the window is currently focused.

- `void setConfigFlags(ConfigFlags flags);`
  Set configuration flags for the window.

- `bool isWindowResized();`
  Check if the window has been resized.

- `bool isWindowState(ConfigFlags flag);`
  Check if a specific window state flag is set.

- `void setWindowState(ConfigFlags flag);`
  Set a specific window state flag.

- `void clearWindowState(ConfigFlags flags);`
  Clear specific window state flags.

- `void toggleFullscreen();`
  Toggle fullscreen mode.

- `void maximizeWindow();`
  Maximize the window.

- `void minimizeWindow();`
  Minimize the window.

- `void setWindowTitle(const std::string &title);`
  Set the title of the window.

- `int getScreenWidth();`
  Get the current screen width.

- `int getScreenHeight();`
  Get the current screen height.

- `int getRenderWidth();`
  Get the current rendering width

- `int getRenderHeight();`
  Get the current rendering height

- `int getMonitorWidth(int monitor);`
  Get the width of the specified monitor.

- `int getMonitorHeight(int monitor);`
  Get the height of the specified monitor.

- `int getMonitorRefreshRate(int monitor);`
  Get the refresh rate of the specified monitor.

- `int getCurrentMonitor();`
  Get the index of the current monitor.

- `void setClipboardText(const std::string &text);`
  Set the text to be copied to the clipboard.

- `std::string getClipboardText();`
  Get the text currently copied to the clipboard.

- `void setWindowIcon(Image image);`
  Set the window icon.

## Cursor-related functions

- `void showCursor();`
  Show the cursor.

- `void hideCursor();`
  Hide the cursor.

- `bool isCursorHidden();`
  Check if the cursor is currently hidden.

- `void enableCursor();`
  Enable cursor (unlock it).

- `void disableCursor();`
  Disable cursor (lock it).

- `bool isCursorOnScreen();`
  Check if the cursor is within the game window.

## Drawing-related functions

- `void clearBackground(Raylib::Color color);`
  Clear the background with a specified color.

- `void beginDrawing();`
  Begin drawing.

- `void endDrawing();`
  End drawing and swap buffers.

## Timing-related functions

- `void setTargetFPS(int fps);`
  Set the target frames-per-second.

- `int getFPS();`
  Get the current frames-per-second.

- `float getFrameTime();`
  Get the time in seconds for a frame.

- `double getTime();`
  Get the current time in seconds.

## Misc. functions

- `void takeScreenshot(const std::string &fileName);`
  Take a screenshot and save it to a file with the specified name.

## Shapes-related functions

- `void drawPixel(int posX, int posY, Raylib::Color color);`
  Draw a pixel at the specified position with the given color.

- `void drawCircle(int centerX, int centerY, float radius, Raylib::Color color);`
  Draw a circle with the specified center, radius, and color.

- `void drawRectangle(int posX, int posY, int width, int height, Raylib::Color color);`
  Draw a rectangle at the specified position with the given width, height, and color.

## Color/pixel related functions

- `Raylib::Color fade(Raylib::Color color, float alpha);`
  Fade a color by a specified alpha value.

- `int colorToInt(Raylib::Color color);`
  Convert a Color to a 32-bit integer.

- `Vector4 colorNormalize(Raylib::Color color);`
  Normalize a Color struct to a Vector4.

- `Raylib::Color colorFromNormalized(Vector4 normalized);`
  Create a Color from a normalized Vector4.

- `Raylib::Color getColor(unsigned int hexValue);`
  Create a Color from a hex value.

## Enum ConfigFlags

You can use the following flags to configure the window:

- `FLAG_FULLSCREEN_MODE`: Set fullscreen mode.
- `FLAG_WINDOW_RESIZABLE`: Allow the window to be resized.
- `FLAG_WINDOW_UNDECORATED`: Disable window decoration (frame and buttons).
- `FLAG_WINDOW_TRANSPARENT`: Allow the window to be transparent.
- `FLAG_WINDOW_HIDDEN`: Hide the window.
- `FLAG_WINDOW_MINIMIZED`: Minimize the window.
- `FLAG_WINDOW_MAXIMIZED`: Maximize the window.
- `FLAG_WINDOW_UNFOCUSED`: Disable window focus.
- `FLAG_WINDOW_TOPMOST`: Set the window to be always on top.
- `FLAG_WINDOW_HIGHDPI`: Enable high-DPI mode.
- `FLAG_WINDOW_ALWAYS_RUN`: Allow the window to run in the background.
- `FLAG_MSAA_4X_HINT`: Enable 4x MSAA.
- `FLAG_VSYNC_HINT`: Enable V-Sync.
