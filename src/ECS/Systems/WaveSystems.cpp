/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Wave Systems implementation
*/

#include <fstream>
#include <sstream>
#include "ECSCustomTypes.hpp"
#include "Maths.hpp"
#include "Registry.hpp"
#include "SystemManagersDirector.hpp"
#include "Systems.hpp"

#ifdef CLIENT
    #include "CustomTypes.hpp"
    #include "NitworkClient.hpp"
    #include "Raylib.hpp"
#else
    #include "NitworkServer.hpp"
#endif

namespace Systems {

    static const std::unordered_map<enemy_type_e, std::string> enemiesTypes = {
        {enemy_type_e::CLASSIC_ENEMY, "default"},
        {enemy_type_e::CLASSIC_ENEMY_2, "default2"},
        {enemy_type_e::DAEMON, "daemon"},
        {enemy_type_e::PAPYRUS, "papyrus"},
        {enemy_type_e::NAPSTABLOOK, "napstablook"},
        {enemy_type_e::LEJUTTER, "le-jutteur"},
        {enemy_type_e::GREEN_HEAD, "green-head"},
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
        nlohmann::json data = json.getJsonObjectById<std::string>(JsonType::ENEMIES, enemyId, "enemies");
        return data;
    }

    //////////////////////////////// INIT ENEMIES ////////////////////////////////

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
            0
        };

        auto rect = Json::getInstance().getDataFromJson<Types::Rect>(enemyData, "rect");

        nlohmann::basic_json<> animRectData =
            Json::getInstance().getDataFromJson<nlohmann::basic_json<>>(enemyData, "animRect");
        Types::AnimRect animRect(rect, animRectData, Types::RectListType::MOVE, Types::Direction::LEFT);

        constexpr float musicVolume = 0.80F;
        if (Json::isDataExist(enemyData, "musicPath")) {
            std::string musicPath = Json::getInstance().getDataFromJson<std::string>(enemyData, "musicPath");
            Raylib::Music music(musicPath, musicVolume);
            music.setNeedToPlay(true);
            Registry::getInstance().getComponents<Raylib::Music>().insertBack(music);
        }

#endif
        Types::Enemy enemyComp = (setId ? Types::Enemy(enemyType, enemyId) : Types::Enemy(enemyType));
        Types::CollisionRect collisionRect =
            Json::getInstance().getDataFromJson<Types::CollisionRect>(enemyData, "collisionRect");
        Types::Damage damageComp   = {Json::getInstance().getDataFromJson<int>(enemyData, "damage")};
        struct health_s healthComp = {Json::getInstance().getDataFromJson<int>(enemyData, "health")};
        Types::Velocity velocity =
            Json::getInstance().getDataFromJson<Types::Velocity>(enemyData, "velocity");

#ifdef CLIENT
        Registry::getInstance().getComponents<Types::Rect>().insertBack(rect);
        Registry::getInstance().getComponents<Types::AnimRect>().insertBack(animRect);
        Registry::getInstance().getComponents<Types::SpriteDatas>().insertBack(enemy);
#endif
        Registry::getInstance().getComponents<Types::Position>().insertBack(position);
        Registry::getInstance().getComponents<Types::CollisionRect>().insertBack(collisionRect);
        Registry::getInstance().getComponents<Types::Velocity>().insertBack(velocity);
        Registry::getInstance().getComponents<struct health_s>().insertBack(healthComp);
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
            JsonType::WAVE,
            waveInfos.getWaveId(),
            "waves");
        std::vector<nlohmann::json> enemies =
            Json::getInstance().getDataFromJson<std::vector<nlohmann::json>>(enemiesData, "enemies");
        for (const auto &enemy : enemies) {
            if (Json::isDataExist(enemy, "position") && Json::isDataExist(enemy, "id")) {
                std::size_t msBeforeSpawn = 0;
                if (Json::isDataExist(enemy, "msBeforeSpawn")) {
                    msBeforeSpawn = Json::getInstance().getDataFromJson<std::size_t>(enemy, "msBeforeSpawn");
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
        if (waveInfos.isFirstEnemyCreated() == true && Types::Enemy::isEnemyAlive() == false
            && waveInfos.isEnemyRemaining() == false) {
            waveInfos.prepareNextWave();
            return;
        }
        // If no enemy remaining, no need to continue
        if (waveInfos.isEnemyRemaining() == false) {
            return;
        }
        // Handle the creation of the enemies
        auto &enemy = waveInfos.getRemainingEnemies().front();
        if (waveInfos.isFirstEnemyCreated() == false)
        {
            waveInfos.setFirstEnemyCreated(true);
        } else if (Registry::getInstance().getClock().elapsedMillisecondsSince(waveInfos.getClockId()) < enemy.second) {
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
