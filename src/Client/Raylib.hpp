/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Raylib
*/

#pragma once

// encapsulate raylib in a namespace
namespace Raylib
{
    // Window-related functions
    void initWindow(int width, int height, const char *title);
    bool windowShouldClose(void);
    void closeWindow(void);
    bool isWindowReady(void);
    bool isWindowFullscreen(void);
    bool isWindowHidden(void);
    bool isWindowMinimized(void);
    bool isWindowMaximized(void);
    bool isWindowFocused(void);
    bool isWindowResized(void);
    bool isWindowState(unsigned int flag);
    void setWindowState(unsigned int flags);
    void clearWindowState(unsigned int flags);
    void toggleFullscreen(void);
    void maximizeWindow(void);
    void minimizeWindow(void);
    void setWindowIcon(Image image);
    void setWindowIcons(Image *images, int count);
    void setWindowTitle(const char *title);
    int getScreenWidth(void);
    int getScreenHeight(void);
    int getRenderWidth(void);
    int getRenderHeight(void);
    int getMonitorWidth(int monitor);
    int getMonitorHeight(int monitor);
    int getMonitorRefreshRate(int monitor);
    void setClipboardText(const char *text);
    const char *getClipboardText(void);

    // Cursor-related functions
    void showCursor(void);
    void hideCursor(void);
    bool isCursorHidden(void);
    void enableCursor(void);
    void disableCursor(void);
    bool isCursorOnScreen(void);

    // Drawing-related functions
    void clearBackground(Color color);
    void beginDrawing(void);
    void endDrawing(void);

    // Timing-related functions
    void setTargetFPS(int fps);
    int getFPS(void);
    float getFrameTime(void);
    double getTime(void);

    // Misc. functions
    void takeScreenshot(const char *fileName);

    // Input-related functions: keyboard
    bool isKeyPressed(int key);
    bool isKeyDown(int key);
    bool isKeyReleased(int key);
    bool isKeyUp(int key);
    void setExitKey(int key);
    int getKeyPressed(void);
    int getCharPressed(void);

    // Input-related functions: mouse
    bool isMouseButtonPressed(int button);
    bool isMouseButtonDown(int button);
    bool isMouseButtonReleased(int button);
    bool isMouseButtonUp(int button);
    int getMouseX(void);
    int getMouseY(void);
    Vector2 getMousePosition(void);
    Vector2 getMouseDelta(void);
    void setMousePosition(int x, int y);
    void setMouseOffset(int offsetX, int offsetY);
    void setMouseScale(float scaleX, float scaleY);
    float getMouseWheelMove(void);
    Vector2 getMouseWheelMoveV(void);
    void setMouseCursor(int cursor);

    // Shapes-related functions
    void drawPixel(int posX, int posY, Color color);
    void drawCircle(int centerX, int centerY, float radius, Color color);
    void drawRectangle(int posX, int posY, int width, int height, Color color);

    Image loadImage(const char *fileName);
    Image loadImageFromTexture(Texture2D texture);
    Image loadImageFromScreen(void);
    bool isImageReady(Image image);
    void unloadImage(Image image);

    // Texture drawing functions
    void drawTexture(Texture2D texture, int posX, int posY, Color tint);
    void drawTextureV(Texture2D texture, Vector2 position, Color tint);
    void drawTextureEx(Texture2D texture, Vector2 position, float rotation, float scale, Color tint);
    void drawTextureRec(Texture2D texture, Rectangle source, Vector2 position, Color tint);
    void drawTexturePro(Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, Color tint);

    // Color/pixel related functions
    Color fade(Color color, float alpha);
    int colorToInt(Color color);
    Vector4 colorNormalize(Color color);
    Color colorFromNormalized(Vector4 normalized);
    Color getColor(unsigned int hexValue);

    // Font loading/unloading functions
    Font getFontDefault(void);
    Font loadFont(const char *fileName);
    bool isFontReady(Font font);
    void unloadFont(Font font);

    // Text drawing functions
    void drawFPS(int posX, int posY);
    void drawText(const char *text, int posX, int posY, int fontSize, Color color);
    void drawTextEx(Font font, const char *text, Vector2 position, float fontSize, float spacing, Color tint);
    void drawTextPro(Font font, const char *text, Vector2 position, Vector2 origin, float rotation, float fontSize, float spacing, Color tint);

    // Text / fonts
    int measureText(const char *text, int fontSize);
    Vector2 measureTextEx(Font font, const char *text, float fontSize, float spacing);
    char *loadUTF8(const int *codepoints, int length);
    void unloadUTF8(char *text);
    const char *textFormat(const char *text, ...);
    void textAppend(char *text, const char *append, int *position);
}
