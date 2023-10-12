/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Systems implementation
*/

#include "Systems.hpp"
#include <fstream>
#include <nlohmann/json.hpp>
#include <sstream>
#include "CustomTypes.hpp"
#include "Registry.hpp"
#include "SystemManagersDirector.hpp"
#include "Json.hpp"
#ifdef CLIENT
    #include "NitworkClient.hpp"
    #include "Raylib.hpp"
#else
    #include "NitworkServer.hpp"
#endif

namespace Systems {

    void windowCollision(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        Registry &registry                                = Registry::getInstance();
        Registry::components<Types::Position> arrPosition = registry.getComponents<Types::Position>();
        Registry::components<Types::CollisionRect> arrCollisionRect =
            registry.getComponents<Types::CollisionRect>();
        std::vector<std::size_t> ids = registry.getEntitiesByComponents(
            {typeid(Types::Player), typeid(Types::Position), typeid(Types::CollisionRect)});

        const float maxPercent = 100.0F;
        for (std::size_t id : ids) {
            if (arrPosition[id].x < 0) {
                arrPosition[id].x = 0;
            }
            if (arrPosition[id].y < 0) {
                arrPosition[id].y = 0;
            }
            if (arrPosition[id].x + arrCollisionRect[id].width > maxPercent) {
                arrPosition[id].x = maxPercent - arrCollisionRect[id].width;
            }
            if (arrPosition[id].y + arrCollisionRect[id].height > maxPercent) {
                arrPosition[id].y = maxPercent - arrCollisionRect[id].height;
            }
        }
    }

    static bool checkAllies(std::size_t fstId, std::size_t scdId)
    {
        Registry &registry                          = Registry::getInstance();
        Registry::components<Types::Player> players = registry.getComponents<Types::Player>();
        Registry::components<Types::Enemy> enemys   = registry.getComponents<Types::Enemy>();
        Registry::components<Types::PlayerAllies> playerAllies =
            registry.getComponents<Types::PlayerAllies>();
        Registry::components<Types::EnemyAllies> enemyAllies = registry.getComponents<Types::EnemyAllies>();

        if ((playerAllies.exist(fstId) && players.exist(scdId))
            || (playerAllies.exist(scdId) && players.exist(fstId))) {
            return true;
        }
        if ((enemyAllies.exist(fstId) && enemys.exist(scdId))
            || (enemyAllies.exist(scdId) && enemys.exist(fstId))) {
            return true;
        }
        return false;
    }

#ifdef CLIENT
    static void sendLifeUpdateToServer(std::size_t id, Registry::components<struct health_s> &arrHealth)
    {
        Registry::components<Types::Player> arrPlayer =
            Registry::getInstance().getComponents<Types::Player>();

        if (arrPlayer.exist(id)) {
            Nitwork::NitworkClient::getInstance().addLifeUpdateMsg(arrPlayer[id].constId, arrHealth[id]);
        }
    }
#endif

    static void giveDamages(std::size_t firstEntity, std::size_t secondEntity)
    {
        Registry::components<Types::Damage> arrDamage =
            Registry::getInstance().getComponents<Types::Damage>();
        Registry::components<struct health_s> arrHealth =
            Registry::getInstance().getComponents<struct health_s>();

        if (checkAllies(firstEntity, secondEntity)) {
            return;
        }

        if (arrDamage.exist(firstEntity) && arrDamage[firstEntity].damage > 0) {
            if (arrHealth.exist(secondEntity)) {
                arrHealth[secondEntity].hp -= arrDamage[firstEntity].damage;
#ifdef CLIENT
                sendLifeUpdateToServer(secondEntity, arrHealth);
#endif
            }
        }
    }

