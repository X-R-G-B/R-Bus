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

    void GraphicSystems::rectRenderer(std::size_t /*unused*/)
    {
        Registry::components<Types::Position> arrPosition =
            Registry::getInstance().getComponents<Types::Position>();
        Registry::components<Types::RectangleShape> arrRect =
            Registry::getInstance().getComponents<Types::RectangleShape>();

        const float denominator = 100.0;

        auto positionIt = arrPosition.begin();
        auto rectIt     = arrRect.begin();

        while (positionIt != arrPosition.end() && rectIt != arrRect.end()) {
            if (positionIt->has_value() && rectIt->has_value()) {
                Types::Position &position        = positionIt->value();
                Types::RectangleShape &rectangle = rectIt->value();

                float x =
                    (position.x * static_cast<float>(Raylib::getScreenWidth()))
                    / denominator;
                float y =
                    (position.y * static_cast<float>(Raylib::getScreenHeight()))
                    / denominator;

                float width = (rectangle.width
                               * static_cast<float>(Raylib::getScreenWidth()))
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
            positionIt++;
            rectIt++;
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

    void GraphicSystems::spriteRenderer(std::size_t /*unused*/)
    {
        Registry::components<Raylib::Sprite> arrSprite =
            Registry::getInstance().getComponents<Raylib::Sprite>();
        Registry::components<Types::Rect> arrRect =
            Registry::getInstance().getComponents<Types::Rect>();
        Registry::components<Types::Position> arrPosition =
            Registry::getInstance().getComponents<Types::Position>();

        auto positionIt = arrPosition.begin();
        auto spriteIt   = arrSprite.begin();
        auto rectIt     = arrRect.begin();

        while (positionIt != arrPosition.end() && spriteIt != arrSprite.end()) {
            if (positionIt->has_value() && spriteIt->has_value()
                && rectIt->has_value()) {
                drawSpriteWithRect(
                    positionIt->value(),
                    spriteIt->value(),
                    rectIt->value());
            } else if (positionIt->has_value() && spriteIt->has_value()) {
                drawSpriteWithoutRect(positionIt->value(), spriteIt->value());
            }
            positionIt++;
            spriteIt++;
            rectIt++;
        }
    }

    void GraphicSystems::soundEffectPlayer(std::size_t /*unused*/)
    {
        Registry::components<Raylib::Sound> arrSoundEffect =
            Registry::getInstance().getComponents<Raylib::Sound>();

        for (auto &soundEffect : arrSoundEffect) {
            if (!soundEffect.has_value()) {
                continue;
            }
            if (soundEffect.value().NeedToPlay()) {
                soundEffect.value().play();
                soundEffect.value().setNeedToPlay(false);
            }
        }
    }

    void GraphicSystems::musicPlayer(std::size_t /*unused*/)
    {
        Registry::components<Raylib::Music> arrMusics =
            Registry::getInstance().getComponents<Raylib::Music>();

        for (auto &music : arrMusics) {
            if (!music.has_value()) {
                continue;
            }
            if (music.value().NeedToPlay()) {
                music.value().play();
                music.value().setNeedToPlay(false);
            }
            if (music.value().isPlaying()) {
                music.value().update();
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

    void GraphicSystems::textRenderer(std::size_t /*unused*/)
    {
        Registry::components<Raylib::Text> arrText =
            Registry::getInstance().getComponents<Raylib::Text>();

        auto textIt = arrText.begin();

        while (textIt != arrText.end()) {
            if (textIt->has_value()) {
                drawTextResponsive(textIt->value());
            }
            textIt++;
        }
    }

    std::vector<std::function<void(std::size_t)>>
    GraphicSystems::getGraphicsSystems()
    {
        return {
            rectRenderer,
            spriteRenderer,
            textRenderer,
            musicPlayer,
            soundEffectPlayer};
    }
} // namespace Systems
