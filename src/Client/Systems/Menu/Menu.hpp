/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Menu
*/

#pragma once

#include "B-luga/Json.hpp"
#include "B-luga/Registry.hpp"
#include "B-luga/SystemManagers/SystemManagersDirector.hpp"
#include "ButtonCallbacks.hpp"

namespace Menu {

    // Enum that defines the type of the object you're creating
    enum ObjectType { UNDEFINED, BUTTON, TEXT, INPUT_BOX };

    // Enum serialization to precise in a json what enum you defined with a string
    NLOHMANN_JSON_SERIALIZE_ENUM(
        ObjectType,
        {
            {UNDEFINED, nullptr },
            {BUTTON,    "button"},
            {TEXT,      "text"  },
            {INPUT_BOX, "input" },
    });

    bool checkClick(std::size_t &idEntity);

    bool checkIsInsideRect(const std::size_t &idEntity);

    /**
     * @class MenuBuilder
     * @brief Singleton class for building and initializing menu entities.
     *
     * This class provides functions for initializing menu entities from a JSON element. It's designed as a
     * Singleton, meaning there should only be one instance of this class in the application.
     */
    class MenuBuilder {
        public:
            /**
             * @brief Returns the single instance of the MenuBuilder class.
             *
             * This function ensures that only one instance of the MenuBuilder class exists.
             * If the instance doesn't already exist, it's created.
             *
             * @return Reference to the singleton instance of the MenuBuilder.
             */
            static MenuBuilder &getInstance();

            /**
             * @brief Initializes a menu entity from a JSON element.
             *
             * This function sets up a menu entity using the provided JSON element and
             * an optional callback function. If no callback is provided, a default callback is used.
             *
             * @param elem The JSON element from which the menu entity will be initialized.
             * @param callback Optional callback function for the menu entity. Defaults to
             * Menu::Callback::defaultCallBack.
             */
            void initMenuEntity(
                nlohmann::json &elem,
                std::function<void()> callback = Menu::Callback::defaultCallBack);

        private:
            MenuBuilder()  = default;
            ~MenuBuilder() = default;

            /**
             * @brief Singleton instance of the MenuBuilder class.
             *
             * This static member holds the only instance of the MenuBuilder class.
             */
            // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
            static MenuBuilder _instance;
            // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
    };
} // namespace Menu
