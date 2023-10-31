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
        {enemy_type_e::TERMINATOR, "terminator"},
    };

    static const std::string &getEnemyId(enemy_type_e enemyType)
    {
        if (enemiesTypes.find(enemyType) == enemiesTypes.end()) {
            Logger::error("Enemy type not found");
            throw std::runtime_error("Enemy type not found");
        }
        return enemiesTypes.at(enemyType);
    }

    nlohmann::json getEnemieData(enemy_type_e enemyType)
    {
        Json &json = Json::getInstance();

        const std::string enemyId = getEnemyId(enemyType);
        nlohmann::json data = json.getJsonObjectById(JsonType::ENEMIES, enemyId, "enemies");
        return data;
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
            0
        };

        auto rect = Json::getInstance().getDataFromJson<Types::Rect>(enemyData, "rect");

        nlohmann::basic_json<> animRectData =
            Json::getInstance().getDataFromJson<nlohmann::basic_json<>>(enemyData, "animRect");
        Types::AnimRect animRect(rect, animRectData, Types::RectListType::MOVE, Types::Direction::LEFT);

#endif
        Types::Enemy enemyComp = (setId ? Types::Enemy {enemyId} : Types::Enemy {});
        Types::CollisionRect collisionRect =
            Json::getInstance().getDataFromJson<Types::CollisionRect>(enemyData, "collisionRect");
        Types::Damage damageComp   = {Json::getInstance().getDataFromJson<int>(enemyData, "damage")};
        struct health_s healthComp = {Json::getInstance().getDataFromJson<int>(enemyData, "health")};
        Types::Velocity velocity =
            Json::getInstance().getDataFromJson<Types::Velocity>(enemyData, "velocity");

        if (position.x == 0 && position.y == 0) {
            Types::Position tmpPos(
                Json::getInstance().getDataFromJson<Types::Position>(enemyData, "position"));
            position = tmpPos;
        }
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

} // namespace Systems
