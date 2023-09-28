#pragma once

#include <string>
#include "raylib.h"
#include "Geometry.hpp"
#include "Inputs.hpp"

namespace Raylib {

    enum class ConfigFlags {
        VSYNC_HINT         = 0x00000040,
        FULLSCREEN_MODE    = 0x00000002,
        WINDOW_RESIZABLE   = 0x00000004,
        WINDOW_UNDECORATED = 0x00000008,
        WINDOW_HIDDEN      = 0x00000080,
        WINDOW_MINIMIZED   = 0x00000200,
        WINDOW_MAXIMIZED   = 0x00000400,
        WINDOW_UNFOCUSED   = 0x00000800,
        WINDOW_TOPMOST     = 0x00001000,
        WINDOW_ALWAYS_RUN  = 0x00000100,
        WINDOW_TRANSPARENT = 0x00000010,
        WINDOW_HIGHDPI     = 0x00002000,
        WINDOW_MOUSE_PASSTHROUGH = 0x00004000,
        MSAA_4X_HINT       = 0x00000020,
        INTERLACED_HINT    = 0x00010000
    };

    // Window-related functions
    void initWindow(int width, int height, std::string title);
    bool windowShouldClose();
    void closeWindow();
    bool isWindowReady();
    bool isWindowFullscreen();
    bool isWindowHidden();
    bool isWindowMinimized();
    bool isWindowMaximized();
    bool isWindowFocused();
    void setConfigFlags(ConfigFlags flags);
    bool isWindowResized();
    bool isWindowState(ConfigFlags flag);
    void setWindowState(ConfigFlags flag);
    void clearWindowState(ConfigFlags flags);
    void toggleFullscreen();
    void maximizeWindow();
    void minimizeWindow();
    void setWindowTitle(std::string title);
    int getScreenWidth();
    int getScreenHeight();
    int getRenderWidth();
    int getRenderHeight();
    int getMonitorWidth(int monitor);
    int getMonitorHeight(int monitor);
    int getMonitorRefreshRate(int monitor);
    int getCurrentMonitor();
    void setClipboardText(std::string);
    std::string getClipboardText();
    void setWindowIcon(Image image);

    // Cursor-related functions
    void showCursor();
    void hideCursor();
    bool isCursorHidden();
    void enableCursor();
    void disableCursor();
    bool isCursorOnScreen();

    // Drawing-related functions
    void clearBackground(Raylib::Color color);
    void beginDrawing();
    void endDrawing();

    // Timing-related functions
    void setTargetFPS(int fps);
    int getFPS();
    float getFrameTime();
    double getTime();

    // Misc. functions
    void takeScreenshot(std::string fileName);

    // Input-related functions: keyboard
    bool isKeyPressed(KeyboardKey key);
    bool isKeyDown(KeyboardKey key);
    bool isKeyReleased(KeyboardKey key);
    bool isKeyUp(KeyboardKey key);
    void setExitKey(KeyboardKey key);
    int getKeyPressed();
    int getCharPressed();

    // Input-related functions: mouse
    bool isMouseButtonPressed(MouseButton button);
    bool isMouseButtonDown(MouseButton button);
    bool isMouseButtonReleased(MouseButton button);
    bool isMouseButtonUp(MouseButton button);
    int getMouseX();
    int getMouseY();
    Vector2 getMousePosition();
    Vector2 getMouseDelta();
    void setMousePosition(int x, int y);
    void setMouseOffset(int offsetX, int offsetY);
    void setMouseScale(float scaleX, float scaleY);
    float getMouseWheelMove();
    Vector2 getMouseWheelMoveV();
    void setMouseCursor(int cursor);

    // Shapes-related functions
    void drawPixel(int posX, int posY, Color color);
    void drawCircle(int centerX, int centerY, float radius, Color color);
    void drawRectangle(int posX, int posY, int width, int height, Color color);

    // Color/pixel related functions
    Color fade(Color color, float alpha);
    int colorToInt(Color color);
    Vector4 colorNormalize(Color color);
    Color colorFromNormalized(Vector4 normalized);
    Color getColor(unsigned int hexValue);

    // Graphic classes

    class Image {
        public:
            Image(std::string fileName);
            Image(int width, int height, Color color);
            ~Image();
            bool isImageReady();
            void unloadImage();
            int getWidth() const;
            int getHeight() const;
            int getMipmaps() const;
            int getFormat() const;
            void *getData() const;

        private:
            ::Image _image;
    };

    class Sprite {
        public:
            Sprite(std::string fileName, float width, float height);
            Sprite(Image image, float width, float height);
            unsigned int getId() const;
            float getWidth() const;
            float getHeight() const;
            int getTextureWidth() const;
            int getTextureHeight() const;
            int getMipmaps() const;
            int getFormat() const;
            void unloadSprite();

            // draw texture functions

            void draw(int posX, int posY, Color tint);
            void drawV(Raylib::Vector2 position, Color tint);
            void drawEx(
                Raylib::Vector2 position,
                float rotation,
                float scale,
                Color tint);
            void drawRec(
                Raylib::Rectangle source,
                Raylib::Vector2 position,
                Color tint);
            void drawPro(
                Rectangle source,
                Rectangle dest,
                Vector2 origin,
                float rotation,
                Color tint);

        private:
            void loadTextureFromImage(Image image);
            ::Texture2D _texture;
            // width and height in percentage of the screen
            float _width;
            float _height;
    };

    // Text functions and classes

    class Text {
        public:
            Text(
                std::string text,
                Vector2 position = {0, 0},
                float fontSize   = 5.0F,
                Color color      = BLACK);
            void draw();
            void drawEx(float spacing);
            void drawPro(Vector2 origin, float rotation, float spacing);

            float x() const;
            float y() const;
            float getFontSize() const;
            void setFontSize(float fontSize);
            Vector2 getPosition() const;
            void setPixelPosition(Vector2 position);

        private:
            std::string _text;
            float _fontSize;
            Color _color;
            Vector2 _position;
            Vector2 _pixelPosition;
    };

    void drawFPS(int posX, int posY);

} // namespace Raylib
