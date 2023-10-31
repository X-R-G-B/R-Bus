/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Wave systems serverside
*/

#pragma once

#include <cstddef>
#include <vector>
#include <mutex>
#include "NitworkServer.hpp"

class Wave {
    public:

        Wave();
        void startNextWave(bool isFirstWave = false);
        std::size_t getEnemiesRemaining() const;
        std::size_t getMsBeforeNextWave() const;
        bool isGameEnded() const;
        static std::size_t _clockId;

    private:

        void decreaseEnemiesRemaining();
        std::vector<std::size_t> _wavesId;
        std::size_t _enemiesRemaining;
        std::size_t _msBeforeNextWave;
        int _waveIndex;
        bool _isGameEnded;
        static std::mutex _mutex;
};

namespace Systems {
    void waveHandler(std::size_t /*unused*/, std::size_t /*unused*/);
} // namespace Systems
