#include "Graphics.hpp"
#include "raylib.h"

namespace Raylib {

    // Window-related functions

    void initWindow(int width, int height, std::string title)
    {
        InitWindow(width, height, title.c_str());
    }

    bool windowShouldClose()
    {
        return WindowShouldClose();
    }

    void closeWindow()
    {
        CloseWindow();
    }

    bool isWindowReady()
    {
        return IsWindowReady();
    }

    bool isWindowFullscreen()
    {
        return IsWindowFullscreen();
    }

    bool isWindowHidden()
    {
        return IsWindowHidden();
    }

    bool isWindowMinimized()
    {
        return IsWindowMinimized();
    }

    bool isWindowMaximized()
    {
        return IsWindowMaximized();
    }

    bool isWindowFocused()
    {
        return IsWindowFocused();
    }

    bool isWindowResized()
    {
        return IsWindowResized();
    }

    bool isWindowState(size_t flag)
    {
        return IsWindowState(static_cast<unsigned int>(flag));
    }

    void setWindowState(size_t flags)
    {
        SetWindowState(static_cast<unsigned int>(flags));
    }

    void clearWindowState(size_t flags)
    {
        ClearWindowState(static_cast<unsigned int>(flags));
    }

    void toggleFullscreen()
    {
        ToggleFullscreen();
    }

    void maximizeWindow()
    {
        MaximizeWindow();
    }

    void minimizeWindow()
    {
        MinimizeWindow();
    }

    void setWindowTitle(std::string title)
    {
        SetWindowTitle(title.c_str());
    }

    int getScreenWidth()
    {
        return GetScreenWidth();
    }

    int getScreenHeight()
    {
        return GetScreenHeight();
    }

    int getRenderWidth()
    {
        return GetRenderWidth();
    }

    int getRenderHeight()
    {
        return GetRenderHeight();
    }

    int getMonitorWidth(int monitor)
    {
        return GetMonitorWidth(monitor);
    }

    int getMonitorHeight(int monitor)
    {
        return GetMonitorHeight(monitor);
    }

    int getMonitorRefreshRate(int monitor)
    {
        return GetMonitorRefreshRate(monitor);
    }

    void setClipboardText(std::string text)
    {
        SetClipboardText(text.c_str());
    }

    std::string getClipboardText()
    {
        return GetClipboardText();
    }

    void setWindowIcon(Image icon)
    {
        ::Image image;

        image.data    = icon.getData();
        image.width   = icon.getWidth();
        image.height  = icon.getHeight();
        image.mipmaps = icon.getMipmaps();
        image.format  = icon.getFormat();
        SetWindowIcon(image);
    }

    // Cursor-related functions

    void showCursor()
    {
        ShowCursor();
    }

    void hideCursor()
    {
        HideCursor();
    }

    bool isCursorHidden()
    {
        return IsCursorHidden();
    }

    void enableCursor()
    {
        EnableCursor();
    }

    void disableCursor()
    {
        DisableCursor();
    }

    bool isCursorOnScreen()
    {
        return IsCursorOnScreen();
    }

    // Drawing-related functions

    void clearBackground(Raylib::Color color)
    {
        ClearBackground({color.R(), color.G(), color.B(), color.A()});
    }

    void beginDrawing()
    {
        BeginDrawing();
    }

    void endDrawing()
    {
        EndDrawing();
    }

    // Timing-related functions

    void setTargetFPS(int fps)
    {
        SetTargetFPS(fps);
    }

    int getFPS()
    {
        return GetFPS();
    }

    float getFrameTime()
    {
        return GetFrameTime();
    }

    double getTime()
    {
        return GetTime();
    }

    // Misc. functions

    void takeScreenshot(std::string fileName)
    {
        TakeScreenshot(fileName.c_str());
    }

    // Input-related functions: keyboard

    bool isKeyPressed(int key)
    {
        return IsKeyPressed(key);
    }

    bool isKeyDown(int key)
    {
        return IsKeyDown(key);
    }

    bool isKeyReleased(int key)
    {
        return IsKeyReleased(key);
    }

