#pragma once

#include <string>
#include "raylib.h"
#include "Geometry.hpp"
#include "Inputs.hpp"

namespace Raylib {

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
    void setConfigFlags(size_t flags);
    bool isWindowResized();
    bool isWindowState(size_t flag);
    void setWindowState(size_t flags);
    void clearWindowState(size_t flags);
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

    // Text functions
    void drawFPS(int posX, int posY);
    void
    drawText(std::string text, int posX, int posY, int fontSize, Color color);
    int measureText(const std::string text, int fontSize);

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
            void
            drawEx(Raylib::Vector2 position, float rotation, float scale, Color tint);
            void drawRec(Raylib::Rectangle source, Raylib::Vector2 position, Color tint);
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

} // namespace Raylib