    static void checkCollisionEntity(
        std::vector<size_t>::iterator itIds,
        std::vector<std::size_t> &ids,
        Registry::components<Types::Position> arrPosition,
        Registry::components<Types::CollisionRect> arrCollisionRect)
    {
        std::size_t id                  = *itIds;
        Types::Position entityPos       = arrPosition[id];
        Types::CollisionRect entityColl = arrCollisionRect[id];

        itIds++;
        while (itIds != ids.end()) {
            if (arrCollisionRect.exist(*itIds)) {
                Types::CollisionRect sndEntityRect = arrCollisionRect[*itIds];
                Types::Position sndEntityPos       = arrPosition[*itIds];
                if (entityPos.x < sndEntityPos.x + sndEntityRect.width
                    && entityPos.x + entityColl.width > sndEntityPos.x
                    && entityPos.y < sndEntityPos.y + sndEntityRect.height
                    && entityPos.y + entityColl.height > sndEntityPos.y) {
                    giveDamages(id, *itIds);
                    giveDamages(*itIds, id);
                }
            }
            itIds++;
        }
    }

    void entitiesCollision(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        Registry &registry                                = Registry::getInstance();
        Registry::components<Types::Position> arrPosition = registry.getComponents<Types::Position>();
        Registry::components<Types::CollisionRect> arrCollisionRect =
            registry.getComponents<Types::CollisionRect>();
        std::vector<std::size_t> ids =
            registry.getEntitiesByComponents({typeid(Types::CollisionRect), typeid(Types::Position)});

        for (auto itIds = ids.begin(); itIds != ids.end(); itIds++) {
            checkCollisionEntity(itIds, ids, arrPosition, arrCollisionRect);
        }
    }

    void moveEntities(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        Registry::components<Types::Position> arrPosition =
            Registry::getInstance().getComponents<Types::Position>();
        Registry::components<Types::Velocity> arrVelocity =
            Registry::getInstance().getComponents<Types::Velocity>();

        std::vector<std::size_t> ids = arrPosition.getExistingsId();

        for (auto &id : ids) {
            if (arrVelocity.exist(id)) {
                arrPosition[id].x += arrVelocity[id].speedX;
                arrPosition[id].y += arrVelocity[id].speedY;
            }
        }
    }

    static void initEnnemy(JsonType ennemyType)
    {

        std::vector<nlohmann::basic_json<>> ennemyData = Json::getInstance().getDataByJsonType("ennemy", ennemyType);

        for (auto &elem : ennemyData) {
            std::size_t id        = Registry::getInstance().addEntity();
#ifdef CLIENT
            Raylib::Sprite ennemy = {Json::getInstance().getDataFromJson(elem, "spritePath"), Json::getInstance().getDataFromJson(elem, "width"), Json::getInstance().getDataFromJson(elem, "height"), id};
#endif
            Types::Position position           = {Types::Position(Json::getInstance().getDataFromJson(elem, "position"))};
            Types::CollisionRect collisionRect = {Types::CollisionRect(Json::getInstance().getDataFromJson(elem, "collisionRect"))};
            Types::Rect rect                   = {Types::Rect(Json::getInstance().getDataFromJson(elem, "rect"))};
            struct health_s healthComp         = {Json::getInstance().getDataFromJson(elem, "health")};
            Types::Damage damageComp           = {Json::getInstance().getDataFromJson(elem, "damage")};
            Types::Velocity velocity           = {Types::Velocity(Json::getInstance().getDataFromJson(elem, "velocity"))};

            nlohmann::basic_json<> animRectData = Json::getInstance().getDataFromJson(elem, "animRect");
            Types::AnimRect animRect = {
                rect,
                Json::getInstance().getDataFromJson(animRectData, "move").get<std::vector<Types::Rect>>(),
                Json::getInstance().getDataFromJson(animRectData, "attack").get<std::vector<Types::Rect>>(),
                Json::getInstance().getDataFromJson(animRectData, "dead").get<std::vector<Types::Rect>>()
            };

            #ifdef CLIENT
                Registry::getInstance().getComponents<Raylib::Sprite>().insertBack(ennemy);
            #endif

            Registry::getInstance().getComponents<Types::Position>().insertBack(position);
            Registry::getInstance().getComponents<Types::CollisionRect>().insertBack(collisionRect);
            Registry::getInstance().getComponents<Types::Rect>().insertBack((rect));
            Registry::getInstance().getComponents<Types::AnimRect>().insertBack(animRect);
            Registry::getInstance().getComponents<Types::Velocity>().insertBack(velocity);
            Registry::getInstance().getComponents<struct health_s>().insertBack(healthComp);
            Registry::getInstance().getComponents<Types::Damage>().insertBack(damageComp);
            Registry::getInstance().setToFrontLayers(id);
        }
    }

