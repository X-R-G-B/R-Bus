/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Wave systems serverside
*/

#pragma once

#include <cstddef>
#include <vector>

class Wave {
    public:
        Wave()
        {
        }

        void StartNextWave()
        {
            n_id_t waveid                  = 0;
            Nitwork::NitworkServer &server = Nitwork::NitworkServer::getInstance();
            server.addStarWaveMessage(waveid);
        }
        ////director.getSystemManager(0).addSystem(Systems::initWave); dont forget to call this after first
        ///wave

    private:
        std::size_t getTimeBeforeNextWave() const;
        void init();

        std::vector<std::size_t> _wavesId;
        std::size_t _currentWave;
        static std::size_t _clockId;
        std::size_t _msBeforeNextWave;
};

namespace Systems {
    void waveHandler(std::size_t /*unused*/, std::size_t /*unused*/);
} // namespace Systems
