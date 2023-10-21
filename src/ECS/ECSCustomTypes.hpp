/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Custom Types
*/

#pragma once

#include <cstddef>
#include <functional>
#include <optional>
#include "nlohmann/json.hpp"
#include "Registry.hpp"

// all values are in percentage of the screen
namespace Types {

    struct CollisionRect {
            int width;
            int height;

            NLOHMANN_DEFINE_TYPE_INTRUSIVE(CollisionRect, width, height);
    };

    struct Position {
            int x;
            int y;

            NLOHMANN_DEFINE_TYPE_INTRUSIVE(Position, x, y);
    };

    struct Damage {
            int damage;
    };

    struct Velocity {
            int speedX;
            int speedY;

            NLOHMANN_DEFINE_TYPE_INTRUSIVE(Velocity, speedX, speedY);
    };

    struct SpriteDatas {
            SpriteDatas(
                const std::string &fileName,
                int width,
                int height,
                enum LayerType layer,
                std::size_t layerSide)
                : fileName(fileName),
                  width(width),
                  height(height),
                  layer(layer),
                  layerSide(layerSide)
            {
            }
            std::string fileName;
            int width;
            int height;
            std::size_t id;
            enum LayerType layer;
            size_t layerSide;
    };

    struct Dead {
            Dead(std::size_t time = 0)
                : deathFunction(std::nullopt),
                  timeToWait(time),
                  clockId(static_cast<std::size_t>(-1)),
                  launched(false) {};
            Dead(std::optional<std::function<void(std::size_t id)>> func, std::size_t time = 0)
                : deathFunction(func),
                  timeToWait(time),
                  clockId(static_cast<std::size_t>(-1)),
                  launched(false) {};
            std::optional<std::function<void(std::size_t id)>> deathFunction;
            std::size_t timeToWait;
            std::size_t clockId;
            bool launched;
    };

} // namespace Types
