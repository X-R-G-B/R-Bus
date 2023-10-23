#include "ECSCustomTypes.hpp"

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
            return std::nullopt;
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
            Logger::error("Physics not found");
            throw std::runtime_error("Physics not found");
        }
        return it->second.value();
    }

    const Types::Position &Physics::getOriginPos() const
    {
        return _originPos;
    }

} // namespace Types
