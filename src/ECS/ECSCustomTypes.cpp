#include "ECSCustomTypes.hpp"

unsigned int Types::Enemy::_enemyNb = 0;
std::mutex Types::Enemy::_mutex;

namespace Types {

    Physics::Physics(
        const Types::Position &originPos,
        unsigned long int timestampDiff,
        unsigned long int timestamp,
        const Types::Velocity &originvVelocity)
        : _originPos(originPos),
          _timestampDiff(timestampDiff),
          _timestamp(timestamp),
          _originvVelocity(originvVelocity)
    {
    }

    unsigned long int Physics::getTimestampDiff() const
    {
        return _timestampDiff;
    }

    void Physics::addPhysic(physicsType_e type, unsigned long int timestamp)
    {
        if (_physicsMap.find(type) != _physicsMap.end()) {
            Logger::error("Physics already added");
            return;
        }
        _physicsMap[type] = Registry::getInstance().getClock().create(true);
        Registry::getInstance().getClock().restart(_physicsMap[type], static_cast<std::time_t>(timestamp));
    }

    void Physics::addPhysic(std::string type, unsigned long int timestamp)
    {
        auto it = physicsTypeMap.find(type);
        if (it == physicsTypeMap.end()) {
            Logger::error("Physics not found");
            return;
        }
        addPhysic(it->second, timestamp);
    }

    std::size_t Physics::getClock(physicsType_e type) const
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

        for (const auto &pair : _physicsMap) {
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
        return it->second;
    }

    const Types::Position &Physics::getOriginPos() const
    {
        return _originPos;
    }

    const Types::Velocity &Physics::getOriginvVelocity() const
    {
        return _originvVelocity;
    }

    unsigned long int Physics::getTimestamp() const
    {
        return _timestamp;
    }

} // namespace Types
