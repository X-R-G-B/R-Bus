/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Graphics
*/

#pragma once

#include <string>
extern "C"
{
#include "raylib.h"
}
#include "Geometry.hpp"
#include "Inputs.hpp"
#include "Registry.hpp"
#include "Graphics.hpp"
#include "ResourcesManager.hpp"

namespace Raylib {

    enum class ConfigFlags {
        VSYNC_HINT               = FLAG_VSYNC_HINT,
        FULLSCREEN_MODE          = FLAG_FULLSCREEN_MODE,
        WINDOW_RESIZABLE         = FLAG_WINDOW_RESIZABLE,
        WINDOW_UNDECORATED       = FLAG_WINDOW_UNDECORATED,
        WINDOW_HIDDEN            = FLAG_WINDOW_HIDDEN,
        WINDOW_MINIMIZED         = FLAG_WINDOW_MINIMIZED,
        WINDOW_MAXIMIZED         = FLAG_WINDOW_MAXIMIZED,
        WINDOW_UNFOCUSED         = FLAG_WINDOW_UNFOCUSED,
        WINDOW_TOPMOST           = FLAG_WINDOW_TOPMOST,
        WINDOW_ALWAYS_RUN        = FLAG_WINDOW_ALWAYS_RUN,
        WINDOW_TRANSPARENT       = FLAG_WINDOW_TRANSPARENT,
        WINDOW_HIGHDPI           = FLAG_WINDOW_HIGHDPI,
        WINDOW_MOUSE_PASSTHROUGH = FLAG_WINDOW_MOUSE_PASSTHROUGH,
        MSAA_4X_HINT             = FLAG_MSAA_4X_HINT,
        INTERLACED_HINT          = FLAG_INTERLACED_HINT,
    };

    class Cursor {
        public:
            static void showCursor()
            {
                ShowCursor();
            }

            static void hideCursor()
            {
                HideCursor();
            }

            static bool isCursorHidden()
            {
                return IsCursorHidden();
            }

            static void enableCursor()
            {
                EnableCursor();
            }

            static void disableCursor()
            {
                DisableCursor();
            }

            static bool isCursorOnScreen()
            {
                return IsCursorOnScreen();
            }

    };

    class Drawing {
        public:
            static void clearBackground(Color color)
            {
                ClearBackground({color.r, color.g, color.b, color.a});
            }

            static void beginDrawing()
            {
                BeginDrawing();
            }

            static void endDrawing()
            {
                EndDrawing();
            }

    };

        // Timing-related functions
    class Frame {
        public:
            static void setTargetFPS(int fps)
            {
                SetTargetFPS(fps);
            }

            static int getFPS()
            {
                return GetFPS();
            }

            static float getFrameTime()
            {
                return GetFrameTime();
            }

            static double getTime()
            {
                return GetTime();
            }

            static void drawFPS(int posX, int posY)
            {
                DrawFPS(posX, posY);
            }
    };

    class Screenshot {
        public:
            static void takeScreenshot(const std::string &fileName)
            {
                TakeScreenshot(fileName.c_str());
            }
    };

    class KeyboardInput {
        public:
            static bool isKeyPressed(KeyboardKey key)
            {
                return IsKeyPressed(static_cast<int>(key));
            }

            static bool isKeyDown(KeyboardKey key)
            {
                return IsKeyDown(static_cast<int>(key));
            }

            static bool isKeyReleased(KeyboardKey key)
            {
                return IsKeyReleased(static_cast<int>(key));
            }

            static bool isKeyUp(KeyboardKey key)
            {
                return IsKeyUp(static_cast<int>(key));
            }

            static void setExitKey(KeyboardKey key)
            {
                SetExitKey(static_cast<int>(key));
            }

            static int getKeyPressed()
            {
                return GetKeyPressed();
            }

            static int getCharPressed()
            {
                return GetCharPressed();
            }
    };

    class MouseInput {
        public:
            static bool isMouseButtonPressed(MouseButton button)
            {
                return IsMouseButtonPressed(static_cast<int>(button));
            }

            static bool isMouseButtonDown(MouseButton button)
            {
                return IsMouseButtonDown(static_cast<int>(button));
            }

            static bool isMouseButtonReleased(MouseButton button)
            {
                return IsMouseButtonReleased(static_cast<int>(button));
            }

