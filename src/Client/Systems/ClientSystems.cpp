/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Systems implementation
*/

#include "ClientSystems.hpp"
#include <iostream>
#include "raylib.h"
#include "CustomTypes.hpp"
#include "Registry.hpp"

void GraphicSystems::rectRenderer(std::size_t)
{
    Registry::components<Types::Position> arrPosition =
        Registry::getInstance().getComponents<Types::Position>();
    Registry::components<Types::RectangleShape> arrRect =
        Registry::getInstance().getComponents<Types::RectangleShape>();

    auto positionIt = arrPosition.begin();
    auto rectIt     = arrRect.begin();

    while (positionIt != arrPosition.end() && rectIt != arrRect.end()) {
        if (positionIt->has_value() && rectIt->has_value()) {
            Types::Position &position        = positionIt->value();
            Types::RectangleShape &rectangle = rectIt->value();

            double x = (position.x * GetScreenWidth()) / 100;
            double y = (position.y * GetScreenHeight()) / 100;

            double width  = (rectangle.width * GetScreenWidth()) / 100;
            double height = (rectangle.height * GetScreenHeight()) / 100;

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
            if (IsKeyDown(KEY_RIGHT)) {
                positionIt->value().x += 1;
            }
            if (IsKeyDown(KEY_LEFT)) {
                positionIt->value().x -= 1;
            }
            if (IsKeyDown(KEY_UP)) {
                positionIt->value().y -= 1;
            }
            if (IsKeyDown(KEY_DOWN)) {
                positionIt->value().y += 1;
            }
        }
        positionIt++;
        playerIt++;
    }
}

static void
drawSpriteWithoutRect(Types::Position &position, Types::Sprite &sprite)
{
    float scale       = 1.0f;
    float rotation    = 0;
    Color tint        = WHITE;
    Vector2 spritePos = {0, 0};

    float x = (position.x * GetScreenWidth()) / 100;
    float y = (position.y * GetScreenHeight()) / 100;

    scale     = (sprite.width * GetScreenWidth()) / 100 / sprite.sprite.width;
    spritePos = {x, y};

    DrawTextureEx(sprite.sprite, spritePos, rotation, scale, tint);
}

static void drawSpriteWithRect(
    Types::Position &position,
    Types::Sprite &sprite,
    Types::Rect &rect)
{
    Vector2 origin = {0, 0};
    float rotation = 0;
    Color tint     = WHITE;

    float x = (position.x * GetScreenWidth()) / 100;
    float y = (position.y * GetScreenHeight()) / 100;

    float width  = (sprite.width * GetScreenWidth()) / 100;
    float height = (sprite.height * GetScreenHeight()) / 100;

    DrawTexturePro(
        sprite.sprite,
        Rectangle(rect.x, rect.y, rect.width, rect.height),
        Rectangle(x, y, width, height),
        origin,
        rotation,
        tint);
}

void GraphicSystems::spriteRenderer(std::size_t)
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
