/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Systems implementation
*/

#include "Systems.hpp"
#include <fstream>
#include <sstream>
#include "ECSCustomTypes.hpp"
#include "Maths.hpp"
#include "Registry.hpp"
#include "SystemManagersDirector.hpp"

#ifdef CLIENT
    #include "CustomTypes.hpp"
    #include "NitworkClient.hpp"
    #include "Raylib.hpp"
#else
    #include "NitworkServer.hpp"
#endif

namespace Systems {
    constexpr float maxPercent = 100.0F;

    static void checkOutsideWindow(std::vector<std::size_t> ids)
    {
        Registry &registry                                = Registry::getInstance();
        Registry::components<Types::Position> arrPosition = registry.getComponents<Types::Position>();
        Registry::components<Types::CollisionRect> arrCollisionRect =
            registry.getComponents<Types::CollisionRect>();

        for (std::size_t id : ids) {
            if (arrPosition[id].x < 0) {
                arrPosition[id].x = 0;
            }
            if (arrPosition[id].y < 0) {
                arrPosition[id].y = 0;
            }
            if (Maths::intToFloatConservingDecimals(arrPosition[id].x)
                    + Maths::intToFloatConservingDecimals(arrCollisionRect[id].width)
                > maxPercent) {
                arrPosition[id].x = Maths::floatToIntConservingDecimals(
                    maxPercent - Maths::intToFloatConservingDecimals(arrCollisionRect[id].width));
            }
            if (Maths::intToFloatConservingDecimals(arrPosition[id].y)
                    + Maths::intToFloatConservingDecimals(arrCollisionRect[id].height)
                > maxPercent) {
                arrPosition[id].y = Maths::floatToIntConservingDecimals(
                    maxPercent - Maths::intToFloatConservingDecimals(arrCollisionRect[id].height));
            }
        }
    }