    void initWave(std::size_t managerId, std::size_t systemId)
    {
        static std::size_t ennemyNumber = 5;
        const std::size_t spawnDelay    = 2;
        Clock &clock                    = Registry::getInstance().getClock();
        static std::size_t clockId      = clock.create(true);

        if (clock.elapsedSecondsSince(clockId) > spawnDelay) {
            initEnnemy(JsonType::DEFAULT_ENNEMY);
            ennemyNumber--;
            clock.restart(clockId);
        }
        if (ennemyNumber <= 0) {
            SystemManagersDirector::getInstance().getSystemManager(managerId).removeSystem(systemId);
        }
    }

    void checkDestroyAfterDeathCallBack(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        Registry &registry   = Registry::getInstance();
        auto deadList        = registry.getComponents<Types::Dead>();
        auto deadIdList      = deadList.getExistingsId();
        Clock &clock         = registry.getClock();
        std::size_t decrease = 0;

        for (auto id : deadIdList) {
            auto tmpId        = id - decrease;
            Types::Dead &dead = deadList[tmpId];
            if (static_cast<int>(dead.clockId) > -1
                && clock.elapsedMillisecondsSince(dead.clockId) > dead.timeToWait) {
                registry.removeEntity(tmpId);
                decrease++;
            }
        }
    }

    static void
    executeDeathFunction(std::size_t id, Registry::components<Types::Dead> arrDead, std::size_t &decrease)
    {
        if (arrDead.exist(id) && arrDead[id].deathFunction != std::nullopt) {
            Types::Dead &deadComp = arrDead[id];
            if (!deadComp.launched) {
                deadComp.deathFunction.value()(id);
                deadComp.clockId  = Registry::getInstance().getClock().create();
                deadComp.launched = true;
            }
        } else {
            Registry::getInstance().removeEntity(id);
            decrease++;
        }
    }

    static void sendEnemyDeath(std::size_t id)
    {
        auto &arrEnemies = Registry::getInstance().getComponents<Types::Enemy>();

        if (!arrEnemies.exist(id)) {
            return;
        }
        n_id_t castedId = static_cast<n_id_t>(id);
#ifdef CLIENT
        Nitwork::NitworkClient::getInstance().addEnemyDeathMsg(castedId);
#else
        Nitwork::NitworkServer::getInstance().addEnemyDeathMessage(castedId);
#endif
    }

    void deathChecker(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        Registry::components<struct health_s> arrHealth =
            Registry::getInstance().getComponents<struct health_s>();
        Registry::components<Types::Dead> arrDead = Registry::getInstance().getComponents<Types::Dead>();
        std::size_t decrease                      = 0;

        std::vector<std::size_t> ids = arrHealth.getExistingsId();
        for (auto &id : ids) {
            auto tmpId = id - decrease;
            if (arrHealth.exist(tmpId) && arrHealth[tmpId].hp <= 0) {
                sendEnemyDeath(tmpId);
                executeDeathFunction(tmpId, arrDead, decrease);
            }
        }
    }

    constexpr float outsideWindowTopLeft  = -25.0F;
    constexpr float outsideWindowBotRigth = 125.0F;

    static bool isOutsideWindow(const Types::Position &pos)
    {
        if (pos.x < outsideWindowTopLeft || pos.x > outsideWindowBotRigth || pos.y < outsideWindowTopLeft
            || pos.y > outsideWindowBotRigth) {
            return (true);
        }
        return (false);
    }

