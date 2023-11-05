/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** CreateServerSystems
*/

#pragma once

#include <functional>
#include <vector>

namespace Systems::CreateServerSystems {
    std::vector<std::function<void(std::size_t /*unused*/, std::size_t /*unused*/)>>
        getCreateServerSystems();
}