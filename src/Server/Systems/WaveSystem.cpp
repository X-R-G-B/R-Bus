/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Wave Systems implementation
*/

#include "ServerNetwork.hpp"
#include "ECSCustomTypes.hpp"
#include "NitworkServer.hpp"
#include "Registry.hpp"
#include "WaveSystem.hpp"

namespace Systems {

    void waveHandler(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        static Wave waveHandler;
        waveHandler.StartNextWave();
    }

} // namespace Systems
