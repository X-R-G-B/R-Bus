/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** AnimRect
*/

#include "AnimRect.hpp"
#include <iostream>

namespace Types {
    AnimRect::AnimRect(Rect rect, nlohmann::json animRectData, RectListType state, Direction direction)
        : _defaultRect(rect), _currentRectList(state), _currentDirection(direction), _currentRectInList(0)
    {
        for (auto &it : animRectData) {
            std::cout << it << "    :::: DATAAAAAAAAAAA" << std::endl;
            Direction direction = it["direction"].get<Direction>();
            RectListType type = it["type"].get<RectListType>();
            std::vector<Rect> animRect = it["list"].get<std::vector<Rect>>();

            std::cout << "Actual direction : " << direction << std::endl;
            std::cout << "Actual type : " << type << std::endl;

            _animRects[type][direction] = animRect;
        }
    }

    void AnimRect::changeRectList(RectListType type)
    {
        _currentRectList = type;
        _currentRectInList = 0;
    }

    void AnimRect::changeDirection(Direction direction)
    {
        _currentDirection = direction;
        _currentRectInList = 0;
    }

    Rect &AnimRect::getCurrentAnimRect()
    {
        DirectionVector animRects;
        if (auto search = _animRects.find(_currentRectList); search == _animRects.end()) {
            std::cout << "BAD TYPE" << std::endl;
            std::cout << "ACTUAL TYPE : " << _currentRectList << std::endl;
            return (_defaultRect);
        }
        animRects = _animRects[_currentRectList];

        std::vector<Rect> animRect;
        if (auto search = animRects.find(_currentDirection); search == animRects.end()) {
            std::cout << "BAD DIRECTION" << std::endl;
            std::cout << "ACTUAL DIRECTION : " << _currentDirection << std::endl;
            return (_defaultRect);
        }
        animRect = animRects[_currentDirection];
        _currentRectInList += 1;
        if (animRect.size() <= _currentRectInList) {
            _currentRectInList = 0;
        }
        std::cout << "pos X : " << animRect[_currentRectInList].x << std::endl;
        std::cout << "pos Y : " << animRect[_currentRectInList].y << std::endl;
        std::cout << "width : " << animRect[_currentRectInList].width << std::endl;
        std::cout << "height : " << animRect[_currentRectInList].height << std::endl;
        std::cout << "rect in list : " << _currentRectInList << std::endl;
        return (animRect[_currentRectInList]);
    }
}