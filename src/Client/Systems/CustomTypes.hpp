/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Custom Types
*/

#pragma once

#include <string>
#include <vector>
#include "ECSCustomTypes.hpp"

namespace Types {

    struct Rect {
            float x;
            float y;
            float width;
            float height;
    };

    struct FontSize {
        float fsz;
    };

    enum RectListType { DEFAULTRECT, MOVE, ATTACK, DEAD };

    struct AnimRect {
        public:
            AnimRect(
                Rect rect,
                std::vector<Rect> _moveRects,
                std::vector<Rect> _attackRects = {},
                std::vector<Rect> _deadRects   = {})
                : defaultRect(rect),
                  moveRects(_moveRects),
                  attackRects(_attackRects),
                  deadRects(_deadRects),
                  currentRectList(Types::RectListType::DEFAULTRECT),
                  currentRectInList(0)
            {
            }
            Rect defaultRect;
            std::vector<Rect> moveRects;
            std::vector<Rect> attackRects;
            std::vector<Rect> deadRects;
            RectListType currentRectList;
            std::size_t currentRectInList;
            void changeRectList(RectListType type = RectListType::DEFAULTRECT)
            {
                currentRectList   = type;
                currentRectInList = 0;
            }
    };

} // namespace Types
