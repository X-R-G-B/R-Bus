/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** CreateLobbySystems
*/

#pragma once

#include <functional>
#include <vector>

namespace Systems {
    namespace CreateLobby {

        std::vector<std::function<void(std::size_t /*unused*/, std::size_t /*unused*/)>> getCreateLobbySystems();

        void manageInputBox(std::size_t /*unused*/, std::size_t /*unused*/);

        void checkTextInput(std::size_t /*unused*/, std::size_t /*unused*/);

        void checkInputDeletion(std::size_t /*unused*/, std::size_t /*unused*/);

        void initCreateLobby(std::size_t managerId, std::size_t systemId);

        void pressButton(std::size_t /*unused*/, std::size_t /*unused*/);

        void hoverInputBox(std::size_t /*unused*/, std::size_t /*unused*/);

    } // namespace CreateLobby
} // namespace Systems
