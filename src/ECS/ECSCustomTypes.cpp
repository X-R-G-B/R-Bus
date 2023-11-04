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
unsigned int Types::Missiles::_missileNb = 0;
std::mutex Types::Missiles::_mutex;

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

    void Physics::initBounce(nlohmann::json & /*unused*/, const Types::Position &originPos)
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
            Raylib::Text endWaveText = Raylib::Text(text, {20, 10}, fontSize, Raylib::WHITE, textKeyword);
            Registry::getInstance().getComponents<Raylib::Text>().insertBack(endWaveText);
        }
#endif
        _waitingForNextWave = value;
    }

} // namespace Types
