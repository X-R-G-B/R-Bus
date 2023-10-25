#include "ECSCustomTypes.hpp"

unsigned int Types::Enemy::_enemyNb = 0;
std::mutex Types::Enemy::_mutex;

namespace Types {

    Physics::Physics()
    {
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

    void Physics::removePhysic(physicsType_e type)
    {
        if (_physicsMap.find(type) == _physicsMap.end()) {
            Logger::error("Physics not found");
            return;
        }
        _physicsMap.erase(type);
    }

    void Physics::removePhysic(std::string type)
    {
        auto it = physicsTypeMap.find(type);
        if (it == physicsTypeMap.end()) {
            Logger::error("Physics not found");
            return;
        }
        removePhysic(it->second);
    }

    void Physics::removePhysics()
    {
        _physicsMap.clear();
    }

} // namespace Types
