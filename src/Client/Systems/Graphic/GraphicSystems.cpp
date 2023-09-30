/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** GraphicSystems
*/

#include "GraphicSystems.hpp"
#include "CustomTypes.hpp"
#include "Raylib.hpp"
#include "Registry.hpp"

namespace Systems {
    void
    GraphicSystems::rectRenderer(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        Registry::components<Types::Position> arrPosition =
            Registry::getInstance().getComponents<Types::Position>();
        Registry::components<Types::RectangleShape> arrRect =
            Registry::getInstance().getComponents<Types::RectangleShape>();
        std::vector<std::size_t> rectShapeIndexes = arrRect.getExistingsId();

        const float denominator = 100.0;

        for (auto id : rectShapeIndexes) {
            if (!arrPosition.exist(id)) {
                continue;
            }
            Types::Position &position        = arrPosition[id];
            Types::RectangleShape &rectangle = arrRect[id];

            float x =
                (position.x * static_cast<float>(Raylib::getScreenWidth()))
                / denominator;
            float y =
                (position.y * static_cast<float>(Raylib::getScreenHeight()))
                / denominator;

            float width =
                (rectangle.width * static_cast<float>(Raylib::getScreenWidth()))
                / denominator;
            float height = (rectangle.height
                            * static_cast<float>(Raylib::getScreenHeight()))
                / denominator;

            DrawRectangle(
                static_cast<int>(x),
                static_cast<int>(y),
                static_cast<int>(width),
                static_cast<int>(height),
                PURPLE);
        }
    }

    static void
    drawSpriteWithoutRect(Types::Position &position, Raylib::Sprite &sprite)
    {
        float scale               = 1.0F;
        float rotation            = 0;
        const Raylib::Color tint  = Raylib::White;
        Raylib::Vector2 spritePos = {0, 0};
        const float denominator   = 100.0;

        float x = (position.x * static_cast<float>(Raylib::getScreenWidth()))
            / denominator;
        float y = (position.y * static_cast<float>(Raylib::getScreenHeight()))
            / denominator;

        scale =
            (sprite.getWidth() * static_cast<float>(Raylib::getScreenWidth()))
            / denominator / static_cast<float>(sprite.getTextureWidth());
        spritePos = {x, y};

        sprite.drawEx(spritePos, rotation, scale, tint);
    }

    static void drawSpriteWithRect(
        Types::Position &position,
        Raylib::Sprite &sprite,
        Types::Rect &rect)
    {
        Raylib::Vector2 origin   = {0, 0};
        float rotation           = 0;
        const Raylib::Color tint = Raylib::White;
        const float denominator  = 100.0;

        float x = (position.x * static_cast<float>(Raylib::getScreenWidth()))
            / denominator;
        float y = (position.y * static_cast<float>(Raylib::getScreenHeight()))
            / denominator;

        float width =
            (sprite.getWidth() * static_cast<float>(Raylib::getScreenWidth()))
            / denominator;
        float height =
            (sprite.getHeight() * static_cast<float>(Raylib::getScreenHeight()))
            / denominator;

        sprite.drawPro(
            Raylib::Rectangle(rect.x, rect.y, rect.width, rect.height),
            Raylib::Rectangle(x, y, width, height),
            origin,
            rotation,
            tint);
    }

    void GraphicSystems::spriteRenderer(
        std::size_t /*unused*/,
        std::size_t /*unused*/)
    {
        Registry::components<Raylib::Sprite> arrSprite =
            Registry::getInstance().getComponents<Raylib::Sprite>();
        Registry::components<Types::Rect> arrRect =
            Registry::getInstance().getComponents<Types::Rect>();
        Registry::components<Types::Position> arrPosition =
            Registry::getInstance().getComponents<Types::Position>();
        std::vector<std::size_t> spriteIndexes = arrSprite.getExistingsId();

        for (auto id : spriteIndexes) {
            if (arrRect.exist(id) && arrPosition.exist(id)) {
                drawSpriteWithRect(arrPosition[id], arrSprite[id], arrRect[id]);
            } else if (arrPosition.exist(id)) {
                drawSpriteWithoutRect(arrPosition[id], arrSprite[id]);
            }
        }
    }

    void GraphicSystems::soundEffectPlayer(
        std::size_t /*unused*/,
        std::size_t /*unused*/)
    {
        Registry::components<Raylib::Sound> arrSoundEffect =
            Registry::getInstance().getComponents<Raylib::Sound>();

        for (auto &soundEffect : arrSoundEffect) {
            if (soundEffect.NeedToPlay()) {
                soundEffect.play();
                soundEffect.setNeedToPlay(false);
            }
        }
    }

    void
    GraphicSystems::musicPlayer(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        Registry::components<Raylib::Music> arrMusics =
            Registry::getInstance().getComponents<Raylib::Music>();

        for (auto &music : arrMusics) {
            if (music.NeedToPlay()) {
                music.play();
                music.setNeedToPlay(false);
            }
            if (music.isPlaying()) {
                music.update();
            }
        }
    }

    static void drawTextResponsive(Raylib::Text &text)
    {
        const float denominator = 100.0;

        float x =
            (text.x() * static_cast<float>(GetScreenWidth())) / denominator;
        float y =
            (text.y() * static_cast<float>(GetScreenHeight())) / denominator;
        float fsz = (text.getFontSize() * static_cast<float>(GetScreenWidth()))
            / denominator;

        text.setPixelPosition({x, y});
        text.setCurrentFontSize(fsz);
        text.draw();
    }

    void
    GraphicSystems::textRenderer(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        Registry::components<Raylib::Text> arrText =
            Registry::getInstance().getComponents<Raylib::Text>();

        for (auto &textIt : arrText) {
            drawTextResponsive(textIt);
        }
    }

    const std::string musicPath  = "assets/Audio/Musics/Title.mp3";
    const std::string soundPath  = "assets/Audio/Sounds/fire.ogg";
    const std::string playerPath = "assets/R-TypeSheet/r-typesheet14.gif";
    const Types::Rect spriteRect = {2, 2, 48, 48};
    const Types::CollisionRect collisionRect = {46, 46};
    const Raylib::Vector2 textPos            = {20, 50};

    void GraphicSystems::playSoundWithKey(
        std::size_t /*unused*/,
        std::size_t /*unused*/)
    {
        Registry::components<Raylib::Music> arrMusics =
            Registry::getInstance().getComponents<Raylib::Music>();
        Registry::components<Raylib::Sound> arrSounds =
            Registry::getInstance().getComponents<Raylib::Sound>();

        for (auto &music : arrMusics) {
            if (music.getPath() == musicPath
                && Raylib::isKeyPressed(Raylib::KeyboardKey::KB_SPACE)) {
                music.setNeedToPlay(true);
            }
        }
        for (auto &sound : arrSounds) {
            if (sound.getPath() == soundPath
                && Raylib::isKeyPressed(Raylib::KeyboardKey::KB_ENTER)) {
                sound.setNeedToPlay(true);
            }
        }
    }

    std::vector<std::function<void(std::size_t, std::size_t)>>
    GraphicSystems::getGraphicsSystems()
    {
        return {
            rectRenderer,
            spriteRenderer,
            textRenderer,
            musicPlayer,
            soundEffectPlayer,
            playSoundWithKey};
    }
} // namespace Systems
