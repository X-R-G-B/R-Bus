/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Wave Systems implementation
*/

#include <fstream>
#include <sstream>
#ifdef CLIENT
    #include "B-luga-graphics/AnimRect.hpp"
    #include "B-luga-graphics/GraphicsCustomTypes.hpp"
#endif
#include "B-luga-physics/ECSCustomTypes.hpp"
#include "B-luga/Maths/Maths.hpp"
#include "B-luga/Registry.hpp"
#include "B-luga/SystemManagers/SystemManagersDirector.hpp"
#include "CreateMissiles.hpp"
#include "GameCustomTypes.hpp"
#include "PhysicSystems.hpp"
#include "ResourcesManager.hpp"
// #include "Systems.hpp"

#ifdef CLIENT
    // #include "CustomTypes.hpp"
    #include "B-luga-graphics/Raylib/Raylib.hpp"
    #include "NitworkClient.hpp"
#else
    #include "NitworkServer.hpp"
#endif

namespace Systems {

    static const std::unordered_map<enemy_type_e, std::string> enemiesTypes = {
        {enemy_type_e::CLASSIC_ENEMY,   "default"        },
        {enemy_type_e::CLASSIC_ENEMY_2, "default2"       },
        {enemy_type_e::BIG_DAEMON,      "big-daemon"     },
        {enemy_type_e::DAEMON,          "daemon"         },
        {enemy_type_e::PAPYRUS,         "papyrus"        },
        {enemy_type_e::NAPSTABLOOK,     "napstablook"    },
        {enemy_type_e::LEJUTTER,        "le-jutteur"     },
        {enemy_type_e::GREEN_HEAD,      "green-head"     },
        {enemy_type_e::BIG_PURPLE,      "big-purple-boss"},
    };

    static const std::string &getEnemyId(enemy_type_e enemyType)
    {
        if (enemiesTypes.find(enemyType) == enemiesTypes.end()) {
            Logger::error("Enemy type not found");
            throw std::runtime_error("Enemy type not found");
        }
        return enemiesTypes.at(enemyType);
    }

    static enemy_type_e getEnemyType(const std::string &enemyId)
    {
        for (const auto &pair : enemiesTypes) {
            if (pair.second == enemyId) {
                return pair.first;
            }
        }
        Logger::error("Enemy type not found");
        throw std::runtime_error("Enemy type not found");
    }

    nlohmann::json getEnemieData(enemy_type_e enemyType)
    {
        Json &json = Json::getInstance();

        const std::string enemyId = getEnemyId(enemyType);
        nlohmann::json data       = json.getJsonObjectById<std::string>(
            ResourcesManager::getPathByJsonType(JsonType::ENEMIES),
            enemyId,
            "enemies");
        return data;
    }

    //////////////////////////////// INIT ENEMIES ////////////////////////////////

    static void initEnemyWeapon(nlohmann::json jsonData, Types::EnemyAttack &attackData)
    {
        Json &json = Json::getInstance();
        if (!Json::isDataExist(jsonData, "attack")) {
            attackData.isAttacking = false;
            return;
        }
        if (Json::isDataExist(jsonData["attack"], "bulletId")) {
            std::string id         = json.getDataFromJson<std::string>(jsonData["attack"], "bulletId");
            attackData.missileType = getMissileTypeFromId(id);
        }
        if (Json::isDataExist(jsonData["attack"], "missileDirection")) {
            attackData.launchDirection =
                json.getDataFromJson<Types::Position>(jsonData["attack"], "missileDirection");
        }
        if (Json::isDataExist(jsonData["attack"], "numberOfMissiles")) {
            attackData.numberOfMissiles = json.getDataFromJson<int>(jsonData["attack"], "numberOfMissiles");
        }
        if (Json::isDataExist(jsonData["attack"], "msBetweenMissiles")) {
            attackData.msBetweenMissiles =
                json.getDataFromJson<float>(jsonData["attack"], "msBetweenMissiles");
        }
        if (Json::isDataExist(jsonData["attack"], "missileSpawnOffset")) {
            attackData.missileSpawnOffset =
                json.getDataFromJson<float>(jsonData["attack"], "missileSpawnOffset");
        }
        if (Json::isDataExist(jsonData["attack"], "bulletSpeed")) {
            attackData.bulletSpeed = json.getDataFromJson<float>(jsonData["attack"], "bulletSpeed");
        }
        if (Json::isDataExist(jsonData["attack"], "emitterId")) {
            attackData.emitterId = json.getDataFromJson<std::string>(jsonData["attack"], "emitterId");
        }
    }

