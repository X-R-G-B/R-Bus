/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Wave Systems implementation
*/

#include "Json.hpp"
#include "Registry.hpp"
#include "Systems.hpp"
#include "WaveSystem.hpp"
#include "SystemManagersDirector.hpp"

std::size_t Wave::_clockId = 0;
std::mutex Wave::_mutex;

Wave::Wave() : _waveIndex(0), _msBeforeNextWave(0), _isGameEnded(false), _enemiesRemaining(0)
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
    startNextWave(true);
}

// Called when the last enemy of the wave is killed and the time between waves is over
void Wave::startNextWave(bool isFirstWave)
{
    auto &director                 = Systems::SystemManagersDirector::getInstance();
    Nitwork::NitworkServer &server = Nitwork::NitworkServer::getInstance();
    std::lock_guard<std::mutex> lock(_mutex);

    if (isFirstWave == false) {
        _waveIndex++;
    }
    if (_waveIndex >= _wavesId.size()) {
        _isGameEnded = true;
        return;
    }
    try {
        std::size_t id = _wavesId.at(_waveIndex);
        nlohmann::json waveData = Json::getInstance().getJsonObjectById<std::size_t>(JsonType::WAVE, id, "waves");
        _msBeforeNextWave =
            Json::getInstance().getDataFromJson<std::size_t>(waveData, "msBeforeNextWave");
        _enemiesRemaining = Json::getInstance().getDataFromJson<std::size_t>(waveData, "nbrEnemies");
        Types::WaveInfos::getInstance().setWaveId(id);
        server.addStarWaveMessage(static_cast<n_id_t>(_wavesId.at(_waveIndex)));
    } catch (const std::exception &e) {
        Logger::fatal("WaveInit: " + std::string(e.what()));
        _isGameEnded = true;
        return;
    }
    director.getSystemManager(0).addSystem(Systems::initWave);
}

void Wave::decreaseEnemiesRemaining()
{
    std::lock_guard<std::mutex> lock(_mutex);
    if (_enemiesRemaining > 0) {
        _enemiesRemaining--;
    }
    if (_enemiesRemaining == 0) {
        Registry::getInstance().getClock().restart(Wave::_clockId);
    }
}

std::size_t Wave::getEnemiesRemaining() const
{
    std::lock_guard<std::mutex> lock(_mutex);
    return _enemiesRemaining;
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

namespace Systems {

    void waveHandler(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        static Wave waveHandler;

        if (waveHandler.getEnemiesRemaining() == 0 && waveHandler.isGameEnded() == false
            && Registry::getInstance().getClock().elapsedMillisecondsSince(Wave::_clockId)
                >= waveHandler.getMsBeforeNextWave()) {
            waveHandler.startNextWave();
        }
    }

} // namespace Systems
