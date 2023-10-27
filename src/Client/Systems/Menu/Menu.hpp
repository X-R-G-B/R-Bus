/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Menu
*/

#pragma once

#include "Json.hpp"
#include "Registry.hpp"
#include "SystemManagersDirector.hpp"

namespace Systems {
    namespace Menu {
        enum ObjectType { UNDEFINED, BUTTON, TEXT, INPUT_BOX };

        NLOHMANN_JSON_SERIALIZE_ENUM(
            ObjectType,
            {
                {UNDEFINED, nullptr },
                {BUTTON,    "button"},
                {TEXT,      "text"  },
                {INPUT_BOX, "input" },
        });

        enum Material { NONE, RECTANGLE, SPRITE };

        NLOHMANN_JSON_SERIALIZE_ENUM(
            Material,
            {
                {NONE,      nullptr    },
                {RECTANGLE, "rectangle"},
                {SPRITE,    "sprite"   },
        });

        std::vector<std::function<void(std::size_t, std::size_t)>> getMenuSystems();

        void manageInputBox(std::size_t, std::size_t);

        void checkInputDeletion(std::size_t, std::size_t);

    } // namespace Menu
} // namespace Systems

namespace Menu {

    bool checkClick(std::size_t &idEntity);

    bool checkIsInsideRect(const std::size_t &id);

} // namespace Menu