            static bool isMouseButtonUp(MouseButton button)
            {
                return IsMouseButtonUp(static_cast<int>(button));
            }

            static Vector2 getMousePosition()
            {
                ::Vector2 position = GetMousePosition();
                return Vector2 {position.x, position.y};
            }

            static int getMouseX()
            {
                return GetMouseX();
            }

            static int getMouseY()
            {
                return GetMouseY();
            }

            static void setMousePosition(int x, int y)
            {
                SetMousePosition(x, y);
            }

            static Vector2 getMouseDelta()
            {
                ::Vector2 delta = GetMouseDelta();
                return Vector2 {delta.x, delta.y};
            }

            static void setMouseOffset(int offsetX, int offsetY)
            {
                SetMouseOffset(offsetX, offsetY);
            }

            static void setMouseScale(float scaleX, float scaleY)
            {
                SetMouseScale(scaleX, scaleY);
            }

            static float getMouseWheelMove()
            {
                return GetMouseWheelMove();
            }

            static Vector2 getMouseWheelMoveV()
            {
                ::Vector2 move = GetMouseWheelMoveV();
                return Vector2 {move.x, move.y};
            }

            static void setMouseCursor(int cursor)
            {
                SetMouseCursor(cursor);
            }
    };

    class DrawShape {
        public:
            static void drawPixel(int posX, int posY, Color color)
            {
                ::Color c = {color.r, color.g, color.b, color.a};

                DrawPixel(posX, posY, c);
            }

            static void drawCircle(int centerX, int centerY, float radius, Color color)
            {
                ::Color c = {color.r, color.g, color.b, color.a};

                DrawCircle(centerX, centerY, radius, c);
            }

            static void drawRectangle(int posX, int posY, int width, int height, Color color)
            {
                ::Color c = {color.r, color.g, color.b, color.a};

                DrawRectangle(posX, posY, width, height, c);
            }
    };

    class RayColor {
        public:
            static Color fade(Color color, float alpha)
            {
                ::Color c = {color.r, color.g, color.b, color.a};
                ::Color f = Fade(c, alpha);
                return Color {f.r, f.g, f.b, f.a};
            }

            static int colorToInt(Color color)
            {
                ::Color c = {color.r, color.g, color.b, color.a};
                return ColorToInt(c);
            }

            static Vector4 colorNormalize(Color color)
            {
                ::Color c   = {color.r, color.g, color.b, color.a};
                ::Vector4 v = ColorNormalize(c);
                return Vector4 {v.x, v.y, v.z, v.w};
            }

            static Color colorFromNormalized(Vector4 normalized)
            {
                ::Vector4 v = {normalized.x, normalized.y, normalized.z, normalized.w};
                ::Color c   = ColorFromNormalized(v);
                return Color {c.r, c.g, c.b, c.a};
            }

            static Color getColor(unsigned int hexValue)
            {
                ::Color c = GetColor(hexValue);
                return Color {c.r, c.g, c.b, c.a};
            }
    };

    class RayImage {
        public:
            RayImage(const std::string &fileName)
                : _image(LoadImage(ECS::ResourcesManager::convertPath(fileName).c_str()))
            {
                if (!isImageReady()) {
                    const ::Color badTexture          = {255, 16, 240, 255};
                    static constexpr int badImageSize = 50;
                    _image                            = GenImageColor(badImageSize, badImageSize, badTexture);
                }
            }

            RayImage(int width, int height, Color color)
                : _image(GenImageColor(width, height, {color.r, color.g, color.b, color.a}))
            {
            }

            ~RayImage()
            {
                if (isImageReady()) {
                    unloadImage();
                }
            }

            bool isImageReady()
            {
                return IsImageReady(_image);
            }

            void unloadImage()
            {
                if (isImageReady()) {
                    UnloadImage(_image);
                }
            }

            int getWidth() const
            {
                return _image.width;
            }

            int getHeight() const
            {
                return _image.height;
            }

            int getMipmaps() const
            {
                return _image.mipmaps;
            }

            int getFormat() const
            {
                return _image.format;
            }

            void setData(RayImage image)
            {
                _image.data = image._image.data;
            }

            static void setExternData(::Image &extern_, RayImage image)
            {
                extern_.data = image._image.data;
            }
        private:
            ::Image _image;
    };