    bool isKeyUp(int key)
    {
        return IsKeyUp(key);
    }

    void setExitKey(int key)
    {
        SetExitKey(key);
    }

    int getKeyPressed()
    {
        return GetKeyPressed();
    }

    int getCharPressed()
    {
        return GetCharPressed();
    }

    // Input-related functions: mouse

    bool isMouseButtonPressed(int button)
    {
        return IsMouseButtonPressed(button);
    }

    bool isMouseButtonDown(int button)
    {
        return IsMouseButtonDown(button);
    }

    bool isMouseButtonReleased(int button)
    {
        return IsMouseButtonReleased(button);
    }

    bool isMouseButtonUp(int button)
    {
        return IsMouseButtonUp(button);
    }

    Vector2 getMousePosition()
    {
        ::Vector2 position = GetMousePosition();
        return Vector2(position.x, position.y);
    }

    int getMouseX()
    {
        return GetMouseX();
    }

    int getMouseY()
    {
        return GetMouseY();
    }

    void setMousePosition(int x, int y)
    {
        SetMousePosition(x, y);
    }

    Vector2 getMouseDelta()
    {
        ::Vector2 delta = GetMouseDelta();
        return Vector2(delta.x, delta.y);
    }

    void setMouseOffset(int offsetX, int offsetY)
    {
        SetMouseOffset(offsetX, offsetY);
    }

    void setMouseScale(float scaleX, float scaleY)
    {
        SetMouseScale(scaleX, scaleY);
    }

    float getMouseWheelMove()
    {
        return GetMouseWheelMove();
    }

    Vector2 getMouseWheelMoveV()
    {
        ::Vector2 move = GetMouseWheelMoveV();
        return Vector2(move.x, move.y);
    }

    void setMouseCursor(int cursor)
    {
        SetMouseCursor(cursor);
    }

    // Shapes-related functions
    void drawPixel(int posX, int posY, Color color)
    {
        DrawPixel(posX, posY, {color.R(), color.G(), color.B(), color.A()});
    }

    void drawCircle(int centerX, int centerY, float radius, Color color)
    {
        DrawCircle(
            centerX,
            centerY,
            radius,
            {color.R(), color.G(), color.B(), color.A()});
    }

    void drawRectangle(int posX, int posY, int width, int height, Color color)
    {
        DrawRectangle(
            posX,
            posY,
            width,
            height,
            {color.R(), color.G(), color.B(), color.A()});
    }

    Image::Image(std::string fileName)
    {
        _image = LoadImage(fileName.c_str());

        if (!isImageReady()) {
            ::Color badTexture                = {255, 16, 240, 255};
            static constexpr int badImageSize = 50;
            _image = GenImageColor(badImageSize, badImageSize, badTexture);
        }
    }

    Image::Image(int width, int height, Color color)
    {
        _image = GenImageColor(
            width,
            height,
            {color.R(), color.G(), color.B(), color.A()});
    }

    Image::~Image()
    {
        if (isImageReady()) {
            unloadImage();
        }
    }

    bool Image::isImageReady()
    {
        return IsImageReady(_image);
    }

    void Image::unloadImage()
    {
        if (isImageReady()) {
            UnloadImage(_image);
        }
    }

    int Image::getWidth()
    {
        return _image.width;
    }

    int Image::getHeight()
    {
        return _image.height;
    }

    int Image::getMipmaps()
    {
        return _image.mipmaps;
    }

    int Image::getFormat()
    {
        return _image.format;
    }

    void *Image::getData()
    {
        return _image.data;
    }

    // Texture functions

    Texture2D::Texture2D(std::string fileName)
    {
        _texture = LoadTexture(fileName.c_str());
        if (!IsTextureReady(_texture)) {
            ::Color badTexture                = {255, 16, 240, 255};
            static constexpr int badImageSize = 50;
            _texture                          = LoadTextureFromImage(
                GenImageColor(badImageSize, badImageSize, badTexture));
        }
    }

    Texture2D::Texture2D(Image image)
    {
        loadTextureFromImage(image);
    }