    void
    initEnemy(enemy_type_e enemyType, Types::Position position, bool setId, struct ::enemy_id_s enemyId)
    {
        nlohmann::json enemyData = getEnemieData(enemyType);
        Registry::getInstance().addEntity();

#ifdef CLIENT
        Types::SpriteDatas enemy = {
            Json::getInstance().getDataFromJson<std::string>(enemyData, "spritePath"),
            Json::getInstance().getDataFromJson<int>(enemyData, "width"),
            Json::getInstance().getDataFromJson<int>(enemyData, "height"),
            LayerType::DEFAULTLAYER,
            0};

        auto rect = Json::getInstance().getDataFromJson<Types::Rect>(enemyData, "rect");

        nlohmann::basic_json<> animRectData =
            Json::getInstance().getDataFromJson<nlohmann::basic_json<>>(enemyData, "animRect");
        Types::AnimRect animRect(rect, animRectData, Types::RectListType::MOVE, Types::Direction::LEFT);

        constexpr float musicVolume = 0.80F;
        if (Json::isDataExist(enemyData, "musicPath")) {
            std::string musicPath =
                Json::getInstance().getDataFromJson<std::string>(enemyData, "musicPath");
            Raylib::MusicShared music = Raylib::Music::fromFile(musicPath, musicVolume);
            music->setNeedToPlay(true);
            Registry::getInstance().getComponents<Raylib::MusicShared>().insertBack(music);
        }

#endif
        Types::Enemy enemyComp = (setId ? Types::Enemy(enemyType, enemyId) : Types::Enemy(enemyType));
        Types::CollisionRect collisionRect =
            Json::getInstance().getDataFromJson<Types::CollisionRect>(enemyData, "collisionRect");
        Types::Damage damageComp = {Json::getInstance().getDataFromJson<int>(enemyData, "damage")};
        Types::Health healthComp = {Json::getInstance().getDataFromJson<int>(enemyData, "health")};
        Types::Velocity velocity =
            Json::getInstance().getDataFromJson<Types::Velocity>(enemyData, "velocity");

        addPhysicsToEntity(enemyData, position);
        initEnemyWeapon(enemyData, enemyComp.getAttack());

#ifdef CLIENT
        Registry::getInstance().getComponents<Types::Rect>().insertBack(rect);
        Registry::getInstance().getComponents<Types::AnimRect>().insertBack(animRect);
        Registry::getInstance().getComponents<Types::SpriteDatas>().insertBack(enemy);
#endif
        Registry::getInstance().getComponents<Types::Position>().insertBack(position);
        Registry::getInstance().getComponents<Types::CollisionRect>().insertBack(collisionRect);
        Registry::getInstance().getComponents<Types::Velocity>().insertBack(velocity);
        Registry::getInstance().getComponents<Types::Health>().insertBack(healthComp);
        Registry::getInstance().getComponents<Types::Damage>().insertBack(damageComp);
        Registry::getInstance().getComponents<Types::Enemy>().insertBack(enemyComp);
    }

    //////////////////////////////// BEGIN WAVES ////////////////////////////////

    void initWave(std::size_t managerId, std::size_t systemId)
    {
        Types::WaveInfos &waveInfos = Types::WaveInfos::getInstance();
        Registry &registry          = Registry::getInstance();
        std::lock_guard<std::mutex> lock(registry.mutex);

        nlohmann::json enemiesData = Json::getInstance().getJsonObjectById<std::size_t>(
            ResourcesManager::getPathByJsonType(JsonType::WAVE),
            waveInfos.getWaveId(),
            "waves");
        std::vector<nlohmann::json> enemies =
            Json::getInstance().getDataFromJson<std::vector<nlohmann::json>>(enemiesData, "enemies");
        for (const auto &enemy : enemies) {
            if (Json::isDataExist(enemy, "position") && Json::isDataExist(enemy, "id")) {
                std::size_t msBeforeSpawn = 0;
                if (Json::isDataExist(enemy, "msBeforeSpawn")) {
                    msBeforeSpawn =
                        Json::getInstance().getDataFromJson<std::size_t>(enemy, "msBeforeSpawn");
                }
                waveInfos.addEnemy(enemy, msBeforeSpawn);
            }
        }
        SystemManagersDirector::getInstance().getSystemManager(managerId).removeSystem(systemId);
    }

    void updateWave(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        Types::WaveInfos &waveInfos = Types::WaveInfos::getInstance();

        // Handle the end of the wave
        if (waveInfos.isFirstEnemyCreated() && !Types::Enemy::isEnemyAlive()
            && !waveInfos.isEnemyRemaining()) {
            waveInfos.prepareNextWave();
            return;
        }
        // If no enemy remaining, no need to continue
        if (!waveInfos.isEnemyRemaining()) {
            return;
        }
        // Handle the creation of the enemies
        const auto &enemy = waveInfos.getRemainingEnemies().front();
        if (!waveInfos.isFirstEnemyCreated()) {
            waveInfos.setFirstEnemyCreated(true);
        } else if (
            Registry::getInstance().getClock().elapsedMillisecondsSince(waveInfos.getClockId())
            < enemy.second) {
            return;
        }
        Registry::getInstance().getClock().restart(waveInfos.getClockId());
        std::string enemyId = Json::getInstance().getDataFromJson<std::string>(enemy.first, "id");
        Types::Position position =
            Json::getInstance().getDataFromJson<Types::Position>(enemy.first, "position");
        initEnemy(getEnemyType(enemyId), position, false, {0});
        waveInfos.removeFirstEnemy();
    }

    //////////////////////////////// END WAVES ////////////////////////////////

    std::vector<std::function<void(std::size_t, std::size_t)>> getWaveSystems()
    {
        return {updateWave};
    }

} // namespace Systems
