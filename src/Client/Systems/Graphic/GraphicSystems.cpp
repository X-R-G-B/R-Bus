/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** GraphicSystems
*/

#include "raylib.h"
#include "GraphicSystems.hpp"
#include "CustomTypes.hpp"
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

                float x = (position.x * static_cast<float>(GetScreenWidth()))
                    / denominator;
                float y = (position.y * static_cast<float>(GetScreenHeight()))
                    / denominator;

                float width =
                    (rectangle.width * static_cast<float>(GetScreenWidth()))
                    / denominator;
                float height =
                    (rectangle.height * static_cast<float>(GetScreenHeight()))
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
    drawSpriteWithoutRect(Types::Position &position, Types::Sprite &sprite)
    {
        float scale             = 1.0F;
        float rotation          = 0;
        auto tint               = WHITE;
        Vector2 spritePos       = {0, 0};
        const float denominator = 100.0;

        float x =
            (position.x * static_cast<float>(GetScreenWidth())) / denominator;
        float y =
            (position.y * static_cast<float>(GetScreenHeight())) / denominator;

        scale = (sprite.width * static_cast<float>(GetScreenWidth()))
            / denominator / static_cast<float>(sprite.sprite.width);
        spritePos = {x, y};

        DrawTextureEx(sprite.sprite, spritePos, rotation, scale, tint);
    }

    static void drawSpriteWithRect(
        Types::Position &position,
        Types::Sprite &sprite,
        Types::Rect &rect)
    {
        Vector2 origin          = {0, 0};
        float rotation          = 0;
        auto tint               = WHITE;
        const float denominator = 100.0;

        float x =
            (position.x * static_cast<float>(GetScreenWidth())) / denominator;
        float y =
            (position.y * static_cast<float>(GetScreenHeight())) / denominator;

        float width =
            (sprite.width * static_cast<float>(GetScreenWidth())) / denominator;
        float height = (sprite.height * static_cast<float>(GetScreenHeight()))
            / denominator;

        DrawTexturePro(
            sprite.sprite,
            Rectangle(rect.x, rect.y, rect.width, rect.height),
            Rectangle(x, y, width, height),
            origin,
            rotation,
            tint);
    }

    void GraphicSystems::spriteRenderer(std::size_t /*unused*/)
    {
        Registry::components<Types::Sprite> arrSprite =
            Registry::getInstance().getComponents<Types::Sprite>();
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
    const std::vector<std::function<void(std::size_t)>> GraphicSystems::getGraphicsSystems()
    {
        return {
            rectRenderer,
            spriteRenderer,
            textRenderer,
            musicPlayer,
            soundEffectPlayer
        };
    }
} // namespace Systems
