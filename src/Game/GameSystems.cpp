
#include "GameSystems.hpp"
#include "B-luga/Json.hpp"
#include "B-luga/Maths/Maths.hpp"
#include "B-luga/SystemManagers/SystemManagersDirector.hpp"
#include "DeathSystems.hpp"
#include "GameCustomTypes.hpp"
#include "ResourcesManager.hpp"
#ifdef CLIENT
    #include "B-luga-graphics/AnimRect.hpp"
    #include "NitworkClient.hpp"
#else
    #include "NitworkServer.hpp"
#endif

namespace Systems {
    const std::vector<JsonType> enemyPaths {JsonType::DEFAULT_ENEMY, JsonType::TERMINATORBOSS};

    std::string getPathFromEnemyType(enemy_type_e enemyType)
    {
        return ResourcesManager::getPathByJsonType(enemyPaths[enemyType]);
    }

    void
    initEnemy(enemy_type_e enemyType, Types::Position position, bool setId, struct ::enemy_id_s enemyId)
    {
        std::vector<nlohmann::basic_json<>> enemyData =
            Json::getInstance().getDataByJsonType(getPathFromEnemyType(enemyType), "enemy");

        for (auto &elem : enemyData) {
            Registry::getInstance().addEntity();

#ifdef CLIENT
            Types::SpriteDatas enemy = {
                Json::getInstance().getDataFromJson<std::string>(elem, "spritePath"),
                Json::getInstance().getDataFromJson<int>(elem, "width"),
                Json::getInstance().getDataFromJson<int>(elem, "height"),
                LayerType::DEFAULTLAYER,
                0};
            auto rect = Json::getInstance().getDataFromJson<Types::Rect>(elem, "rect");
            nlohmann::basic_json<> animRectData =
                Json::getInstance().getDataFromJson<nlohmann::basic_json<>>(elem, "animRect");
            Types::AnimRect animRect(rect, animRectData, Types::RectListType::MOVE, Types::Direction::LEFT);
#endif
            Types::Enemy enemyComp = (setId ? Types::Enemy {enemyId} : Types::Enemy {});
            Types::CollisionRect collisionRect =
                Json::getInstance().getDataFromJson<Types::CollisionRect>(elem, "collisionRect");
            Types::Damage damageComp   = {Json::getInstance().getDataFromJson<int>(elem, "damage")};
            struct health_s healthComp = {Json::getInstance().getDataFromJson<int>(elem, "health")};
            Types::Velocity velocity =
                Json::getInstance().getDataFromJson<Types::Velocity>(elem, "velocity");

            if (position.x == 0 && position.y == 0) {
                Types::Position tmpPos(
                    Json::getInstance().getDataFromJson<Types::Position>(elem, "position"));
                position = tmpPos;
            }
#ifdef CLIENT
            Registry::getInstance().getComponents<Types::Rect>().insertBack(rect);
            Registry::getInstance().getComponents<Types::AnimRect>().insertBack(animRect);
            Registry::getInstance().getComponents<Types::SpriteDatas>().insertBack(enemy);
#endif
            if (enemyType == TERMINATOR) {
                Types::Boss boss = {};
                Registry::getInstance().getComponents<Types::Boss>().insertBack(boss);
            }
            Registry::getInstance().getComponents<Types::Position>().insertBack(position);
            Registry::getInstance().getComponents<Types::CollisionRect>().insertBack(collisionRect);
            Registry::getInstance().getComponents<Types::Velocity>().insertBack(velocity);
            Registry::getInstance().getComponents<struct health_s>().insertBack(healthComp);
            Registry::getInstance().getComponents<Types::Damage>().insertBack(damageComp);
            Registry::getInstance().getComponents<Types::Enemy>().insertBack(enemyComp);
        }
    }

