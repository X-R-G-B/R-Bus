/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** AnimRect
*/

#pragma once

#include <unordered_map>
#include "Json.hpp"
#include "nlohmann/json.hpp"
#include "GraphicsCustomTypes.hpp"

namespace Types {
    enum Direction { NONE, UP, DOWN, RIGHT, LEFT };

    NLOHMANN_JSON_SERIALIZE_ENUM(
        Direction,
        {
            {NONE,  nullptr},
            {UP,    "up"   },
            {DOWN,  "down" },
            {RIGHT, "right"},
            {LEFT,  "left" },
    });

    enum RectListType { UNDEFINED, DEFAULT_RECT, MOVE, ATTACK, DEAD };

    NLOHMANN_JSON_SERIALIZE_ENUM(
        RectListType,
        {
            {UNDEFINED,    nullptr  },
            {DEFAULT_RECT, "default"},
            {MOVE,         "move"   },
            {ATTACK,       "attack" },
            {DEAD,         "dead"   },
    });

    class AnimRect {
        public:
            using DirectionVector =
                std::unordered_map<Direction, std::pair<std::size_t, std::vector<Rect>>>;
            using NoIncrVector = std::unordered_map<Direction, bool>;
            using RestartIndex = std::unordered_map<Direction, std::size_t>;
            /**
             * @brief Constructor for the AnimRect class.
             *
             * @param rect The default rectangle.
             * @param animRectData Data for the animation rectangles.
             * @param state The type/state of the rectangle list (default to DEFAULT_RECT).
             * @param direction The initial direction for the animation (default to NONE).
             */
            AnimRect(
                const Rect &rect,
                nlohmann::json animRectData,
                RectListType state  = RectListType::DEFAULT_RECT,
                Direction direction = Direction::NONE)
                : _defaultRect(rect),
                  _currentRectList(state),
                  _currentDirection(direction),
                  _currentRectInList(0)
            {
                for (auto it : animRectData) {
                    initAnimRectData(it);
                }
            }

            /// Destructor for the AnimRect class.
            ~AnimRect() = default;

            /**
             * @brief Changes the rectangle list type.
             *
             * @param type The new type of rectangle list (default to DEFAULT_RECT).
             */
            void changeRectList(RectListType type = RectListType::DEFAULT_RECT)
            {
                if (type != _currentRectList) {
                    _currentRectList   = type;
                    _currentRectInList = 0;
                }
            }

            const std::size_t defaultTimeAnim = 120;

            /**
             * @brief Retrieves the actual delay time for the current animation.
             *
             * @return The delay time in milliseconds.
             */
            std::size_t getActualAnimDelay()
            {
                if (!isCurrentKeyValid()) {
                    return (defaultTimeAnim);
                }
                return (_animRects[_currentRectList][_currentDirection].first);
            }

            /**
             * @brief Changes the direction for the animation.
             *
             * @param direction The new direction for the animation (default to NONE).
             */
            void changeDirection(Direction direction = Direction::NONE)
            {
                if (direction != _currentDirection) {
                    _currentDirection  = direction;
                    _currentRectInList = 0;
                }
            }

            /**
             * @brief Checks if the current key (state and direction) is valid in the animation data.
             *
             * @return True if the key is valid, false otherwise.
             */
            bool isCurrentKeyValid()
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

            /**
             * @brief Retrieves the current rectangle for the animation.
             *
             * @return The current rectangle.
             */
            Rect getCurrentAnimRect()
            {
                if (!isCurrentKeyValid()) {
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

            AnimRect &operator=(const AnimRect &other)
            {
                _defaultRect = other._defaultRect;
                _currentRectList = other._currentRectList;
                _currentDirection = other._currentDirection;
                _currentRectInList = other._currentRectInList;
                _animRects   = other._animRects;
                _noIncrVector = other._noIncrVector;
                _animRestart  = other._animRestart;
                return (*this);
            }

        private:
            /**
             * @brief Initializes animation rectangle data from JSON.
             *
             * @param json JSON data containing the animation rectangles information.
             */
            void initAnimRectData(nlohmann::json &json)
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

            /**
             * @brief Fills the vector list with rectangle data from JSON.
             *
             * @param list JSON list containing the rectangle data.
             * @param animRect Vector to fill with rectangle data.
             */
            void fillVectorList(nlohmann::json &list, std::vector<Rect> &animRect)
            {
                for (auto &it : list) {
                    animRect.push_back(Types::Rect(it));
                }
            }

            Rect _defaultRect;
            RectListType _currentRectList;
            Direction _currentDirection;
            std::size_t _currentRectInList;
            std::unordered_map<RectListType, DirectionVector> _animRects;
            std::unordered_map<RectListType, NoIncrVector> _noIncrVector;
            std::unordered_map<RectListType, RestartIndex> _animRestart;
    };
} // namespace Types
