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
#ifdef CLIENT
    #include "NitworkClient.hpp"
    #include "Raylib.hpp"
#else
    #include "NitworkServer.hpp"
#endif

namespace Systems {

    void windowCollision(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
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
        std::vector<std::size_t> ids = arrPosition.getExistingsId();

        if (clock.elapsedMillisecondsSince(clockId) < moveTime) {
            return;
        }
        for (auto &id : ids) {
            if (arrVelocity.exist(id)) {
                arrPosition[id].x += arrVelocity[id].speedX;
                arrPosition[id].y += arrVelocity[id].speedY;
            }
        }
        clock.restart(clockId);
    }

    static void initEnemyEntity(nlohmann::json_abi_v3_11_2::basic_json<> &enemyData)
    {
#ifdef CLIENT
        std::size_t id       = Registry::getInstance().addEntity();
        Raylib::Sprite enemy = {enemyData["spritePath"], enemyData["width"], enemyData["height"], id};
#else
        Registry::getInstance().addEntity();
#endif
        Types::Position position           = {Types::Position(enemyData["position"])};
        Types::CollisionRect collisionRect = {Types::CollisionRect(enemyData["collisionRect"])};
        Types::Rect rect                   = {Types::Rect(enemyData["rect"])};
        struct health_s healthComp         = {enemyData["health"]};
        Types::Damage damageComp           = {enemyData["damage"]};
        Types::Enemy enemyStruct           = {};
        Types::Velocity velocity           = {Types::Velocity(enemyData["velocity"])};

        nlohmann::json animRectData = enemyData["animRect"];
        nlohmann::json moveData     = animRectData["move"];
        nlohmann::json attackData   = animRectData["attack"];
        nlohmann::json deadData     = animRectData["dead"];

        Types::AnimRect animRect = {
            Types::Rect(enemyData["rect"]),
            moveData.get<std::vector<Types::Rect>>(),
            attackData.get<std::vector<Types::Rect>>(),
            deadData.get<std::vector<Types::Rect>>()};

#ifdef CLIENT
        Registry::getInstance().getComponents<Raylib::Sprite>().insertBack(enemy);
#endif
        Registry::getInstance().getComponents<Types::Position>().insertBack(position);
        Registry::getInstance().getComponents<Types::CollisionRect>().insertBack(collisionRect);
        Registry::getInstance().getComponents<Types::Rect>().insertBack((rect));
        Registry::getInstance().getComponents<Types::AnimRect>().insertBack(animRect);
        Registry::getInstance().getComponents<Types::Velocity>().insertBack(velocity);
        Registry::getInstance().getComponents<struct health_s>().insertBack(healthComp);
        Registry::getInstance().getComponents<Types::Damage>().insertBack(damageComp);
        Registry::getInstance().getComponents<Types::Enemy>().insertBack(enemyStruct);
    }

    void initEnemy(const std::string &path)
    {
        nlohmann::json jsonData = openJsonData(path);

        if (jsonData["enemy"] == nullptr) {
            return;
        }
        for (auto &enemyData : jsonData["enemy"]) {
            initEnemyEntity(enemyData);
        }
    }

    const std::string enemyFile = "assets/Json/enemyData.json";

    void initWave(std::size_t managerId, std::size_t systemId)
    {
        static std::size_t enemyNumber = 5;
        const std::size_t spawnDelay   = 2;
        Clock &clock                   = Registry::getInstance().getClock();
        static std::size_t clockId     = clock.create(true);

        if (clock.elapsedSecondsSince(clockId) > spawnDelay) {
            initEnemy(enemyFile);
            enemyNumber--;
            clock.restart(clockId);
        }
        if (enemyNumber <= 0) {
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

    void initPlayer()
    {
#ifdef CLIENT
        std::size_t id = Registry::getInstance().addEntity();
#else
        Registry::getInstance().addEntity();
#endif
        nlohmann::json jsonData = openJsonData(playerFile);
        if (jsonData["player"] == nullptr) {
            return;
        }
        jsonData = jsonData["player"];

        // Components

        Types::Player playerComp   = {0};
        Types::Dead deadComp       = {jsonData["deadTime"]};
        struct health_s healthComp = {jsonData["health"]};
        Types::Damage damageComp   = {jsonData["damage"]};
#ifdef CLIENT
        Types::SpriteDatas playerDatas(
            jsonData["spritePath"],
            jsonData["width"],
            jsonData["height"],
            id,
            FRONTLAYER,
            static_cast<std::size_t>(FRONT));
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
        Registry::getInstance().getComponents<Types::SpriteDatas>().insertBack(playerDatas);
#endif
        Registry::getInstance().getComponents<Types::Position>().insertBack(position);
        Registry::getInstance().getComponents<Types::Rect>().insertBack(rect);
        Registry::getInstance().getComponents<Types::CollisionRect>().insertBack(collisionRect);
        Registry::getInstance().getComponents<Types::AnimRect>().insertBack(animRect);
        Registry::getInstance().getComponents<Types::Player>().insertBack(playerComp);
        Registry::getInstance().getComponents<Types::Damage>().insertBack(damageComp);
        Registry::getInstance().getComponents<struct health_s>().insertBack(healthComp);
        Registry::getInstance().getComponents<Types::Dead>().insertBack(deadComp);
    }

    void createMissile(Types::Position &pos, Types::Missiles &typeOfMissile)
    {
        std::size_t entityId = Registry::getInstance().addEntity();

        Types::CollisionRect collisionRect1  = {1, 1};
        constexpr float bulletWidth          = 5.0F;
        constexpr float bulletHeight         = 5.0F;
        Types::CollisionRect collisionRect2  = {bulletWidth, bulletHeight};
        Types::Velocity velocity             = {0.7F, 0.0F};
        Types::Missiles missileType          = typeOfMissile;
        Types::Dead deadComp                 = {};
        Types::PlayerAllies playerAlliesComp = {};
        Types::Position position             = {pos.x, pos.y};
#ifdef CLIENT
        const std::string bulletPath         = "assets/R-TypeSheet/r-typesheet1.gif";
        Types::Rect spriteRect               = {200, 121, 32, 10};
        Types::SpriteDatas bulletDatas(
            bulletPath,
            bulletWidth,
            bulletHeight,
            entityId,
            FRONTLAYER,
            static_cast<std::size_t>(FRONT));
#endif
        struct health_s healthComp           = {1};
        Types::Damage damageComp             = {10};

        Registry::getInstance().getComponents<Types::Position>().insertBack(position);
#ifdef CLIENT
        Registry::getInstance().getComponents<Types::SpriteDatas>().insertBack(bulletDatas);
        Registry::getInstance().getComponents<Types::Rect>().insertBack(spriteRect);
#endif
        Registry::getInstance().getComponents<Types::CollisionRect>().insertBack(collisionRect1);
        Registry::getInstance().getComponents<Types::CollisionRect>().insertBack(collisionRect2);
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
