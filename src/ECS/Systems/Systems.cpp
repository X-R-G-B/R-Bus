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
        Registry::components<health_s> arrHealth = Registry::getInstance().getComponents<health_s>();

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
        const float maxOffsideParallax = 0,
        bool isCopy                    = false)
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
        Registry::getInstance().getComponents<Types::Parallax>().insertBack({});
        Registry::components<Types::Position> arrPosition =
            Registry::getInstance().getComponents<Types::Position>();
        if (isCopy) {
            arrPosition[id].x += maxOffsideParallax;
        }
        Registry::getInstance().getComponents<Types::InitialPosition>().insertBack(
            {arrPosition[id].x, arrPosition[id].y});
        if (parallaxData["copy"] != nullptr && parallaxData["copy"] == true && isCopy == false) {
            initParallaxEntity(parallaxData, maxOutParallaxRight, true);
        }
    }

    const std::string parallaxFile = "assets/Json/parallaxData.json";

    void initParalax(std::size_t managerId, std::size_t systemId)
    {
        nlohmann::json jsonData = openJsonData(parallaxFile);

        for (auto &e : jsonData["parallax"]) {
            initParallaxEntity(e);
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

    void checkDestroyAfterDeathCallBack(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        Registry &registry = Registry::getInstance();
        auto deadList      = registry.getComponents<Types::Dead>();
        auto deadIdList    = deadList.getExistingsId();
        Clock &clock       = registry.getClock();

        for (auto id : deadIdList) {
            Types::Dead &dead = deadList[id];
            if (static_cast<int>(dead.clockId) > -1
                && clock.elapsedMillisecondsSince(dead.clockId) > dead.timeToWait) {
                clock.restart(dead.clockId);
                registry.removeEntity(id);
            }
        }
    }

    static void executeDeathFunction(std::size_t id, Registry::components<Types::Dead> arrDead)
    {
        Types::Dead &deadComp = arrDead[id];
        if (deadComp.deathFunction != std::nullopt) {
            if (!deadComp.launched) {
                deadComp.deathFunction.value()(id);
                deadComp.clockId  = Registry::getInstance().getClock().create();
                deadComp.launched = true;
            }
        } else {
            Registry::getInstance().removeEntity(id);
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

    void manageOutsideWindowEntity(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        Registry::components<Types::Position> arrPosition =
            Registry::getInstance().getComponents<Types::Position>();
        Registry::components<Types::Parallax> arrParallax =
            Registry::getInstance().getComponents<Types::Parallax>();

        std::vector<std::size_t> ids = arrPosition.getExistingsId();

        for (auto &id : ids) {
            if (!arrParallax.exist(id)) {
                if (isOutsideWindow(arrPosition[id])) {
                    Registry::getInstance().removeEntity(id);
                }
            }
        }
    }

    static void resetParallaxPosition(
        std::size_t id,
        Registry::components<Types::InitialPosition> &arrInitialPos,
        Registry::components<Types::Position> &arrPosition)
    {
        if (arrPosition[id].x <= maxOutParallaxLeft) {
            if (arrInitialPos[id].x >= maxOutParallaxRight) {
                arrPosition[id].x = arrInitialPos[id].x;
            } else {
                arrPosition[id].x = arrInitialPos[id].x + maxOutParallaxRight;
            }
            arrPosition[id].y = arrInitialPos[id].y;
        }
    }

    void manageParallax(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        Registry::components<Types::Position> arrPosition =
            Registry::getInstance().getComponents<Types::Position>();
        Registry::components<Types::Parallax> arrParallax =
            Registry::getInstance().getComponents<Types::Parallax>();
        Registry::components<Types::Velocity> arrVelocity =
            Registry::getInstance().getComponents<Types::Velocity>();
        Registry::components<Types::InitialPosition> arrInitialPos =
            Registry::getInstance().getComponents<Types::InitialPosition>();

        std::vector<std::size_t> ids = arrParallax.getExistingsId();

        for (auto &id : ids) {
            if (arrPosition.exist(id) && arrVelocity.exist(id) && arrInitialPos.exist(id)) {
                resetParallaxPosition(id, arrInitialPos, arrPosition);
            }
        }
    }

    void deathChecker(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        Registry::components<health_s> arrHealth  = Registry::getInstance().getComponents<health_s>();
        Registry::components<Types::Dead> arrDead = Registry::getInstance().getComponents<Types::Dead>();

        std::vector<std::size_t> ids = arrHealth.getExistingsId();
        for (auto itIds = ids.begin(); itIds != ids.end(); itIds++) {
            if (arrHealth.exist(*itIds) && arrHealth[*itIds].hp <= 0 && arrDead.exist(*itIds)) {
                executeDeathFunction(*itIds, arrDead);
            }
        }
    }

    const std::string playerFile = "assets/Json/playerData.json";

    void initPlayer(std::size_t managerId, std::size_t systemId)
    {
        nlohmann::json jsonData = openJsonData(playerFile);
        std::size_t id          = Registry::getInstance().addEntity();

        if (jsonData["player"] == nullptr) {
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
        Registry::getInstance().getComponents<Types::Dead>().insertBack({std::nullopt});
        Registry::getInstance().setToFrontLayers(id);
        SystemManagersDirector::getInstance().getSystemManager(managerId).removeSystem(systemId);
    }

    void initAudio(std::size_t /*managerId*/, std::size_t /*systemId*/)
    {
        const std::string musicPath = "assets/Audio/Musics/Title.mp3";
        const std::string soundPath = "assets/Audio/Sounds/fire.ogg";
        constexpr float musicVolume = 0.10F;
        constexpr float soundVolume = 0.13F;

        Raylib::Music music(musicPath, musicVolume);
        Raylib::Sound sound(soundPath, soundVolume);

        Registry::getInstance().addEntity();
        Registry::getInstance().getComponents<Raylib::Music>().insertBack(music);
        Registry::getInstance().getComponents<Raylib::Sound>().insertBack(sound);
        SystemManagersDirector::getInstance().getSystemManager(managerId).removeSystem(systemId);
    }

    std::vector<std::function<void(std::size_t, std::size_t)>> getECSSystems()
    {
        return {
            windowCollision,
            initPlayer,
            initParalax,
            initAudio,
            entitiesCollision,
            moveEntities,
#ifndef NDEBUG
            debugCollisionRect,
#else
#endif
            deathChecker,
            manageOutsideWindowEntity,
            manageParallax};
    }
} // namespace Systems