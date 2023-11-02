/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Custom Types
*/

#include "ECSCustomTypes.hpp"
#ifdef CLIENT
    #include "Raylib.hpp"
#endif

unsigned int Types::Enemy::_enemyNb = 0;
std::mutex Types::Enemy::_mutex;

namespace Types {

    Physics::Physics(const Types::Position &originPos) : _originPos(originPos)
    {
    }

    void Physics::addPhysic(physicsType_e type)
    {
        if (_physicsMap.find(type) != _physicsMap.end()) {
            Logger::error("Physics already added");
            return;
        }
        if (type == ZIGZAG) {
            _physicsMap[type] = Registry::getInstance().getClock().create(true);
        } else {
            _physicsMap[type] = std::nullopt;
        }
    }

    void Physics::addPhysic(std::string type)
    {
        auto it = physicsTypeMap.find(type);
        if (it == physicsTypeMap.end()) {
            Logger::error("Physics not found");
            return;
        }
        addPhysic(it->second);
    }

    std::optional<std::size_t> Physics::getClock(physicsType_e type) const
    {
        if (_physicsMap.find(type) == _physicsMap.end()) {
            Logger::error("Physics not found");
            throw std::runtime_error("Get clock: Physics of type " + std::to_string(type) + " not found");
        }
        return _physicsMap.at(type);
    }

    std::vector<physicsType_e> Physics::getPhysics() const
    {
        std::vector<physicsType_e> physics;

        for (auto &pair : _physicsMap) {
            physics.push_back(pair.first);
        }
        return physics;
    }

    bool Physics::hasPhysics(physicsType_e type) const
    {
        return _physicsMap.find(type) != _physicsMap.end();
    }

    bool Physics::hasPhysics() const
    {
        return !_physicsMap.empty();
    }

    void Physics::removePhysics(physicsType_e type)
    {
        if (_physicsMap.find(type) == _physicsMap.end()) {
            Logger::error("Physics not found");
            return;
        }
        _physicsMap.erase(type);
    }

    std::size_t Physics::getClockId(physicsType_e type) const
    {
        auto it = _physicsMap.find(type);
        if (it == _physicsMap.end()) {
            Logger::error("Get clock id: Physics not found");
            throw std::runtime_error(
                "Physics of type " + std::to_string(type) + " not found in getClockId");
        }
        return it->second.value();
    }

    const Types::Position &Physics::getOriginPos() const
    {
        return _originPos;
    }

    void Types::WaveInfos::prepareNextWave()
    {
        setFirstEnemyCreated(false);
        _remainingEnemies.clear();
        setWaitingForNextWave(true);
    }

    void Types::WaveInfos::setWaitingForNextWave(bool value)
    {
#ifdef CLIENT
        Registry &registry            = Registry::getInstance();
        const std::string textKeyword = "WaveText";
        std::vector<std::size_t> ids  = registry.getEntitiesByComponents({typeid(Raylib::Text)});
        Registry::components<Raylib::Text> arrCol = registry.getComponents<Raylib::Text>();
        static constexpr float fontSize = 4.0F;

        if (value == false) {
            for (auto &id : ids) {
                if (arrCol[id].getKeyword() == textKeyword) {
                    registry.removeEntity(id);
                    break;
                }
            }
        } else {
            unsigned int previousWave = getWaveId();
            const std::string text    = "Wave " + std::to_string(previousWave) + " survived";

            for (auto &id : ids) {
                if (arrCol[id].getKeyword() == textKeyword) {
                    arrCol[id].setText(text);
                    break;
                }
            }
            Registry::getInstance().addEntity();
            Raylib::Text test = Raylib::Text(text, {20, 10}, fontSize, Raylib::WHITE, textKeyword);
            Registry::getInstance().getComponents<Raylib::Text>().insertBack(test);
        }
#endif
        _waitingForNextWave = value;
    }

} // namespace Types