    void Texture2D::loadTextureFromImage(Image image)
    {
        ::Image img;
        img.data    = image.getData();
        img.width   = image.getWidth();
        img.height  = image.getHeight();
        img.mipmaps = image.getMipmaps();
        img.format  = image.getFormat();
        _texture    = LoadTextureFromImage(img);
    }

    Texture2D::~Texture2D()
    {
        UnloadTexture(_texture);
    }

    int Texture2D::getId()
    {
        return _texture.id;
    }

    int Texture2D::getWidth()
    {
        return _texture.width;
    }

    int Texture2D::getHeight()
    {
        return _texture.height;
    }

    int Texture2D::getMipmaps()
    {
        return _texture.mipmaps;
    }

    int Texture2D::getFormat()
    {
        return _texture.format;
    }

    // Texture drawing functions

    void Texture2D::draw(int posX, int posY, Color tint)
    {
        DrawTexture(
            _texture,
            posX,
            posY,
            {tint.R(), tint.G(), tint.B(), tint.A()});
    }

    void Texture2D::drawV(Vector2 position, Color tint)
    {
        ::Vector2 pos = {position.X(), position.Y()};
        DrawTextureV(_texture, pos, {tint.R(), tint.G(), tint.B(), tint.A()});
    }

    void
    Texture2D::drawEx(Vector2 position, float rotation, float scale, Color tint)
    {
        ::Vector2 pos = {position.X(), position.Y()};
        DrawTextureEx(
            _texture,
            pos,
            rotation,
            scale,
            {tint.R(), tint.G(), tint.B(), tint.A()});
    }

    void Texture2D::drawRec(Rectangle source, Vector2 position, Color tint)
    {
        ::Rectangle src =
            {source.X(), source.Y(), source.Width(), source.Height()};
        ::Vector2 pos = {position.X(), position.Y()};
        DrawTextureRec(
            _texture,
            src,
            pos,
            {tint.R(), tint.G(), tint.B(), tint.A()});
    }

    void Texture2D::drawPro(
        Rectangle source,
        Rectangle dest,
        Vector2 origin,
        float rotation,
        Color tint)
    {
        ::Rectangle src =
            {source.X(), source.Y(), source.Width(), source.Height()};
        ::Rectangle dst = {dest.X(), dest.Y(), dest.Width(), dest.Height()};
        ::Vector2 ori   = {origin.X(), origin.Y()};
        DrawTexturePro(
            _texture,
            src,
            dst,
            ori,
            rotation,
            {tint.R(), tint.G(), tint.B(), tint.A()});
    }

    // Color/pixel related functions
    Color fade(Color color, float alpha)
    {
        ::Color c = {color.R(), color.G(), color.B(), color.A()};
        ::Color f = Fade(c, alpha);
        return Color(f.r, f.g, f.b, f.a);
    }

    int colorToInt(Color color)
    {
        ::Color c = {color.R(), color.G(), color.B(), color.A()};
        return ColorToInt(c);
    }

    Vector4 colorNormalize(Color color)
    {
        ::Color c   = {color.R(), color.G(), color.B(), color.A()};
        ::Vector4 v = ColorNormalize(c);
        return Vector4(v.x, v.y, v.z, v.w);
    }

    Color colorFromNormalized(Vector4 normalized)
    {
        ::Vector4 v =
            {normalized.X(), normalized.Y(), normalized.Z(), normalized.W()};
        ::Color c = ColorFromNormalized(v);
        return Color(c.r, c.g, c.b, c.a);
    }

    Color getColor(unsigned int hexValue)
    {
        ::Color c = GetColor(hexValue);
        return Color(c.r, c.g, c.b, c.a);
    }

    // Text functions
    void
    drawText(std::string text, int posX, int posY, int fontSize, Color color)
    {
        DrawText(
            text.c_str(),
            posX,
            posY,
            fontSize,
            {color.R(), color.G(), color.B(), color.A()});
    }

    void drawFPS(int posX, int posY)
    {
        DrawFPS(posX, posY);
    }

    int measureText(const std::string text, int fontSize)
    {
        return MeasureText(text.c_str(), fontSize);
    }

} // namespace Raylib
