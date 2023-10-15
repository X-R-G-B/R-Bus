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
#include "ECSCustomTypes.hpp"
#include "Registry.hpp"
#include "SystemManagersDirector.hpp"

#ifdef CLIENT
    #include "CustomTypes.hpp"
    #include "NitworkClient.hpp"
    #include "Raylib.hpp"
#else
    #include "NitworkServer.hpp"
#endif

extern "C"
{
#include "MessageTypes.h"
}

namespace Systems {
    constexpr float maxPercent = 100.0F;

    void windowCollision(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        Registry &registry                                = Registry::getInstance();
        Registry::components<Types::Position> arrPosition = registry.getComponents<Types::Position>();
        Registry::components<Types::CollisionRect> arrCollisionRect =
            registry.getComponents<Types::CollisionRect>();
        std::vector<std::size_t> ids = registry.getEntitiesByComponents(
            {typeid(Types::Player), typeid(Types::Position), typeid(Types::CollisionRect)});

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
        Registry::components<Types::Enemy> enemies  = registry.getComponents<Types::Enemy>();
        Registry::components<Types::PlayerAllies> playerAllies =
            registry.getComponents<Types::PlayerAllies>();
        Registry::components<Types::EnemyAllies> enemyAllies = registry.getComponents<Types::EnemyAllies>();

        if ((playerAllies.exist(fstId) && players.exist(scdId))
            || (playerAllies.exist(scdId) && players.exist(fstId))
            || (playerAllies.exist(fstId) && playerAllies.exist(scdId))
            || (players.exist(scdId) && players.exist(fstId))) {
            return true;
        }
        if ((enemyAllies.exist(fstId) && enemies.exist(scdId))
            || (enemyAllies.exist(scdId) && enemies.exist(fstId))
            || (enemyAllies.exist(fstId) && enemyAllies.exist(scdId))
            || (enemies.exist(scdId) && enemies.exist(fstId))) {
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

        if (arrDamage.exist(firstEntity) && arrDamage[firstEntity].damage > 0) {
            if (arrHealth.exist(secondEntity)) {
                arrHealth[secondEntity].hp -= arrDamage[firstEntity].damage;
#ifdef CLIENT
                sendLifeUpdateToServer(secondEntity, arrHealth);
#endif
            }
        }
    }

    static void checkSide(std::size_t firstEntity, std::size_t secondEntity)
    {
        if (checkAllies(firstEntity, secondEntity)) {
            return;
        }
        giveDamages(firstEntity, secondEntity);
        giveDamages(secondEntity, firstEntity);
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
                    checkSide(id, *itIds);
                }
            }
            itIds++;
        }
    }

    void entitiesCollision(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
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

    const std::size_t moveTime = 20;

    void moveEntities(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        Registry::components<Types::Position> arrPosition =
            Registry::getInstance().getComponents<Types::Position>();
        Registry::components<Types::Velocity> arrVelocity =
            Registry::getInstance().getComponents<Types::Velocity>();
        Clock &clock                 = Registry::getInstance().getClock();
        static std::size_t clockId   = clock.create();
        std::vector<std::size_t> ids = Registry::getInstance().getEntitiesByComponents(
            {typeid(Types::Position), typeid(Types::Velocity)});

        while (clock.elapsedMillisecondsSince(clockId) >= moveTime) {
            for (auto &id : ids) {
                arrPosition[id].x += arrVelocity[id].speedX;
                arrPosition[id].y += arrVelocity[id].speedY;
            }
            clock.decreaseMilliseconds(clockId, moveTime);
        }
    }

    void initEnemy(enemy_type_e enemyType, bool setId, struct ::enemy_id_s enemyId)
    {
        JsonType jsonType = messageTypes.at(enemyType);
        std::vector<nlohmann::basic_json<>> enemyData =
            Json::getInstance().getDataByJsonType("enemy", jsonType);

        for (auto &elem : enemyData) {
#ifdef CLIENT
            std::size_t id = Registry::getInstance().addEntity();
#else
            Registry::getInstance().addEntity();
#endif

#ifdef CLIENT

            Types::SpriteDatas enemy = {
                Json::getInstance().getDataFromJson<std::string>(elem, "spritePath"),
                Json::getInstance().getDataFromJson<float>(elem, "width"),
                Json::getInstance().getDataFromJson<float>(elem, "height"),
                id,
                LayerType::DEFAULTLAYER,
                0};

            Types::Rect rect = Json::getInstance().getDataFromJson<Types::Rect>(elem, "rect");

            nlohmann::basic_json<> animRectData =
                Json::getInstance().getDataFromJson<nlohmann::basic_json<>>(elem, "animRect");
            Types::AnimRect animRect = {
                rect,
                Json::getInstance().getDataFromJson<std::vector<Types::Rect>>(animRectData, "move"),
                Json::getInstance().getDataFromJson<std::vector<Types::Rect>>(animRectData, "attack"),
                Json::getInstance().getDataFromJson<std::vector<Types::Rect>>(animRectData, "dead")};
            animRect.changeRectList(Types::RectListType::MOVE);

#endif
            Types::Enemy enemyComp = (setId ? Types::Enemy {enemyId} : Types::Enemy {});
            Types::Velocity velocity =
                Json::getInstance().getDataFromJson<Types::Velocity>(elem, "velocity");
            Types::Position position =
                Json::getInstance().getDataFromJson<Types::Position>(elem, "position");
            Types::CollisionRect collisionRect =
                Json::getInstance().getDataFromJson<Types::CollisionRect>(elem, "collisionRect");
            Types::Damage damageComp   = {Json::getInstance().getDataFromJson<int>(elem, "damage")};
            struct health_s healthComp = {Json::getInstance().getDataFromJson<int>(elem, "health")};

#ifdef CLIENT
            Registry::getInstance().getComponents<Types::Rect>().insertBack(rect);
            Registry::getInstance().getComponents<Types::AnimRect>().insertBack(animRect);
            Registry::getInstance().getComponents<Types::SpriteDatas>().insertBack(enemy);
#endif
            if (jsonType == JsonType::TERMINATOR) {
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

    void manageBoss(std::size_t managerId, std::size_t systemId)
    {
        const float bossSpeed = 0.2F;
        Registry::components<Types::Position> &arrPosition =
            Registry::getInstance().getComponents<Types::Position>();
        Registry::components<Types::Velocity> &arrVelocity =
            Registry::getInstance().getComponents<Types::Velocity>();
        Registry::components<Types::CollisionRect> &arrCollisonRect =
            Registry::getInstance().getComponents<Types::CollisionRect>();
        std::vector<std::size_t> ids =
            Registry::getInstance().getEntitiesByComponents({typeid(Types::Boss)});

        if (ids.empty()) {
            SystemManagersDirector::getInstance().getSystemManager(managerId).removeSystem(systemId);
        }
        for (auto &id : ids) {
            if (arrPosition[id].y < 0) {
                arrVelocity[id].speedY = bossSpeed;
            }
            if (arrPosition[id].y + arrCollisonRect[id].height > maxPercent) {
                arrVelocity[id].speedY = -bossSpeed;
            }
        }
    }

    void initWave(std::size_t managerId, std::size_t systemId)
    {
        static std::size_t enemyNumber = 5;
        const std::size_t spawnDelay   = 2;
        Clock &clock                   = Registry::getInstance().getClock();
        static std::size_t clockId     = clock.create(true);
        static bool fstCall            = true;

        Registry::components<Types::Boss> &bossArr = Registry::getInstance().getComponents<Types::Boss>();
        Registry::components<Types::Enemy> &enemyArr =
            Registry::getInstance().getComponents<Types::Enemy>();

        if (fstCall) {
            fstCall = false;
            clock.restart(clockId);
        }
        if (clock.elapsedSecondsSince(clockId) >= spawnDelay && enemyNumber > 0) {
            initEnemy(CLASSIC_ENEMY);
            enemyNumber--;
            clock.decreaseSeconds(clockId, spawnDelay);
        }
        if (enemyArr.getExistingsId().empty() && enemyNumber <= 0 && bossArr.getExistingsId().empty()) {
            initEnemy(TERMINATOR);
            SystemManagersDirector::getInstance().getSystemManager(managerId).addSystem(manageBoss);
            SystemManagersDirector::getInstance().getSystemManager(managerId).removeSystem(systemId);
        }
    }

    void checkDestroyAfterDeathCallBack(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        Registry &registry   = Registry::getInstance();
        auto deadList        = registry.getComponents<Types::Dead>();
        auto deadIdList      = deadList.getExistingsId();
        Clock &clock         = registry.getClock();
        std::size_t decrease = 0;

        std::sort(deadIdList.begin(), deadIdList.end());
        for (auto id : deadIdList) {
            auto tmpId        = id - decrease;
            Types::Dead &dead = deadList[tmpId];
            if (static_cast<int>(dead.clockId) > -1
                && clock.elapsedMillisecondsSince(dead.clockId) >= dead.timeToWait) {
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

    static void sendEnemyDeath(std::size_t arrId)
    {
        auto &arrEnemies = Registry::getInstance().getComponents<Types::Enemy>();

        if (!arrEnemies.exist(arrId)) {
            return;
        }
#ifdef CLIENT
        Nitwork::NitworkClient::getInstance().addEnemyDeathMsg(arrEnemies[arrId].getConstId().id);
#else
        Nitwork::NitworkServer::getInstance().addEnemyDeathMessage(arrEnemies[arrId].getConstId().id);
#endif
    }

    void deathChecker(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        Registry::components<struct health_s> arrHealth =
            Registry::getInstance().getComponents<struct health_s>();
        Registry::components<Types::Dead> arrDead = Registry::getInstance().getComponents<Types::Dead>();
        std::size_t decrease                      = 0;

        std::vector<std::size_t> ids = arrHealth.getExistingsId();
        std::sort(ids.begin(), ids.end());
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
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        Registry::components<Types::Position> arrPosition =
            Registry::getInstance().getComponents<Types::Position>();
#ifdef CLIENT
        Registry::components<Types::Parallax> arrParallax =
            Registry::getInstance().getComponents<Types::Parallax>();
#endif
        std::vector<std::size_t> ids =
            Registry::getInstance().getEntitiesByComponents({typeid(Types::Position)});
        std::size_t decrease = 0;

        std::sort(ids.begin(), ids.end());
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

    void initPlayer(JsonType playerType, unsigned int constId, bool otherPlayer)
    {
#ifdef CLIENT
        std::size_t id = Registry::getInstance().addEntity();
#else
        Registry::getInstance().addEntity();
#endif

        Types::Dead deadComp = {Json::getInstance().getDataByVector({"player", "deadTime"}, playerType)};
        struct health_s healthComp = {
            Json::getInstance().getDataByVector({"player", "health"}, playerType)};
        Types::Damage damageComp = {Json::getInstance().getDataByVector({"player", "damage"}, playerType)};
#ifdef CLIENT
        Types::SpriteDatas playerDatas(
            Json::getInstance().getDataByVector({"player", "spritePath"}, playerType),
            Json::getInstance().getDataByVector({"player", "width"}, playerType),
            Json::getInstance().getDataByVector({"player", "height"}, playerType),
            id,
            FRONTLAYER,
            static_cast<std::size_t>(FRONT));

        Types::Rect rect = {
            Types::Rect(Json::getInstance().getDataByVector({"player", "rect"}, playerType))};
        nlohmann::basic_json<> animRectData =
            Json::getInstance().getDataByVector({"player", "animRect"}, playerType);
        Types::AnimRect animRect = {
            rect,
            Json::getInstance().getDataFromJson<std::vector<Types::Rect>>(animRectData, "move"),
            Json::getInstance().getDataFromJson<std::vector<Types::Rect>>(animRectData, "attack"),
            Json::getInstance().getDataFromJson<std::vector<Types::Rect>>(animRectData, "dead")};

#endif

        // Add components to registry
        if (otherPlayer) {
            Types::OtherPlayer otherPlayerComp(constId);
            Registry::getInstance().getComponents<Types::OtherPlayer>().insertBack(otherPlayerComp);
        } else {
            Types::Player playerComp = {constId};
            Registry::getInstance().getComponents<Types::Player>().insertBack(playerComp);
        }

        Types::Position position = {
            Types::Position(Json::getInstance().getDataByVector({"player", "position"}, playerType))};
        Types::CollisionRect collisionRect = {Types::CollisionRect(
            Json::getInstance().getDataByVector({"player", "collisionRect"}, playerType))};

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
    }

    void createMissile(Types::Position &pos, Types::Missiles &typeOfMissile)
    {
#ifdef CLIENT
        std::size_t entityId = Registry::getInstance().addEntity();
#else
        Registry::getInstance().addEntity();
#endif

        constexpr float bulletWidth          = 5.0F;
        constexpr float bulletHeight         = 5.0F;
        Types::CollisionRect collisionRect   = {bulletWidth, bulletHeight};
        Types::Velocity velocity             = {0.7F, 0.0F};
        Types::Missiles missileType          = typeOfMissile;
        Types::Dead deadComp                 = {};
        Types::PlayerAllies playerAlliesComp = {};
        Types::Position position             = {pos.x, pos.y};
#ifdef CLIENT
        const std::string bulletPath = "assets/R-TypeSheet/r-typesheet1.gif";
        Types::Rect spriteRect       = {200, 121, 32, 10};
        Types::SpriteDatas bulletDatas(
            bulletPath,
            bulletWidth,
            bulletHeight,
            entityId,
            FRONTLAYER,
            static_cast<std::size_t>(FRONT));
#endif
        struct health_s healthComp = {1};
        Types::Damage damageComp   = {10};

        Registry::getInstance().getComponents<Types::Position>().insertBack(position);
#ifdef CLIENT
        Registry::getInstance().getComponents<Types::SpriteDatas>().insertBack(bulletDatas);
        Registry::getInstance().getComponents<Types::Rect>().insertBack(spriteRect);
#endif
        Registry::getInstance().getComponents<Types::CollisionRect>().insertBack(collisionRect);
        Registry::getInstance().getComponents<Types::Missiles>().insertBack(missileType);
        Registry::getInstance().getComponents<Types::PlayerAllies>().insertBack(playerAlliesComp);
        Registry::getInstance().getComponents<Types::Velocity>().insertBack(velocity);
        Registry::getInstance().getComponents<struct health_s>().insertBack(healthComp);
        Registry::getInstance().getComponents<Types::Damage>().insertBack(damageComp);
        Registry::getInstance().getComponents<Types::Dead>().insertBack(deadComp);
    }

    std::vector<std::function<void(std::size_t, std::size_t)>> getECSSystems()
    {
        return {
            windowCollision,
            checkDestroyAfterDeathCallBack,
            entitiesCollision,
            destroyOutsideWindow,
            deathChecker,
            moveEntities};
    }
} // namespace Systems