    void destroyOutsideWindow(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        Registry::components<Types::Position> arrPosition =
            Registry::getInstance().getComponents<Types::Position>();
#ifdef CLIENT
        Registry::components<Types::Parallax> arrParallax =
            Registry::getInstance().getComponents<Types::Parallax>();
#endif
        std::vector<std::size_t> ids =
            Registry::getInstance().getEntitiesByComponents({typeid(Types::Position)});
        std::size_t decrease = 0;

        for (auto &id : ids) {
            auto tmpId = id - decrease;
#ifdef CLIENT
            bool isNotParallax = !arrParallax.exist(tmpId);
#else
            bool isNotParallax = true;
#endif
            if (isNotParallax && isOutsideWindow(arrPosition[tmpId])) {
                Registry::getInstance().removeEntity(tmpId);
                decrease++;
            }
        }
    }

    const std::string playerFile = "assets/Json/playerData.json";

    void initPlayer(std::size_t managerId, std::size_t systemId)
    {
        std::size_t id          = Registry::getInstance().addEntity();

        Types::Player playerComp   = {};
        Types::Dead deadComp       = {Json::getInstance().getDataByVector({"player", "deadTime"} , JsonType::DEFAULT_PLAYER)};
        struct health_s healthComp = {Json::getInstance().getDataByVector({"player", "health"} , JsonType::DEFAULT_PLAYER)};
        Types::Damage damageComp   = {Json::getInstance().getDataByVector({"player", "damage"} , JsonType::DEFAULT_PLAYER)};
#ifdef CLIENT
        Raylib::Sprite sprite = {
            Json::getInstance().getDataByVector({"player", "spritePath"}, JsonType::DEFAULT_PLAYER),
            Json::getInstance().getDataByVector({"player", "width"}, JsonType::DEFAULT_PLAYER),
            Json::getInstance().getDataByVector({"player", "height"}, JsonType::DEFAULT_PLAYER),
            id
        };
#endif
        Types::Position position           = {Types::Position(Json::getInstance().getDataByVector({"player", "position"} , JsonType::DEFAULT_PLAYER))};
        Types::Rect rect                   = {Types::Rect(Json::getInstance().getDataByVector({"player", "rect"} , JsonType::DEFAULT_PLAYER))};
        Types::CollisionRect collisionRect = {Types::CollisionRect(Json::getInstance().getDataByVector({"player", "collisionRect"} , JsonType::DEFAULT_PLAYER))};

        nlohmann::basic_json<> animRectData = Json::getInstance().getDataByVector({"player", "animRect"} , JsonType::DEFAULT_PLAYER);
        Types::AnimRect animRect = {
            rect,
            Json::getInstance().getDataFromJson(animRectData, "move").get<std::vector<Types::Rect>>(),
            Json::getInstance().getDataFromJson(animRectData, "attack").get<std::vector<Types::Rect>>(),
            Json::getInstance().getDataFromJson(animRectData, "dead").get<std::vector<Types::Rect>>()
        };

#ifdef CLIENT
        Registry::getInstance().getComponents<Raylib::Sprite>().insertBack(sprite);
#endif
        Registry::getInstance().getComponents<Types::Position>().insertBack(position);
        Registry::getInstance().getComponents<Types::Rect>().insertBack(rect);
        Registry::getInstance().getComponents<Types::CollisionRect>().insertBack(collisionRect);
        Registry::getInstance().getComponents<Types::AnimRect>().insertBack(animRect);
        Registry::getInstance().getComponents<Types::Player>().insertBack(playerComp);
        Registry::getInstance().getComponents<Types::Damage>().insertBack(damageComp);
        Registry::getInstance().getComponents<struct health_s>().insertBack(healthComp);
        Registry::getInstance().getComponents<Types::Dead>().insertBack(deadComp);
        Registry::getInstance().setToFrontLayers(id);
        SystemManagersDirector::getInstance().getSystemManager(managerId).removeSystem(systemId);
    }

    std::vector<std::function<void(std::size_t, std::size_t)>> getECSSystems()
    {
        return {
            windowCollision,
            checkDestroyAfterDeathCallBack,
            initPlayer,
            entitiesCollision,
            destroyOutsideWindow,
            deathChecker,
            initWave,
            moveEntities};
    }
} // namespace Systems