    void windowCollision(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        Registry &registry           = Registry::getInstance();
        std::vector<std::size_t> ids = registry.getEntitiesByComponents(
            {typeid(Types::Player), typeid(Types::Position), typeid(Types::CollisionRect)});
        std::vector<std::size_t> idsOtherPlayer = registry.getEntitiesByComponents(
            {typeid(Types::OtherPlayer), typeid(Types::Position), typeid(Types::CollisionRect)});

        checkOutsideWindow(ids);
        checkOutsideWindow(idsOtherPlayer);
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
        Types::Position realPosFstEntity   = {entityPos.x + entityColl.offsetX, entityPos.y + entityColl.offsetY};
        itIds++;
        while (itIds != ids.end()) {
            if (arrCollisionRect.exist(*itIds)) {
                Types::Position realPosSndEntity   = {arrPosition[*itIds].x + arrCollisionRect[*itIds].offsetX, arrPosition[*itIds].y + arrCollisionRect[*itIds].offsetY};
                if (Maths::intToFloatConservingDecimals(realPosFstEntity.x)
                        < (Maths::intToFloatConservingDecimals(realPosSndEntity.x)
                           + Maths::intToFloatConservingDecimals(arrCollisionRect[*itIds].width))
                    && (Maths::intToFloatConservingDecimals(realPosFstEntity.x)
                        + Maths::intToFloatConservingDecimals(entityColl.width))
                        > Maths::intToFloatConservingDecimals(realPosSndEntity.x)
                    && Maths::intToFloatConservingDecimals(realPosFstEntity.y)
                        < (Maths::intToFloatConservingDecimals(realPosSndEntity.y)
                           + Maths::intToFloatConservingDecimals(arrCollisionRect[*itIds].height))
                    && (Maths::intToFloatConservingDecimals(realPosFstEntity.y)
                        + Maths::intToFloatConservingDecimals(entityColl.height))
                        > Maths::intToFloatConservingDecimals(realPosSndEntity.y)) {
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
                Maths::addFloatToDecimalInt(
                    arrPosition[id].x,
                    Maths::intToFloatConservingDecimals(arrVelocity[id].speedX));
                Maths::addFloatToDecimalInt(
                    arrPosition[id].y,
                    Maths::intToFloatConservingDecimals(arrVelocity[id].speedY));
            }
            clock.decreaseMilliseconds(clockId, moveTime);
        }
    }

    void checkDestroyAfterDeathCallBack(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        Registry &registry   = Registry::getInstance();
        auto &deadList       = registry.getComponents<Types::Dead>();
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
#ifdef CLIENT
        auto &arrPlayer      = Registry::getInstance().getComponents<Types::Player>();
        auto &arrOtherPlayer = Registry::getInstance().getComponents<Types::OtherPlayer>();
#endif

        if (arrDead.exist(id) && arrDead[id].deathFunction != std::nullopt) {
#ifdef CLIENT
            if (arrPlayer.exist(id)) {
                Nitwork::NitworkClient::getInstance().addPlayerDeathMsg(arrPlayer[id].constId);
            } else if (arrOtherPlayer.exist(id)) {
                Nitwork::NitworkClient::getInstance().addPlayerDeathMsg(arrOtherPlayer[id].constId);
            }
#endif
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

    static void sendDeathMsg(std::size_t arrId)
    {
        auto &arrEnemies  = Registry::getInstance().getComponents<Types::Enemy>();
        auto &arrMissiles = Registry::getInstance().getComponents<Types::Missiles>();

#ifndef CLIENT
        auto &arrOtherPlayer = Registry::getInstance().getComponents<Types::OtherPlayer>();
        if (arrOtherPlayer.exist(arrId)) {
            Nitwork::NitworkServer::getInstance().addPlayerDeathMsg(arrOtherPlayer[arrId].constId);
        }
#endif

        if (arrMissiles.exist(arrId)) {
#ifdef CLIENT
            Nitwork::NitworkClient::getInstance().addMissileDeathMsg(arrMissiles[arrId].constId);
#else
            Nitwork::NitworkServer::getInstance().addMissileDeathMsg(arrMissiles[arrId].constId);
#endif
        }

        if (arrEnemies.exist(arrId)) {
#ifdef CLIENT
            Nitwork::NitworkClient::getInstance().addEnemyDeathMsg(arrEnemies[arrId].getConstId().id);
#else
            Nitwork::NitworkServer::getInstance().addEnemyDeathMessage(arrEnemies[arrId].getConstId().id);
#endif
        }
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
                sendDeathMsg(tmpId);
                executeDeathFunction(tmpId, arrDead, decrease);
            }
        }
    }

    constexpr float outsideWindowTopLeft  = -25.0F;
    constexpr float outsideWindowBotRigth = 125.0F;

    static bool isOutsideWindow(const Types::Position &pos)
    {
        if (Maths::intToFloatConservingDecimals(pos.x < outsideWindowTopLeft)
            || Maths::intToFloatConservingDecimals(pos.x) > outsideWindowBotRigth
            || Maths::intToFloatConservingDecimals(pos.y) < outsideWindowTopLeft
            || Maths::intToFloatConservingDecimals(pos.y) > outsideWindowBotRigth) {
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

    void initPlayer(
        unsigned int constId,
        const struct position_absolute_s &pos,
        const struct health_s &life,
        bool otherPlayer)
    {
        JsonType playerType = JsonType::DEFAULT_PLAYER;

        Registry::getInstance().addEntity();

        Types::Dead deadComp = {
            Json::getInstance().getDataByVector<std::size_t>({"player", "deadTime"}, playerType)};
        struct health_s healthComp = life;
        Types::Damage damageComp = {Json::getInstance().getDataByVector({"player", "damage"}, playerType)};
#ifdef CLIENT
        Types::SpriteDatas playerDatas(
            Json::getInstance().getDataByVector({"player", "spritePath"}, playerType),
            Json::getInstance().getDataByVector({"player", "width"}, playerType),
            Json::getInstance().getDataByVector({"player", "height"}, playerType),
            FRONTLAYER,
            static_cast<std::size_t>(FRONT));

        Types::Rect rect = {
            Types::Rect(Json::getInstance().getDataByVector({"player", "rect"}, playerType))};
        nlohmann::basic_json<> animRectData =
            Json::getInstance().getDataByVector({"player", "animRect"}, playerType);
        Types::AnimRect animRect(rect, animRectData, Types::RectListType::MOVE);
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
        Types::Position position           = {pos.x, pos.y};
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

    std::vector<std::function<void(std::size_t, std::size_t)>> getECSSystems()
    {
        std::vector<std::function<void(std::size_t, std::size_t)>> EcsSystems {
            windowCollision,
            checkDestroyAfterDeathCallBack,
            entitiesCollision,
            destroyOutsideWindow,
            deathChecker,
            moveEntities};

        std::vector<std::function<void(std::size_t, std::size_t)>> bulletSystems = getBulletsSystems();
        std::vector<std::function<void(std::size_t, std::size_t)>> physicSystems = getPhysicSystems();
        std::vector<std::function<void(std::size_t, std::size_t)>> waveSystems = getWaveSystems();

        EcsSystems.insert(EcsSystems.end(), bulletSystems.begin(), bulletSystems.end());
        EcsSystems.insert(EcsSystems.end(), physicSystems.begin(), physicSystems.end());
        EcsSystems.insert(EcsSystems.end(), waveSystems.begin(), waveSystems.end());
        return EcsSystems;
    }
} // namespace Systems
