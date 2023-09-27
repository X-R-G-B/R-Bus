/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Systems implementation
*/

#include "ClientSystems.hpp"
#include <iostream>
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

    void EventsSystems::playerMovement(std::size_t /*unused*/)
    {
        Registry::components<Types::Position> arrPosition =
            Registry::getInstance().getComponents<Types::Position>();

        Registry::components<Types::Player> arrPlayer =
            Registry::getInstance().getComponents<Types::Player>();

        auto positionIt = arrPosition.begin();
        auto playerIt   = arrPlayer.begin();

        while (positionIt != arrPosition.end() && playerIt != arrPlayer.end()) {
            if (playerIt->has_value() && positionIt->has_value()
                && playerIt->value().isMine) {
                if (Raylib::isKeyDown(Raylib::KeyboardKey::KB_RIGHT)) {
                    positionIt->value().x += 1;
                }
                if (Raylib::isKeyDown(Raylib::KeyboardKey::KB_LEFT)) {
                    positionIt->value().x -= 1;
                }
                if (Raylib::isKeyDown(Raylib::KeyboardKey::KB_UP)) {
                    positionIt->value().y -= 1;
                }
                if (Raylib::isKeyDown(Raylib::KeyboardKey::KB_DOWN)) {
                    positionIt->value().y += 1;
                }
            }
            positionIt++;
            playerIt++;
        }
    }

    static void
    drawSpriteWithoutRect(Types::Position &position, Raylib::Sprite &sprite)
    {
        float scale               = 1.0F;
        float rotation            = 0;
        Raylib::Color tint        = Raylib::White;
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
        Raylib::Vector2 origin  = {0, 0};
        float rotation          = 0;
        Raylib::Color tint      = Raylib::White;
        const float denominator = 100.0;

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
        Registry::components<Types::SoundEffect> arrSoundEffect =
            Registry::getInstance().getComponents<Types::SoundEffect>();

        for (auto &soundEffect : arrSoundEffect) {
            if (!soundEffect.has_value()) {
                continue;
            }
            if (soundEffect.value().needToPlay) {
                PlaySound(soundEffect.value().sound);
                soundEffect.value().needToPlay = false;
            }
        }
    }

    void GraphicSystems::musicPlayer(std::size_t /*unused*/)
    {
        Registry::components<Types::MusicStream> arrMusics =
            Registry::getInstance().getComponents<Types::MusicStream>();

        for (auto &music : arrMusics) {
            if (!music.has_value()) {
                continue;
            }
            if (music.value().needToPlay) {
                PlayMusicStream(music.value().music);
                music.value().needToPlay = false;
                music.value().isPlaying  = true;
            }
            if (music.value().isPlaying) {
                UpdateMusicStream(music.value().music);
            }
        }
    }

    static void drawTextResponsive(Types::Position &position, Types::Text &text)
    {
        const float denominator = 100.0;

        float x =
            (position.x * static_cast<float>(GetScreenWidth())) / denominator;
        float y =
            (position.y * static_cast<float>(GetScreenHeight())) / denominator;

        float fsz = (text.fontSize * static_cast<float>(GetScreenWidth()))
            / denominator;

        DrawTextEx(
            text.font,
            text.text.data(),
            Vector2(x, y),
            fsz,
            0,
            text.color);
    }

    void GraphicSystems::textRenderer(std::size_t /*unused*/)
    {
        Registry::components<Types::Text> arrText =
            Registry::getInstance().getComponents<Types::Text>();
        Registry::components<Types::Position> arrPosition =
            Registry::getInstance().getComponents<Types::Position>();

        auto positionIt = arrPosition.begin();
        auto textIt     = arrText.begin();

        while (positionIt != arrPosition.end() && textIt != arrText.end()) {
            if (textIt->has_value() && positionIt->has_value()) {
                drawTextResponsive(positionIt->value(), textIt->value());
            }
            positionIt++;
            textIt++;
        }
    }

    const std::vector<std::function<void(std::size_t)>>
        GraphicSystems::graphicSystems {
            rectRenderer,
            spriteRenderer,
            textRenderer,
            musicPlayer,
            soundEffectPlayer};

    const std::vector<std::function<void(std::size_t)>>
        EventsSystems::eventSystems {playerMovement};

} // namespace Systems
