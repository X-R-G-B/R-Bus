/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** AnimRect
*/

#include "AnimRect.hpp"
#include <iostream>
#include "Json.hpp"

namespace Types {
    constexpr std::size_t defaultTimeAnim = 120;

    AnimRect::AnimRect(
        const Rect &rect,
        nlohmann::json animRectData,
        RectListType state,
        Direction direction)
        : _defaultRect(rect),
          _currentRectList(state),
          _currentDirection(direction),
          _currentRectInList(0)
    {
        for (auto &it : animRectData) {
            initAnimRectData(it);
        }
    }

    void AnimRect::fillVectorList(nlohmann::json &list, std::vector<Rect> &animRect)
    {
        for (auto &it : list) {
            animRect.push_back(Types::Rect(it));
        }
    }

    void AnimRect::initAnimRectData(nlohmann::json &json)
    {
        Direction direction = json["direction"].get<Direction>();
        RectListType type   = json["type"].get<RectListType>();
        std::size_t time;
        std::size_t restartIndex = 0;
        bool noIncr              = false;
        std::vector<Rect> animRect({});

        !Json::getInstance().isDataExist(json, "list") ? animRect.push_back(_defaultRect)
                                                       : fillVectorList(json["list"], animRect);
        time         = !Json::getInstance().isDataExist(json, "time") ? defaultTimeAnim
                                                                      : json["time"].get<std::size_t>();
        restartIndex = !Json::getInstance().isDataExist(json, "restartIndex")
            ? 0
            : json["restartIndex"].get<std::size_t>();
        noIncr = !Json::getInstance().isDataExist(json, "noIncr") ? false : json["noIncr"].get<bool>();
        _animRects[type][direction]    = std::make_pair(time, animRect);
        _noIncrVector[type][direction] = noIncr;
        _animRestart[type][direction]  = restartIndex;
    }

    void AnimRect::changeRectList(RectListType type)
    {
        if (type != _currentRectList) {
            _currentRectList   = type;
            _currentRectInList = 0;
        }
    }

    void AnimRect::changeDirection(Direction direction)
    {
        if (direction != _currentDirection) {
            _currentDirection  = direction;
            _currentRectInList = 0;
        }
    }

    bool AnimRect::checkValidKey()
    {
        DirectionVector animRects;

        if (auto search = _animRects.find(_currentRectList); search == _animRects.end()) {
            return (false);
        }
        animRects = _animRects[_currentRectList];
        if (auto search = animRects.find(_currentDirection); search == animRects.end()) {
            return (false);
        }
        return (true);
    }

    std::size_t AnimRect::getActualAnimDelay()
    {
        if (!checkValidKey()) {
            return (defaultTimeAnim);
        }
        return (_animRects[_currentRectList][_currentDirection].first);
    }

    Rect AnimRect::getCurrentAnimRect()
    {
        if (!checkValidKey()) {
            return (_defaultRect);
        }
        std::vector<Rect> &animRect(_animRects[_currentRectList][_currentDirection].second);

        _currentRectInList += 1;
        if (animRect.size() <= _currentRectInList) {
            _currentRectInList = _noIncrVector[_currentRectList][_currentDirection]
                ? animRect.size() - 1
                : _animRestart[_currentRectList][_currentDirection];
        }
        Types::Rect &rect(animRect[_currentRectInList]);

        return (rect);
    }
} // namespace Types
