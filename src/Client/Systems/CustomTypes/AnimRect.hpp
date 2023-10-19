/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** AnimRect
*/

#pragma once

#include <nlohmann/json.hpp>
#include <unordered_map>

namespace Types {
    enum Direction {NONE, UP, DOWN, RIGHT, LEFT};

    struct Rect {
            float x;
            float y;
            float width;
            float height;

            NLOHMANN_DEFINE_TYPE_INTRUSIVE(Rect, x, y, width, height);
    };

    NLOHMANN_JSON_SERIALIZE_ENUM( Direction, {
        {NONE, nullptr},
        {UP, "up"},
        {DOWN, "down"},
        {RIGHT, "right"},
        {LEFT, "left"},
    });

    enum RectListType {UNDEFINED, DEFAULT_RECT, MOVE, ATTACK, DEAD };

    NLOHMANN_JSON_SERIALIZE_ENUM( RectListType, {
        {UNDEFINED, nullptr},
        {MOVE, "move"},
        {ATTACK, "attack"},
        {DEAD, "dead"},
    });

    class AnimRect {
        public:

            using DirectionVector =  std::unordered_map<Direction, std::vector<Rect>>;
            AnimRect(Rect rect, nlohmann::json animRectData, RectListType state = RectListType::DEFAULT_RECT, Direction direction = Direction::NONE);
            ~AnimRect() = default;

            void changeRectList(RectListType type = RectListType::DEFAULT_RECT);

            void changeDirection(Direction direction = Direction::NONE);

            Rect &getCurrentAnimRect();

        private:
            Rect _defaultRect;
            RectListType _currentRectList;
            Direction _currentDirection;
            std::size_t _currentRectInList;
            std::unordered_map<RectListType, DirectionVector> _animRects;
    };
}
