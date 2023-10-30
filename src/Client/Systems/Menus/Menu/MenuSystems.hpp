/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** MenuSystems
*/

#pragma once

#include <functional>
#include <vector>

namespace Systems {
    namespace Menu {

        std::vector<std::function<void(std::size_t /*unused*/, std::size_t /*unused*/)>> getMenuSystems();

        void manageInputBox(std::size_t /*unused*/, std::size_t /*unused*/);

        void checkTextInput(std::size_t /*unused*/, std::size_t /*unused*/);

        void checkInputDeletion(std::size_t /*unused*/, std::size_t /*unused*/);

        void initMenu(std::size_t managerId, std::size_t systemId);

        void pressButton(std::size_t /*unused*/, std::size_t /*unused*/);

        void hoverInputBox(std::size_t /*unused*/, std::size_t /*unused*/);

    } // namespace Menu
} // namespace Systems