    class Sprite {
        public:
            Sprite(const std::string &fileName, float width, float height, std::size_t id)
                : _texture(LoadTexture(ECS::ResourcesManager::convertPath(fileName).c_str())),
                  _width(width),
                  _height(height)
            {
                if (!IsTextureReady(_texture)) {
                    static const ::Color badTexture   = {255, 16, 240, 255};
                    static constexpr int badImageSize = 50;
                    _texture = LoadTextureFromImage(GenImageColor(badImageSize, badImageSize, badTexture));
                }
                Registry::getInstance().setToDefaultLayer(id);
            }

            Sprite(RayImage image, float width, float height) : _texture(), _width(width), _height(height)
            {
                loadTextureFromImage(image);
            }

            void unloadSprite()
            {
                UnloadTexture(_texture);
            }

            unsigned int getId() const
            {
                return _texture.id;
            }

            float getWidth() const
            {
                return _width;
            }

            float getHeight() const
            {
                return _height;
            }

            int getTextureWidth() const
            {
                return _texture.width;
            }

            int getTextureHeight() const
            {
                return _texture.height;
            }

            int getMipmaps() const
            {
                return _texture.mipmaps;
            }

            int getFormat() const
            {
                return _texture.format;
            }

            // draw texture functions

            void draw(int posX, int posY, Color tint)
            {
                ::Color tnt = {tint.r, tint.g, tint.b, tint.a};

                DrawTexture(_texture, posX, posY, tnt);
            }

            void drawV(Vector2 position, Color tint)
            {
                ::Vector2 pos = {position.x, position.y};
                ::Color tnt   = {tint.r, tint.g, tint.b, tint.a};

                DrawTextureV(_texture, pos, tnt);
            }

            void drawEx(Vector2 position, float rotation, float scale, Color tint)
            {
                ::Vector2 pos = {position.x, position.y};
                ::Color tnt   = {tint.r, tint.g, tint.b, tint.a};

                DrawTextureEx(_texture, pos, rotation, scale, tnt);
            }

            void drawRec(Rectangle source, Vector2 position, Color tint)
            {
                ::Rectangle src = {source.x, source.y, source.width, source.height};
                ::Vector2 pos   = {position.x, position.y};
                ::Color tnt     = {tint.r, tint.g, tint.b, tint.a};

                DrawTextureRec(_texture, src, pos, tnt);
            }

            void drawPro(Rectangle source, Rectangle dest, Vector2 origin, float rotation, Color tint)
            {
                ::Rectangle src = {source.x, source.y, source.width, source.height};
                ::Rectangle dst = {dest.x, dest.y, dest.width, dest.height};
                ::Vector2 org   = {origin.x, origin.y};
                ::Color tnt     = {tint.r, tint.g, tint.b, tint.a};

                DrawTexturePro(_texture, src, dst, org, rotation, tnt);
            }

        private:
            void loadTextureFromImage(RayImage image)
            {
                ::Image img;
                RayImage::setExternData(img, image);
                img.width   = image.getWidth();
                img.height  = image.getHeight();
                img.mipmaps = image.getMipmaps();
                img.format  = image.getFormat();
                _texture    = LoadTextureFromImage(img);
            }
            ::Texture2D _texture;
            // width and height in percentage of the screen
            float _width;
            float _height;
    };

    class Window {
        public:
        static void initWindow(int width, int height, const std::string &title)
        {
            InitWindow(width, height, title.c_str());
        }

        static bool windowShouldClose()
        {
            return WindowShouldClose();
        }

        static void closeWindow()
        {
            CloseWindow();
        }

        static bool isWindowReady()
        {
            return IsWindowReady();
        }

        static bool isWindowFullscreen()
        {
            return IsWindowFullscreen();
        }

        static bool isWindowHidden()
        {
            return IsWindowHidden();
        }

        static bool isWindowMinimized()
        {
            return IsWindowMinimized();
        }

        static bool isWindowMaximized()
        {
            return IsWindowMaximized();
        }

        static bool isWindowFocused()
        {
            return IsWindowFocused();
        }

        static void setConfigFlags(ConfigFlags flags)
        {
            SetConfigFlags(static_cast<unsigned int>(flags));
        }

        static bool isWindowResized()
        {
            return IsWindowResized();
        }

        static bool isWindowState(ConfigFlags flag)
        {
            return IsWindowState(static_cast<unsigned int>(flag));
        }

        static void setWindowState(ConfigFlags flags)
        {
            SetWindowState(static_cast<unsigned int>(flags));
        }