    void initPlayer(
        unsigned int constId,
        const struct position_absolute_s &pos,
        const struct health_s &life,
        bool otherPlayer)
    {
        std::string playerPath = ResourcesManager::getPathByJsonType(JsonType::DEFAULT_PLAYER);

        Registry::getInstance().addEntity();

        Types::Dead deadComp = {
            Json::getInstance().getDataByVector<std::size_t>(playerPath, {"player", "deadTime"})};
        Types::Damage damageComp = {
            Json::getInstance().getDataByVector<int>(playerPath, {"player", "damage"})};
        Types::Container container(
            Json::getInstance().getDataByVector(playerPath, {"player", "container"}));
        Types::CollisionRect collisionRect = {Types::CollisionRect(
            Json::getInstance().getDataByVector(playerPath, {"player", "collisionRect"}))};
        Types::Position position           = {pos.x, pos.y};
        struct health_s healthComp         = life;
#ifdef CLIENT
        Types::SpriteDatas playerDatas(
            Json::getInstance().getDataByVector(
                ResourcesManager::getPathByJsonType(JsonType::DEFAULT_PLAYER),
                {"player", "spritePath"}),
            Json::getInstance().getDataByVector(
                ResourcesManager::getPathByJsonType(JsonType::DEFAULT_PLAYER),
                {"player", "width"}),
            Json::getInstance().getDataByVector(
                ResourcesManager::getPathByJsonType(JsonType::DEFAULT_PLAYER),
                {"player", "height"}),
            FRONTLAYER,
            static_cast<std::size_t>(FRONT));

        Types::Rect rect = {
            Types::Rect(Json::getInstance().getDataByVector(playerPath, {"player", "rect"}))};
        nlohmann::basic_json<> animRectData =
            Json::getInstance().getDataByVector(playerPath, {"player", "animRect"});
        Types::AnimRect animRect(rect, animRectData);
#endif

        // Add components to registry
        if (otherPlayer) {
            Types::OtherPlayer otherPlayerComp(constId);
            Registry::getInstance().getComponents<Types::OtherPlayer>().insertBack(otherPlayerComp);
            Types::PlayerAllies allie;
            Registry::getInstance().getComponents<Types::PlayerAllies>().insertBack(allie);
        } else {
            Types::Player playerComp = {constId};
            Registry::getInstance().getComponents<Types::Player>().insertBack(playerComp);
        }

#ifdef CLIENT
        Registry::getInstance().getComponents<Types::Rect>().insertBack(rect);
        Registry::getInstance().getComponents<Types::AnimRect>().insertBack(animRect);
        Registry::getInstance().getComponents<Types::SpriteDatas>().insertBack(playerDatas);
#endif
        Registry::getInstance().getComponents<Types::Position>().insertBack(position);
        Registry::getInstance().getComponents<Types::CollisionRect>().insertBack(collisionRect);
        Registry::getInstance().getComponents<Types::Damage>().insertBack(damageComp);
        Registry::getInstance().getComponents<struct health_s>().insertBack(healthComp);
        Registry::getInstance().getComponents<Types::Dead>().insertBack(deadComp);
        Registry::getInstance().getComponents<Types::Container>().insertBack(container);
    }

    static void sendMissileDeath(std::size_t arrId)
    {
        Logger::fatal("start send missile death");
        auto &arrMissiles = Registry::getInstance().getComponents<Types::Missiles>();

        if (arrMissiles.exist(arrId)) {
#ifdef CLIENT
            Nitwork::NitworkClient::getInstance().addMissileDeathMsg(arrMissiles[arrId].constId);
#else
            Nitwork::NitworkServer::getInstance().addMissileDeathMsg(arrMissiles[arrId].constId);
#endif
        }
    }

