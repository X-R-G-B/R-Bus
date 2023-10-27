/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Menu
*/

#pragma once

#include "ButtonCallbacks.hpp"
#include "Json.hpp"
#include "Registry.hpp"
#include "SystemManagersDirector.hpp"

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

    bool checkClick(std::size_t &idEntity);

    bool checkIsInsideRect(const std::size_t &idEntity);

    class MenuFactory {
        public:
            static MenuFactory &getInstance();

            void initMenuEntity(
                nlohmann::json &elem,
                ObjectType type,
                std::function<void()> callback = Menu::Callback::defaultCallBack);

        private:
            MenuFactory()  = default;
            ~MenuFactory() = default;

            // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
            static MenuFactory _instance;
            // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
    };
} // namespace Menu