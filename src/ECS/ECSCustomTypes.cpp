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

    void Physics::initBounce(nlohmann::json &jsonObject, const Types::Position &originPos)
    {
        Bouncing bounce(originPos);
        _physicsMap[BOUNCING] = bounce;
    }

    void Physics::initZigzag(nlohmann::json &jsonObject, const Types::Position &originPos)
    {
        Json &json = Json::getInstance();
        Zigzag zigzag(originPos);
        if (json.isDataExist(jsonObject, "amplitude")) {
            zigzag.amplitude = json.getDataFromJson<float>(jsonObject, "amplitude");
        }
        if (json.isDataExist(jsonObject, "period")) {
            zigzag.period = json.getDataFromJson<float>(jsonObject, "period");
        }
        if (json.isDataExist(jsonObject, "maxScreenY")) {
            zigzag.maxScreenY = json.getDataFromJson<float>(jsonObject, "maxScreenY");
        }
        if (json.isDataExist(jsonObject, "minScreenY")) {
            zigzag.minScreenY = json.getDataFromJson<float>(jsonObject, "minScreenY");
        }
        _physicsMap[ZIGZAG] = zigzag;
    }

    void Physics::addPhysic(nlohmann::json &jsonObject, const Types::Position &originPos)
    {
        Json &json     = Json::getInstance();
        std::string id = json.getDataFromJson<std::string>(jsonObject, "id");
        if (id == "zigzag") {
            initZigzag(jsonObject, originPos);
        }
        if (id == "bouncing") {
            initBounce(jsonObject, originPos);
        }
    }

} // namespace Types
