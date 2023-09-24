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

static const auto pixelRenderNumber = 50;

void GraphicSystems::pixelRenderer(std::size_t /*unused*/)
{
    Registry::components<Types::Pixel> arrPixel =
        Registry::getInstance().getComponents<Types::Pixel>();
    for (auto &begin : arrPixel) {
        if (!begin.has_value()) {
            continue;
        }
        for (int i = 0; i < pixelRenderNumber; i++) {
            for (int j = 0; j < pixelRenderNumber; j++) {
                DrawPixel(begin.value().x + i, begin.value().y + j, PURPLE);
            }
        }
    }
}

void EventsSystems::playerMovement(std::size_t /*unused*/)
{
    Registry::components<Types::Pixel> arrPixel =
        Registry::getInstance().getComponents<Types::Pixel>();

    for (auto &pixel : arrPixel) {
        if (!pixel.has_value()) {
            continue;
        }
        if (IsKeyDown(KEY_RIGHT)) {
            pixel.value().x += 1;
        }
        if (IsKeyDown(KEY_LEFT)) {
            pixel.value().x -= 1;
        }
        if (IsKeyDown(KEY_UP)) {
            pixel.value().y -= 1;
        }
        if (IsKeyDown(KEY_DOWN)) {
            pixel.value().y += 1;
        }
    }
}

void GraphicSystems::spriteRenderer(std::size_t /*unused*/)
{
    Registry::components<Types::Sprite> arrSprite =
        Registry::getInstance().getComponents<Types::Sprite>();
    Registry::components<Types::Rect> arrRect =
        Registry::getInstance().getComponents<Types::Rect>();
    Registry::components<Types::Position> arrPosition =
        Registry::getInstance().getComponents<Types::Position>();

    for (std::size_t i = 0;
         i < arrSprite.size() || i < arrRect.size() || i < arrPosition.size();
         i++) {
        if (!arrSprite[i].has_value() || !arrRect[i].has_value()
            || !arrPosition[i].has_value()) {
            continue;
        }
        DrawTextureRec(
            arrSprite[i].value().sprite,
            Rectangle(
                arrRect[i].value().x,
                arrRect[i].value().y,
                arrRect[i].value().width,
                arrRect[i].value().height),
            Vector2(arrPosition[i].value().x, arrPosition[i].value().y),
            WHITE);
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