    static void sendEnemyDeath(std::size_t arrId)
    {
        Logger::fatal("start send enemy death");
        auto &arrEnemies = Registry::getInstance().getComponents<Types::Enemy>();

        if (!arrEnemies.exist(arrId)) {
            return;
        }
#ifdef CLIENT
        Nitwork::NitworkClient::getInstance().addEnemyDeathMsg(arrEnemies[arrId].getConstId().id);
#else
        Nitwork::NitworkServer::getInstance().addEnemyDeathMessage(arrEnemies[arrId].getConstId().id);
#endif
        Logger::fatal("end send enemy death");
    }

#ifdef CLIENT
    static void sendLifeUpdateToServer(std::size_t id)
    {
        Registry::components<Types::Player> arrPlayer =
            Registry::getInstance().getComponents<Types::Player>();
        auto arrHealth = Registry::getInstance().getComponents<struct health_s>();

        if (arrPlayer.exist(id)) {
            Nitwork::NitworkClient::getInstance().addLifeUpdateMsg(arrPlayer[id].constId, arrHealth[id]);
        }
    }

    static void sendPlayerDeathToServer(std::size_t id)
    {
        Logger::fatal("start send player death");
        auto &arrPlayer      = Registry::getInstance().getComponents<Types::Player>();
        auto &arrOtherPlayer = Registry::getInstance().getComponents<Types::OtherPlayer>();

        if (arrPlayer.exist(id)) {
            Logger::debug("Player send death " + std::to_string(id));
            Nitwork::NitworkClient::getInstance().addPlayerDeathMsg(arrPlayer[id].constId);
        } else if (arrOtherPlayer.exist(id)) {
            Logger::debug("other player send death " + std::to_string(id));
            Nitwork::NitworkClient::getInstance().addPlayerDeathMsg(arrOtherPlayer[id].constId);
        }
        Logger::fatal("end send player death");
    }
#endif

    void GamePlugin::initPlugin()
    {
        auto &registry = Registry::getInstance();
        std::lock_guard<std::mutex> lock(registry.mutex);
        registry.addEventCallback(Events::ENTITY_DEATH, sendEnemyDeath);
        registry.addEventCallback(Events::ENTITY_DEATH, sendMissileDeath);
#ifdef CLIENT
        registry.addEventCallback(Events::ENTITY_DEATH, sendPlayerDeathToServer);

        registry.addEventCallback(Events::TAKE_DAMAGE, sendLifeUpdateToServer);
#endif
        registry.addAllie(static_cast<std::size_t>(AlliesType::PLAYERS), typeid(Types::Player));
        registry.addAllie(static_cast<std::size_t>(AlliesType::PLAYERS), typeid(Types::OtherPlayer));
        registry.addAllie(static_cast<std::size_t>(AlliesType::PLAYERS), typeid(Types::PlayerAllies));
        registry.addAllie(static_cast<std::size_t>(AlliesType::ENEMIES), typeid(Types::Enemy));
        registry.addAllie(static_cast<std::size_t>(AlliesType::ENEMIES), typeid(Types::EnemyAllies));
    }

    std::vector<std::function<void(std::size_t, std::size_t)>> GamePlugin::getSystems()
    {
        std::vector<std::function<void(std::size_t, std::size_t)>> gameSystems = {};
        std::vector<std::function<void(std::size_t, std::size_t)>> deathSystems =
            DeathSystems::getDeathSystems();
        std::vector<std::function<void(std::size_t, std::size_t)>> bulletSystems = getBulletsSystems();
        std::vector<std::function<void(std::size_t, std::size_t)>> physicSystems = getPhysicSystems();
        std::vector<std::function<void(std::size_t, std::size_t)>> waveSystems = getWaveSystems();

        gameSystems.insert(gameSystems.end(), deathSystems.begin(), deathSystems.end());
        gameSystems.insert(gameSystems.end(), bulletSystems.begin(), bulletSystems.end());
        gameSystems.insert(gameSystems.end(), physicSystems.begin(), physicSystems.end());
        gameSystems.insert(gameSystems.end(), waveSystems.begin(), waveSystems.end());
        return gameSystems;
    }
} // namespace Systems
