#include "Graphics.hpp"
#include "Inputs.hpp"

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

    bool isKeyPressed(KeyboardKey key)
    {
        return IsKeyPressed(static_cast<int>(key));
    }

    bool isKeyDown(KeyboardKey key)
    {
        return IsKeyDown(static_cast<int>(key));
    }

    bool isKeyReleased(KeyboardKey key)
    {
        return IsKeyReleased(static_cast<int>(key));
    }

    bool isKeyUp(KeyboardKey key)
    {
        return IsKeyUp(static_cast<int>(key));
    }

    void setExitKey(KeyboardKey key)
    {
        SetExitKey(static_cast<int>(key));
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

    bool isMouseButtonPressed(MouseButton button)
    {
        return IsMouseButtonPressed(static_cast<int>(button));
    }

    bool isMouseButtonDown(MouseButton button)
    {
        return IsMouseButtonDown(static_cast<int>(button));
    }

    bool isMouseButtonReleased(MouseButton button)
    {
        return IsMouseButtonReleased(static_cast<int>(button));
    }

    bool isMouseButtonUp(MouseButton button)
    {
        return IsMouseButtonUp(static_cast<int>(button));
    }

    Vector2 getMousePosition()
    {
        ::Vector2 position = GetMousePosition();
        return Vector2 {position.x, position.y};
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
        return Vector2 {delta.x, delta.y};
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
        return Vector2 {move.x, move.y};
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

    Image::Image(std::string fileName) : _image(LoadImage(fileName.c_str()))
    {
        if (!isImageReady()) {
            const ::Color badTexture          = {255, 16, 240, 255};
            static constexpr int badImageSize = 50;
            _image = GenImageColor(badImageSize, badImageSize, badTexture);
        }
    }

    Image::Image(int width, int height, Color color)
        : _image(GenImageColor(
            width,
            height,
            {color.R(), color.G(), color.B(), color.A()}))
    {
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

    int Image::getWidth() const
    {
        return _image.width;
    }

    int Image::getHeight() const
    {
        return _image.height;
    }

    int Image::getMipmaps() const
    {
        return _image.mipmaps;
    }

    int Image::getFormat() const
    {
        return _image.format;
    }

    void *Image::getData() const
    {
        return _image.data;
    }

    // Texture functions

    Sprite::Sprite(std::string fileName, float width, float height): _texture(LoadTexture(fileName.c_str())), _width(width), _height(height)
    {
        if (!IsTextureReady(_texture)) {
            static const ::Color badTexture   = {255, 16, 240, 255};
            static constexpr int badImageSize = 50;
            _texture                          = LoadTextureFromImage(
                GenImageColor(badImageSize, badImageSize, badTexture));
        }
    }

    Sprite::Sprite(Image image, float width, float height)
        : _texture({0, 0, 0, 0}), _width(width), _height(height)
    {
        loadTextureFromImage(image);
    }

    void Sprite::loadTextureFromImage(Image image)
    {
        ::Image img;
        img.data    = image.getData();
        img.width   = image.getWidth();
        img.height  = image.getHeight();
        img.mipmaps = image.getMipmaps();
        img.format  = image.getFormat();
        _texture    = LoadTextureFromImage(img);
    }

    Sprite::~Sprite()
    {
        UnloadTexture(_texture);
    }

    unsigned int Sprite::getId() const
    {
        return _texture.id;
    }

    float Sprite::getWidth() const
    {
        return _width;
    }

    float Sprite::getHeight() const
    {
        return _height;
    }

    int Sprite::getTextureWidth() const
    {
        return _texture.width;
    }

    int Sprite::getTextureHeight() const
    {
        return _texture.height;
    }

    int Sprite::getMipmaps() const
    {
        return _texture.mipmaps;
    }

    int Sprite::getFormat() const
    {
        return _texture.format;
    }

    // Texture drawing functions

    void Sprite::draw(int posX, int posY, Color tint)
    {
        DrawTexture(
            _texture,
            posX,
            posY,
            {tint.R(), tint.G(), tint.B(), tint.A()});
    }

    void Sprite::drawV(Vector2 position, Color tint)
    {
        ::Vector2 pos = {position.X(), position.Y()};
        DrawTextureV(_texture, pos, {tint.R(), tint.G(), tint.B(), tint.A()});
    }

    void
    Sprite::drawEx(Vector2 position, float rotation, float scale, Color tint)
    {
        ::Vector2 pos = {position.X(), position.Y()};
        DrawTextureEx(
            _texture,
            pos,
            rotation,
            scale,
            {tint.R(), tint.G(), tint.B(), tint.A()});
    }

    void Sprite::drawRec(Rectangle source, Vector2 position, Color tint)
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

    void Sprite::drawPro(
        Rectangle source,
        Rectangle dest,
        Vector2 origin,
        float rotation,
        Color tint)
    {
        DrawTexturePro(
            _texture,
            {source.X(), source.Y(), source.Width(), source.Height()},
            {dest.X(), dest.Y(), dest.Width(), dest.Height()},
            {origin.X(), origin.Y()},
            rotation,
            {tint.R(), tint.G(), tint.B(), tint.A()});
    }

    // Color/pixel related functions
    Color fade(Color color, float alpha)
    {
        ::Color c = {color.R(), color.G(), color.B(), color.A()};
        ::Color f = Fade(c, alpha);
        return Color {f.r, f.g, f.b, f.a};
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
        return Vector4 {v.x, v.y, v.z, v.w};
    }

    Color colorFromNormalized(Vector4 normalized)
    {
        ::Vector4 v =
            {normalized.X(), normalized.Y(), normalized.Z(), normalized.W()};
        ::Color c = ColorFromNormalized(v);
        return Color {c.r, c.g, c.b, c.a};
    }

    Color getColor(unsigned int hexValue)
    {
        ::Color c = GetColor(hexValue);
        return Color {c.r, c.g, c.b, c.a};
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
