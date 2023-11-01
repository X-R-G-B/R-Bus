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
        bool isWaveEnded() const;
        std::size_t getMsBeforeNextWave() const;
        bool isGameEnded() const;
        bool isTimeBetweenWaves() const;
        void setTimeBetweenWaves(bool isTimeBetweenWaves);
        static std::size_t _clockId;

    private:

        std::vector<std::size_t> _wavesId;
        std::size_t _msBeforeNextWave;
        int _waveIndex;
        bool _isGameEnded;
        bool _isTimeBetweenWaves;
        static std::mutex _mutex;
};

namespace Systems {
    void waveHandler(std::size_t /*unused*/, std::size_t /*unused*/);
} // namespace Systems
