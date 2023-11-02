/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Wave Systems implementation
*/

#include "WaveSystem.hpp"
#include "ECSCustomTypes.hpp"
#include "Json.hpp"
#include "Registry.hpp"
#include "SystemManagersDirector.hpp"
#include "Systems.hpp"

#include <iostream>

std::size_t Wave::_clockId = 0;
std::mutex Wave::_mutex;

Wave::Wave() : _waveIndex(0), _msBeforeNextWave(0), _isGameEnded(false), _isTimeBetweenWaves(false)
{
    try {
        _wavesId = Json::getInstance().getObjectsIdInArray<std::size_t>(JsonType::WAVE, "waves");
    } catch (const std::exception &e) {
        Logger::fatal("WaveInit: " + std::string(e.what()));
    }
    if (_wavesId.empty()) {
        Logger::error("WaveInit: No waves found");
        _isGameEnded = true;
        return;
    }
    _clockId = Registry::getInstance().getClock().create();
    startNextWave();
}

// Called when the last enemy of the wave is killed and the time between waves is over
void Wave::startNextWave()
{
    auto &director                 = Systems::SystemManagersDirector::getInstance();
    Nitwork::NitworkServer &server = Nitwork::NitworkServer::getInstance();
    std::lock_guard<std::mutex> lock(_mutex);

    if (_waveIndex > _wavesId.size()) {
        _isGameEnded = true;
        return;
    }
    try {
        std::size_t id = _wavesId.at(_waveIndex);
        nlohmann::json waveData =
            Json::getInstance().getJsonObjectById<std::size_t>(JsonType::WAVE, id, "waves");
        _msBeforeNextWave = Json::getInstance().getDataFromJson<std::size_t>(waveData, "msBeforeNextWave");
        Types::WaveInfos::getInstance().setWaveId(static_cast<unsigned int>(id));
        server.addStarWaveMessage(static_cast<n_id_t>(_wavesId.at(_waveIndex)));
        _waveIndex++;
    } catch (const std::exception &e) {
        Logger::fatal("WaveInit: " + std::string(e.what()));
        _isGameEnded = true;
        return;
    }
    director.getSystemManager(0).addSystem(Systems::initWave);
}

bool Wave::isWaveEnded() const
{
    if (Types::WaveInfos::getInstance().getWaitingForNextWave() == false
        || Types::WaveInfos::getInstance().isEnemyRemaining() == true || Types::Enemy::isEnemyAlive()) {
        return false;
    }
    return true;
}

bool Wave::isGameEnded() const
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _isGameEnded;
}

std::size_t Wave::getMsBeforeNextWave() const
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _msBeforeNextWave;
}

bool Wave::isTimeBetweenWaves() const
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _isTimeBetweenWaves;
}

void Wave::setTimeBetweenWaves(bool isTimeBetweenWaves)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _isTimeBetweenWaves = isTimeBetweenWaves;
}

namespace Systems {

    void waveHandler(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        static Wave waveHandler;
        Registry &registry = Registry::getInstance();
        std::lock_guard<std::mutex> lock(registry.mutex);

        if (waveHandler.isGameEnded()) {
            std::cout << "------------------ GAME ENDED ------------------" << std::endl;
        }

        if (waveHandler.isWaveEnded() && waveHandler.isGameEnded() == false) {
            if (waveHandler.isTimeBetweenWaves() == false) {
                waveHandler.setTimeBetweenWaves(true);
                registry.getClock().restart(Wave::_clockId);
                return;
            }
            if (registry.getClock().elapsedMillisecondsSince(Wave::_clockId)
                >= waveHandler.getMsBeforeNextWave()) {
                waveHandler.setTimeBetweenWaves(false);
                waveHandler.startNextWave();
                Types::WaveInfos::getInstance().setWaitingForNextWave(false);
            }
        }
    }
} // namespace Systems
