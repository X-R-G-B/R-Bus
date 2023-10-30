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
    enum Direction { NONE, UP, DOWN, RIGHT, LEFT };

    struct Rect {
            float x;
            float y;
            float width;
            float height;

            NLOHMANN_DEFINE_TYPE_INTRUSIVE(Rect, x, y, width, height);
    };

    NLOHMANN_JSON_SERIALIZE_ENUM(
        Direction,
        {
            {NONE,  nullptr},
            {UP,    "up"   },
            {DOWN,  "down" },
            {RIGHT, "right"},
            {LEFT,  "left" },
    });

    enum RectListType { UNDEFINED, DEFAULT_RECT, MOVE, ATTACK, DEAD, HOVER, SELECTED };

    NLOHMANN_JSON_SERIALIZE_ENUM(
        RectListType,
        {
            {UNDEFINED,    nullptr  },
            {DEFAULT_RECT, "default"},
            {MOVE,         "move"   },
            {ATTACK,       "attack" },
            {DEAD,         "dead"   },
            {HOVER,        "hover"  },
            {SELECTED,        "selected"  }
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
                Direction direction = Direction::NONE);

            /// Destructor for the AnimRect class.
            ~AnimRect() = default;

            /**
             * @brief Changes the rectangle list type.
             *
             * @param type The new type of rectangle list (default to DEFAULT_RECT).
             */
            void changeRectList(RectListType type = RectListType::DEFAULT_RECT);

            /**
             * @brief Retrieves the actual delay time for the current animation.
             *
             * @return The delay time in milliseconds.
             */
            std::size_t getActualAnimDelay();

            /**
             * @brief Changes the direction for the animation.
             *
             * @param direction The new direction for the animation (default to NONE).
             */
            void changeDirection(Direction direction = Direction::NONE);

            /**
             * @brief Checks if the current key (state and direction) is valid in the animation data.
             *
             * @return True if the key is valid, false otherwise.
             */
            bool isCurrentKeyValid();

            /**
             * @brief Retrieves the current rectangle for the animation.
             *
             * @return The current rectangle.
             */
            Rect getCurrentAnimRect();

        private:
            /**
             * @brief Initializes animation rectangle data from JSON.
             *
             * @param json JSON data containing the animation rectangles information.
             */
            void initAnimRectData(nlohmann::json &json);

            /**
             * @brief Fills the vector list with rectangle data from JSON.
             *
             * @param list JSON list containing the rectangle data.
             * @param animRect Vector to fill with rectangle data.
             */
            void fillVectorList(nlohmann::json &list, std::vector<Rect> &animRect);

            Rect _defaultRect;
            RectListType _currentRectList;
            Direction _currentDirection;
            std::size_t _currentRectInList;
            std::unordered_map<RectListType, DirectionVector> _animRects;
            std::unordered_map<RectListType, NoIncrVector> _noIncrVector;
            std::unordered_map<RectListType, RestartIndex> _animRestart;
    };
} // namespace Types
