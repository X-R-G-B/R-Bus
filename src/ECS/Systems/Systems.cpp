/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Systems implementation
*/

#include <iostream>
#include "Systems.hpp"
#include <cstddef>
#include <fstream>
#include <nlohmann/json.hpp>
#include <sstream>
#include "CustomTypes.hpp"
#ifdef CLIENT
    #include "Raylib.hpp"
#endif
#include "Registry.hpp"
#include "SystemManagersDirector.hpp"

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

    static nlohmann::json openJsonData(const std::string &path)
    {
        std::ifstream fileData(path);
        std::ostringstream input;
        nlohmann::json jsonData = {};

        if (fileData.is_open()) {
            input << fileData.rdbuf();
            if (nlohmann::json::accept(input.str())) {
                jsonData = nlohmann::json::parse(input.str());
                return jsonData;
            }
        }
        Registry::getInstance().getLogger().error("Could not load json file. An error occured");
        return jsonData;
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

    static void initEnnemyEntity(nlohmann::json_abi_v3_11_2::basic_json<> &ennemyData)
    {
        std::size_t id = Registry::getInstance().addEntity();

#ifdef CLIENT
        Raylib::Sprite ennemy = {ennemyData["spritePath"], ennemyData["width"], ennemyData["height"], id};
#endif
        Types::Position position           = {Types::Position(ennemyData["position"])};
        Types::CollisionRect collisionRect = {Types::CollisionRect(ennemyData["collisionRect"])};
        Types::Rect rect                   = {Types::Rect(ennemyData["rect"])};
        struct health_s healthComp         = {ennemyData["health"]};
        Types::Damage damageComp           = {ennemyData["damage"]};
        Types::Velocity velocity           = {Types::Velocity(ennemyData["velocity"])};

        nlohmann::json animRectData = ennemyData["animRect"];
        nlohmann::json moveData     = animRectData["move"];
        nlohmann::json attackData   = animRectData["attack"];
        nlohmann::json deadData     = animRectData["dead"];

        Types::AnimRect animRect = {
            Types::Rect(ennemyData["rect"]),
            moveData.get<std::vector<Types::Rect>>(),
            attackData.get<std::vector<Types::Rect>>(),
            deadData.get<std::vector<Types::Rect>>()};

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
    }

    static void initEnnemy(const std::string &path)
    {
        nlohmann::json jsonData = openJsonData(path);

        if (jsonData["ennemy"] == nullptr) {
            return;
        }
        for (auto &ennemyData : jsonData["ennemy"]) {
            initEnnemyEntity(ennemyData);
        }
    }

    const std::string ennemyFile = "assets/Json/ennemyData.json";

    void initWave(std::size_t managerId, std::size_t systemId)
    {
        static std::size_t ennemyNumber = 5;
        const std::size_t spawnDelay    = 2;
        Clock &clock                    = Registry::getInstance().getClock();
        static std::size_t clockId      = clock.create(true);

        if (clock.elapsedSecondsSince(clockId) > spawnDelay) {
            initEnnemy(ennemyFile);
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
    const std::size_t deathTime  = 500;

    void initPlayer(std::size_t managerId, std::size_t systemId)
    {
        nlohmann::json jsonData = openJsonData(playerFile);
        std::size_t id          = Registry::getInstance().addEntity();
        if (jsonData["player"] == nullptr) {
            SystemManagersDirector::getInstance().getSystemManager(managerId).removeSystem(systemId);
            return;
        }
        jsonData = jsonData["player"];

        // Components

        Types::Player playerComp   = {};
        Types::Dead deadComp       = {std::nullopt};
        struct health_s healthComp = {jsonData["health"]};
        Types::Damage damageComp   = {jsonData["damage"]};
#ifdef CLIENT
        Raylib::Sprite sprite = {jsonData["spritePath"], jsonData["width"], jsonData["height"], id};
#endif
        Types::Position position           = {Types::Position(jsonData["position"])};
        Types::Rect rect                   = {Types::Rect(jsonData["rect"])};
        Types::CollisionRect collisionRect = {Types::CollisionRect(jsonData["collisionRect"])};

        // AnimRect
        nlohmann::json animRectData = jsonData["animRect"];
        nlohmann::json moveData     = animRectData["move"];
        nlohmann::json attackData   = animRectData["attack"];
        nlohmann::json deadData     = animRectData["dead"];

        Types::AnimRect animRect = {
            Types::Rect(jsonData["rect"]),
            moveData.get<std::vector<Types::Rect>>(),
            attackData.get<std::vector<Types::Rect>>(),
            deadData.get<std::vector<Types::Rect>>()};

        // Add components to registry

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
        std::cout << "player id: " << id << std::endl;
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