        static void clearWindowState(ConfigFlags flags)
        {
            ClearWindowState(static_cast<unsigned int>(flags));
        }

        static void toggleFullscreen()
        {
            ToggleFullscreen();
        }

        static void maximizeWindow()
        {
            MaximizeWindow();
        }

        static void minimizeWindow()
        {
            MinimizeWindow();
        }

        static void setWindowTitle(const std::string &title)
        {
            SetWindowTitle(title.c_str());
        }

        static int getScreenWidth()
        {
            return GetScreenWidth();
        }

        static int getScreenHeight()
        {
            return GetScreenHeight();
        }

        static int getRenderWidth()
        {
            return GetRenderWidth();
        }

        static int getRenderHeight()
        {
            return GetRenderHeight();
        }

        static int getMonitorWidth(int monitor)
        {
            return GetMonitorWidth(monitor);
        }

        static int getMonitorHeight(int monitor)
        {
            return GetMonitorHeight(monitor);
        }

        static int getMonitorRefreshRate(int monitor)
        {
            return GetMonitorRefreshRate(monitor);
        }

        static int getCurrentMonitor()
        {
            return GetCurrentMonitor();
        }

        static void setClipboardText(const std::string &text)
        {
            SetClipboardText(text.c_str());
        }

        static std::string getClipboardText()
        {
            return GetClipboardText();
        }

        static void setWindowIcon(RayImage icon)
        {
            ::Image image;

            RayImage::setExternData(image, icon);
            image.width   = icon.getWidth();
            image.height  = icon.getHeight();
            image.mipmaps = icon.getMipmaps();
            image.format  = icon.getFormat();
            SetWindowIcon(image);
        }
    };

    // Text functions and classes

    class Text {
        public:

            static void drawText(std::string text, int posX, int posY, int fontSize, Color color)
            {
                ::Color textColor = {color.r, color.g, color.b, color.a};

                DrawText(text.c_str(), posX, posY, fontSize, textColor);
            }

            static int measureText(const std::string text, int fontSize)
            {
                return MeasureText(text.c_str(), fontSize);
            }

            Text(std::string text, Vector2 position, float fontSize, Color color)
                : _text(std::move(text)),
                  _fontSize(fontSize),
                  _currentFontSize(fontSize),
                  _color(color),
                  _position(position),
                  _pixelPosition(position)
            {
            }

            void draw()
            {
                ::Color textColor = {_color.r, _color.g, _color.b, _color.a};

                DrawText(
                    _text.c_str(),
                    static_cast<int>(_pixelPosition.x),
                    static_cast<int>(_pixelPosition.y),
                    static_cast<int>(_currentFontSize),
                    textColor);
            }

            void drawEx(float spacing)
            {
                ::Color textColor = {_color.r, _color.g, _color.b, _color.a};
                ::Vector2 pos     = {_pixelPosition.x, _pixelPosition.y};

                DrawTextEx(GetFontDefault(), _text.c_str(), pos, _currentFontSize, spacing, textColor);
            }

            void drawPro(Vector2 origin, float rotation, float spacing)
            {
                ::Vector2 textOrigin = {origin.x, origin.y};
                ::Vector2 pos        = {_pixelPosition.x, _pixelPosition.y};
                ::Color textColor    = {_color.r, _color.g, _color.b, _color.a};

                DrawTextPro(
                    GetFontDefault(),
                    _text.c_str(),
                    pos,
                    textOrigin,
                    rotation,
                    _currentFontSize,
                    spacing,
                    textColor);
            }


            float x() const
            {
                return _position.x;
            }

            float y() const
            {
                return _position.y;
            }

            float getFontSize() const
            {
                return _fontSize;
            }

            void setFontSize(float fontSize)
            {
                _fontSize = fontSize;
            }

            Vector2 getPosition() const
            {
                return _position;
            }

            void setPixelPosition(Vector2 position)
            {
                _pixelPosition = position;
            }

            void setColor(Color color)
            {
                _color = color;
            }

            Color getColor() const
            {
                return _color;
            }

            void setCurrentFontSize(float fontSize)
            {
                _currentFontSize = fontSize;
            }

        private:
            std::string _text;
            float _fontSize;
            float _currentFontSize;
            Color _color;
            Vector2 _position;
            Vector2 _pixelPosition;
    };
} // namespace Raylib
