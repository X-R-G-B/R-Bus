
#include "GameSystems.hpp"
#include "Json.hpp"
#include "Maths.hpp"
#include "SystemManagersDirector.hpp"
#include "GameCustomTypes.hpp"
#ifdef CLIENT
    #include "GraphicsCustomTypes.hpp"
    #include "NitworkClient.hpp"
#else
    #include "NitworkServer.hpp"
#endif

namespace Systems {
    void
    initEnemy(enemy_type_e enemyType, Types::Position position, bool setId, struct ::enemy_id_s enemyId)
    {
        JsonType jsonType = messageTypes.at(enemyType);
        std::vector<nlohmann::basic_json<>> enemyData =
                Json::getInstance().getDataByJsonType("enemy", jsonType);

        for (auto &elem : enemyData) {
            Registry::getInstance().addEntity();

    #ifdef CLIENT
            Types::SpriteDatas enemy = {
                Json::getInstance().getDataFromJson<std::string>(elem, "spritePath"),
                Json::getInstance().getDataFromJson<int>(elem, "width"),
                Json::getInstance().getDataFromJson<int>(elem, "height"),
                LayerType::DEFAULTLAYER,
                0
            };

            auto rect = Json::getInstance().getDataFromJson<Types::Rect>(elem, "rect");

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

    static constexpr float maxPercent = 100.0F;

    void manageBoss(std::size_t managerId, std::size_t systemId)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        const float posToGo   = 65.0;
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
            if (Maths::intToFloatConservingDecimals(arrPosition[id].x) <= posToGo
                && Maths::intToFloatConservingDecimals(arrVelocity[id].speedY) == 0) {
                arrVelocity[id].speedX = 0;
                arrVelocity[id].speedY = Maths::floatToIntConservingDecimals(bossSpeed);
            }
            if (arrPosition[id].y < 0) {
                arrVelocity[id].speedY = Maths::floatToIntConservingDecimals(bossSpeed);
            }
            if (Maths::intToFloatConservingDecimals(arrPosition[id].y)
                + Maths::intToFloatConservingDecimals(arrCollisonRect[id].height)
                > maxPercent) {
                arrVelocity[id].speedY = Maths::floatToIntConservingDecimals(-bossSpeed);
            }
        }
    }

    void initWave(std::size_t managerId, std::size_t systemId)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        static std::size_t enemyNumber =
                Json::getInstance().getDataByVector({"wave", "nbrEnemy"}, JsonType::WAVE);
        const std::size_t spawnDelay = 2;
        Clock &clock                 = Registry::getInstance().getClock();
        static std::size_t clockId   = clock.create(true);
        static bool fstCall          = true;
        std::vector<nlohmann::json> jsonVector =
                Json::getInstance().getDataByVector({"wave", "positions"}, JsonType::WAVE);
        nlohmann::json jsonPos;
        Registry::components<Types::Boss> &bossArr = Registry::getInstance().getComponents<Types::Boss>();
        Registry::components<Types::Enemy> &enemyArr =
                Registry::getInstance().getComponents<Types::Enemy>();

        if (enemyNumber > 0) {
            jsonPos = Json::getInstance().getDataFromJson<Types::Position>(
                    jsonVector[enemyNumber - 1],
                    "position");
        } else {
            jsonPos = Json::getInstance().getDataFromJson<Types::Position>(jsonVector[0], "position");
        }
        Types::Position pos(jsonPos);
        if (fstCall) {
            fstCall = false;
            clock.restart(clockId);
        }
        if (clock.elapsedSecondsSince(clockId) >= spawnDelay && enemyNumber > 0) {
            initEnemy(CLASSIC_ENEMY, pos);
            enemyNumber--;
            clock.decreaseSeconds(clockId, spawnDelay);
        }
        if (enemyArr.getExistingsId().empty() && enemyNumber <= 0 && bossArr.getExistingsId().empty()) {
            initEnemy(TERMINATOR, pos);
            SystemManagersDirector::getInstance().getSystemManager(managerId).addSystem(manageBoss);
            SystemManagersDirector::getInstance().getSystemManager(managerId).removeSystem(systemId);
        }
    }

    void initPlayer(unsigned int constId, bool otherPlayer)
    {
        JsonType playerType = JsonType::DEFAULT_PLAYER;

        Registry::getInstance().addEntity();

        Logger::info("player avant template");
        Types::Dead deadComp = {
                Json::getInstance().getDataByVector<std::size_t>({"player", "deadTime"}, playerType)};
        struct health_s healthComp = {
                Json::getInstance().getDataByVector<int>({"player", "health"}, playerType)};
        Logger::info("player after template");
        Types::Damage damageComp = {Json::getInstance().getDataByVector({"player", "damage"}, playerType)};
        Types::Container container = {Json::getInstance().getDataByVector({"player", "container"}, playerType)};
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
            Types::PlayerAllies allie;
            Registry::getInstance().getComponents<Types::PlayerAllies>().insertBack(allie);
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
        Registry::getInstance().addEntity();

        constexpr float bulletWidth        = 5.0F;
        constexpr float bulletHeight       = 5.0F;
        constexpr float speedX             = 0.7F;
        constexpr float speedY             = 0.0F;
        Types::CollisionRect collisionRect = {
                Maths::floatToIntConservingDecimals(bulletWidth),
                Maths::floatToIntConservingDecimals(bulletHeight)};
        Types::Velocity velocity = {
                Maths::floatToIntConservingDecimals(speedX),
                Maths::floatToIntConservingDecimals(speedY)};
        Types::Missiles missileType          = typeOfMissile;
        Types::Dead deadComp                 = {};
        Types::PlayerAllies playerAlliesComp = {};
        Types::Position position             = {pos.x, pos.y};

    #ifdef CLIENT
        const std::string bulletPath = "assets/R-TypeSheet/r-typesheet1.gif";
            Types::Rect spriteRect       = {200, 121, 32, 10};
            Types::SpriteDatas bulletDatas(
                bulletPath,
                Maths::floatToIntConservingDecimals(bulletWidth),
                Maths::floatToIntConservingDecimals(bulletHeight),
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

    void addAllies(std::size_t managerId, std::size_t sysId)
    {
        auto &registry = Registry::getInstance();
        registry.addAllie(static_cast<std::size_t>(AlliesType::PLAYERS), typeid(Types::Player));
        registry.addAllie(static_cast<std::size_t>(AlliesType::PLAYERS), typeid(Types::OtherPlayer));
        registry.addAllie(static_cast<std::size_t>(AlliesType::PLAYERS), typeid(Types::PlayerAllies));
        registry.addAllie(static_cast<std::size_t>(AlliesType::ENEMIES), typeid(Types::Enemy));
        registry.addAllie(static_cast<std::size_t>(AlliesType::ENEMIES), typeid(Types::EnemyAllies));
        SystemManagersDirector::getInstance().getSystemManager(managerId).removeSystem(sysId);
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
#endif

    void setupEventsCallback(std::size_t managerId, std::size_t sysId)
    {
        Registry::getInstance().addEventCallback(Events::REMOVE_ENTITY, sendEnemyDeath);
#ifdef CLIENT
        Registry::getInstance().addEventCallback(Events::TAKE_DAMAGE, sendLifeUpdateToServer);
#endif
        SystemManagersDirector::getInstance().getSystemManager(managerId).removeSystem(sysId);
    }

    std::vector<std::function<void(std::size_t, std::size_t)>> Systems::getGameSystems()
    {
        return {
            setupEventsCallback,
            addAllies
        };
    }
}
