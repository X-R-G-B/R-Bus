/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Systems implementation
*/

#include "Systems.hpp"
#include <cstddef>
#include <fstream>
#include <nlohmann/json.hpp>
#include <sstream>
#include "CustomTypes.hpp"
#include "Raylib.hpp"
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

    constexpr int maxOutParallaxLeft  = -100;
    constexpr int maxOutParallaxRight = 100;

    static void initParallaxEntity(
        nlohmann::json_abi_v3_11_2::basic_json<> &parallaxData,
        const float maxOffsideParallax = 0)
    {
        std::size_t id = Registry::getInstance().addEntity();

        Registry::getInstance().getComponents<Raylib::Sprite>().insertBack(
            {parallaxData["spritePath"], parallaxData["width"], parallaxData["height"], id});
        Registry::getInstance().getComponents<Types::Position>().insertBack(
            {Types::Position(parallaxData["position"])});
        Registry::getInstance().getComponents<Types::Velocity>().insertBack(
            {Types::Velocity(parallaxData["velocity"])});
        if (parallaxData["rect"] != nullptr) {
            Registry::getInstance().getComponents<Types::Rect>().insertBack(
                {Types::Rect(parallaxData["rect"])});
        }
        Registry::getInstance().setToBackLayers(id);
        Registry::components<Types::Position> arrPosition =
            Registry::getInstance().getComponents<Types::Position>();
        if (maxOffsideParallax > 0) {
            arrPosition[id].x += maxOffsideParallax;
        }
        Registry::getInstance().getComponents<Types::Parallax>().insertBack(
            {arrPosition[id].x, arrPosition[id].y});
    }

    const std::string parallaxFile = "assets/Json/parallaxData.json";

    void initParalax(std::size_t managerId, std::size_t systemId)
    {
        nlohmann::json jsonData = openJsonData(parallaxFile);

        for (auto &e : jsonData["parallax"]) {
            initParallaxEntity(e);
            if (e["copy"] != nullptr && e["copy"]) {
                initParallaxEntity(e, maxOutParallaxRight);
            }
        }
        SystemManagersDirector::getInstance().getSystemManager(managerId).removeSystem(systemId);
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

#ifndef NDEBUG
    void debugCollisionRect(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        Registry::components<Types::CollisionRect> arrCollisionRect =
            Registry::getInstance().getComponents<Types::CollisionRect>();
        Registry::components<Types::Position> arrPosition =
            Registry::getInstance().getComponents<Types::Position>();
        Registry::components<Types::RectangleShape> arrRectangleShape =
            Registry::getInstance().getComponents<Types::RectangleShape>();

        std::vector<std::size_t> ids = arrCollisionRect.getExistingsId();

        for (auto &id : ids) {
            if (arrPosition.exist(id) && !arrRectangleShape.exist(id)) {
                Registry::getInstance().getComponents<Types::RectangleShape>().insert(
                    id,
                    {arrCollisionRect[id].width, arrCollisionRect[id].height});
            }
        }
    }
#endif

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

        Registry::getInstance().getComponents<Raylib::Sprite>().insertBack(
            Raylib::Sprite(ennemyData["spritePath"], ennemyData["width"], ennemyData["height"], id));
        Registry::getInstance().getComponents<Types::Position>().insertBack(
            {Types::Position(ennemyData["position"])});
        Registry::getInstance().getComponents<Types::CollisionRect>().insertBack(
            (Types::CollisionRect(ennemyData["collisionRect"])));
        Registry::getInstance().getComponents<Types::Rect>().insertBack((Types::Rect(ennemyData["rect"])));
        nlohmann::json animRectData = ennemyData["animRect"];
        nlohmann::json moveData     = animRectData["move"];
        nlohmann::json attackData   = animRectData["attack"];
        nlohmann::json deadData     = animRectData["dead"];
        Registry::getInstance().getComponents<Types::AnimRect>().insertBack(Types::AnimRect(
            Types::Rect(ennemyData["rect"]),
            moveData.get<std::vector<Types::Rect>>(),
            attackData.get<std::vector<Types::Rect>>(),
            deadData.get<std::vector<Types::Rect>>()));
        Registry::getInstance().getComponents<Types::Velocity>().insertBack(
            {Types::Velocity(ennemyData["velocity"])});
        Registry::getInstance().getComponents<struct health_s>().insertBack({ennemyData["health"]});
        Registry::getInstance().getComponents<Types::Damage>().insertBack({ennemyData["damage"]});
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
        static std::size_t ennemyNumber     = 5;
        const std::size_t spawnDelay = 2;
        Clock &clock                 = Registry::getInstance().getClock();
        static std::size_t clockId   = clock.create(true);

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
        for (auto itIds = ids.begin(); itIds != ids.end(); itIds++) {
            auto tmpId = (*itIds) - decrease;
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
        Registry::components<Types::Parallax> arrParallax =
            Registry::getInstance().getComponents<Types::Parallax>();
        std::vector<std::size_t> ids =
            Registry::getInstance().getEntitiesByComponents({typeid(Types::Position)});
        std::size_t decrease = 0;

        for (auto &id : ids) {
            auto tmpId = id - decrease;
            if (!arrParallax.exist(tmpId) && isOutsideWindow(arrPosition[tmpId])) {
                Registry::getInstance().removeEntity(tmpId);
                decrease++;
            }
        }
    }

    static void resetParallaxPosition(
        std::size_t id,
        Registry::components<Types::Parallax> &arrParallax,
        Registry::components<Types::Position> &arrPosition)
    {
        if (arrPosition[id].x <= maxOutParallaxLeft) {
            if (arrParallax[id].x >= maxOutParallaxRight) {
                arrPosition[id].x = arrParallax[id].x;
            } else {
                arrPosition[id].x = arrParallax[id].x + maxOutParallaxRight;
            }
            arrPosition[id].y = arrParallax[id].y;
        }
    }

    void manageParallax(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        Registry::components<Types::Position> arrPosition =
            Registry::getInstance().getComponents<Types::Position>();
        Registry::components<Types::Parallax> arrParallax =
            Registry::getInstance().getComponents<Types::Parallax>();

        std::vector<std::size_t> ids = Registry::getInstance().getEntitiesByComponents(
            {typeid(Types::Position), typeid(Types::Parallax), typeid(Types::Velocity)});

        for (auto &id : ids) {
            resetParallaxPosition(id, arrParallax, arrPosition);
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
        Registry::getInstance().getComponents<Raylib::Sprite>().insertBack(
            {jsonData["spritePath"], jsonData["width"], jsonData["height"], id});
        Registry::getInstance().getComponents<Types::Position>().insertBack(
            {Types::Position(jsonData["position"])});
        Registry::getInstance().getComponents<Types::Rect>().insertBack({Types::Rect(jsonData["rect"])});
        Registry::getInstance().getComponents<Types::CollisionRect>().insertBack(
            {Types::CollisionRect(jsonData["collisionRect"])});
        nlohmann::json animRectData = jsonData["animRect"];
        nlohmann::json moveData     = animRectData["move"];
        nlohmann::json attackData   = animRectData["attack"];
        nlohmann::json deadData     = animRectData["dead"];
        Registry::getInstance().getComponents<Types::AnimRect>().insertBack(Types::AnimRect(
            Types::Rect(jsonData["rect"]),
            moveData.get<std::vector<Types::Rect>>(),
            attackData.get<std::vector<Types::Rect>>(),
            deadData.get<std::vector<Types::Rect>>()));
        Registry::getInstance().getComponents<Types::Player>().insertBack({});
        Registry::getInstance().getComponents<Types::Damage>().insertBack({jsonData["damage"]});
        Registry::getInstance().getComponents<struct health_s>().insertBack({jsonData["health"]});
        Registry::getInstance().getComponents<Types::Dead>().insertBack({std::nullopt, deathTime});
        Registry::getInstance().setToFrontLayers(id);
        SystemManagersDirector::getInstance().getSystemManager(managerId).removeSystem(systemId);
    }

    std::vector<std::function<void(std::size_t, std::size_t)>> getECSSystems()
    {
        return {
#ifndef NDEBUG
            debugCollisionRect,
#else
#endif
            windowCollision,
            checkDestroyAfterDeathCallBack,
            initPlayer,
            initParalax,
            entitiesCollision,
            destroyOutsideWindow,
            deathChecker,
            initWave,
            moveEntities,
            manageParallax};
    }
} // namespace Systems
