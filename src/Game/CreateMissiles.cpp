/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Bullets systems implementation
*/

#include "CreateMissiles.hpp"
#include "B-luga-physics/ECSCustomTypes.hpp"
#include "B-luga/Json.hpp"
#include "B-luga/Maths/Maths.hpp"
#include "B-luga/Registry.hpp"
#include "MessageTypes.h"
#include "PhysicSystems.hpp"
#include "ResourcesManager.hpp"
#ifdef CLIENT
    #include "B-luga-graphics/Raylib/Raylib.hpp"
    #include "B-luga-graphics/AnimRect.hpp"
    #include "B-luga-graphics/GraphicsCustomTypes.hpp"
    #include "NitworkClient.hpp"
#endif

namespace Systems {

    static const std::map<missileTypes_e, std::string> missileTypeMap = {
        {CLASSIC,   "classic"  },
        {FAST,      "fast"     },
        {BOUNCE,    "bounce"   },
        {PERFORANT, "perforant"}
    };

    std::string getMissileId(missileTypes_e type)
    {
        auto it = missileTypeMap.find(type);
        if (it != missileTypeMap.end()) {
            return it->second;
        }
        throw std::runtime_error("Unknown missile type");
    }

    missileTypes_e getMissileTypeFromId(const std::string &id)
    {
        for (const auto &it : missileTypeMap) {
            if (it.second == id) {
                return it.first;
            }
        }
        throw std::runtime_error("Unknown missile id");
    }

    std::string getMissileIdFromType(missileTypes_e type)
    {
        auto it = missileTypeMap.find(type);
        if (it != missileTypeMap.end()) {
            return it->second;
        }
        throw std::runtime_error("Unknown missile type");
    }

#ifdef CLIENT
    static void playBulletSound(Types::Missiles &typeOfMissile)
    {
        Json &json = Json::getInstance();
        Registry::components<Raylib::SoundShared> arrSounds =
            Registry::getInstance().getComponents<Raylib::SoundShared>();
        nlohmann::json bulletData =
            json.getJsonObjectById(ResourcesManager::getPathByJsonType(JsonType::BULLETS), getMissileIdFromType(typeOfMissile.type), "bullets");

        const std::string soundPathShoot = json.getDataFromJson<std::string>(bulletData, "soundPath");

        for (auto &sound : arrSounds) {
            if (sound->getPath() == soundPathShoot) {
                sound->setNeedToPlay(true);
                break;
            }
        }
    }
#endif

#ifdef CLIENT
    static void addSpriteRectsForBullet(nlohmann::json &bulletData, Types::CollisionRect &collisionRect)
    {
        Json &json = Json::getInstance();
        const std::string bulletPath = json.getDataFromJson<std::string>(bulletData, "spritePath");
        Types::Rect spriteRect       = json.getDataFromJson<Types::Rect>(bulletData, "spriteRect");
        Types::SpriteDatas
            bulletDatas(bulletPath, collisionRect.width, collisionRect.height, FRONTLAYER, 0);
        Registry::getInstance().getComponents<Types::SpriteDatas>().insertBack(bulletDatas);
        Registry::getInstance().getComponents<Types::Rect>().insertBack(spriteRect);

        if (Json::isDataExist(bulletData, "animRect")) {
            nlohmann::basic_json<> animRectData =
                Json::getInstance().getDataFromJson<nlohmann::basic_json<>>(bulletData, "animRect");
            Types::AnimRect animRect(spriteRect, animRectData, Types::RectListType::MOVE);
            Registry::getInstance().getComponents<Types::AnimRect>().insertBack(animRect);
        }
    }
#endif

    std::size_t createPlayerMissile(Types::Position pos, Types::Missiles &typeOfMissile)
    {
        if (typeOfMissile.type >= MAX_MISSILE_TYPE || typeOfMissile.type < 0) {
            throw std::runtime_error("Unknown missile type");
        }
        Json &json = Json::getInstance();
        std::size_t id = Registry::getInstance().addEntity();
        nlohmann::json bulletData =
            json.getJsonObjectById<std::string>(ResourcesManager::getPathByJsonType(JsonType::BULLETS), getMissileIdFromType(typeOfMissile.type), "bullets");
        Types::CollisionRect collisionRect =
            json.getDataFromJson<Types::CollisionRect>(bulletData, "collisionRect");
        Types::Velocity velocity    = json.getDataFromJson<Types::Velocity>(bulletData, "velocity");
        Types::Missiles missileType = typeOfMissile;
        Types::Dead deadComp        = {};
        Types::PlayerAllies playerAlliesComp = {};
        Types::Health healthComp           = {json.getDataFromJson<int>(bulletData, "health")};
        Types::Damage damageComp             = {json.getDataFromJson<int>(bulletData, "damage")};

#ifdef CLIENT
        addSpriteRectsForBullet(bulletData, collisionRect);
        playBulletSound(typeOfMissile);
#endif

        Registry::getInstance().getComponents<Types::PlayerAllies>().insertBack(playerAlliesComp);
        addPhysicsToEntity(bulletData, pos);
        Registry::getInstance().getComponents<Types::Position>().insertBack(pos);
        Registry::getInstance().getComponents<Types::CollisionRect>().insertBack(collisionRect);
        Registry::getInstance().getComponents<Types::Missiles>().insertBack(missileType);
        Registry::getInstance().getComponents<Types::Velocity>().insertBack(velocity);
        Registry::getInstance().getComponents<Types::Health>().insertBack(healthComp);
        Registry::getInstance().getComponents<Types::Damage>().insertBack(damageComp);
        Registry::getInstance().getComponents<Types::Dead>().insertBack(deadComp);
        return id;
    }

    void
    createEnemyMissile(Types::Position position, Types::Missiles &typeOfMissile, Types::Velocity velocity)
    {
        Json &json = Json::getInstance();
        Registry::getInstance().addEntity();
        nlohmann::json bulletData =
            json.getJsonObjectById(ResourcesManager::getPathByJsonType(JsonType::BULLETS), getMissileIdFromType(typeOfMissile.type), "bullets");
        Types::CollisionRect collisionRect =
            json.getDataFromJson<Types::CollisionRect>(bulletData, "collisionRect");
        Types::Missiles missileType        = typeOfMissile;
        Types::Dead deadComp               = {};
        Types::EnemyAllies enemyAlliesComp = {};
        Types::Health healthComp         = {json.getDataFromJson<int>(bulletData, "health")};
        Types::Damage damageComp           = {json.getDataFromJson<int>(bulletData, "damage")};

#ifdef CLIENT
        addSpriteRectsForBullet(bulletData, collisionRect);
        playBulletSound(typeOfMissile);
#endif

        Registry::getInstance().getComponents<Types::EnemyAllies>().insertBack(enemyAlliesComp);
        addPhysicsToEntity(bulletData, position);
        Registry::getInstance().getComponents<Types::Position>().insertBack(position);
        Registry::getInstance().getComponents<Types::CollisionRect>().insertBack(collisionRect);
        Registry::getInstance().getComponents<Types::Missiles>().insertBack(missileType);
        Registry::getInstance().getComponents<Types::Velocity>().insertBack(velocity);
        Registry::getInstance().getComponents<Types::Health>().insertBack(healthComp);
        Registry::getInstance().getComponents<Types::Damage>().insertBack(damageComp);
        Registry::getInstance().getComponents<Types::Dead>().insertBack(deadComp);
    }

    static void launchMissileInLine(Types::Enemy &enemy, Types::Position &emitterPosition, std::size_t id)
    {
        Types::Missiles missileType = {enemy.getAttack().missileType};
        Registry::components<Types::CollisionRect> arrCollision =
            Registry::getInstance().getComponents<Types::CollisionRect>();
        if (arrCollision.exist(id)) {
            emitterPosition.x += Maths::divisionWithTwoIntDecimals(arrCollision[id].width, 200);
            emitterPosition.y += Maths::divisionWithTwoIntDecimals(arrCollision[id].height, 200);
        }

        Types::Velocity velocity = {
            Maths::floatToIntConservingDecimals(
                enemy.getAttack().bulletSpeed * enemy.getAttack().launchDirection.x),
            Maths::floatToIntConservingDecimals(
                enemy.getAttack().bulletSpeed * enemy.getAttack().launchDirection.y)};

        float totalHeight = enemy.getAttack().numberOfMissiles * enemy.getAttack().missileSpawnOffset;
        float firstPos    = Maths::intToFloatConservingDecimals(emitterPosition.y) - totalHeight / 2;
        for (std::size_t i = 0; i < enemy.getAttack().numberOfMissiles; i++) {
            Types::Position missilePos = emitterPosition;
            missilePos.y =
                Maths::floatToIntConservingDecimals(firstPos + i * enemy.getAttack().missileSpawnOffset);
            createEnemyMissile(missilePos, missileType, velocity);
        }
    }

    static void launchMissileInCircle(Types::Enemy &enemy, Types::Position &emitterPosition, std::size_t id)
    {
        Types::Missiles missileType = {enemy.getAttack().missileType};
        Registry::components<Types::CollisionRect> arrCollision =
            Registry::getInstance().getComponents<Types::CollisionRect>();
        if (arrCollision.exist(id)) {
            emitterPosition.x += Maths::divisionWithTwoIntDecimals(arrCollision[id].width, 200);
            emitterPosition.y += Maths::divisionWithTwoIntDecimals(arrCollision[id].height, 200);
        }

        Types::Velocity velocity = {
            enemy.getAttack().launchDirection.x,
            enemy.getAttack().launchDirection.y};

        float angle = Maths::getAngleFromVector(
            Maths::intToFloatConservingDecimals(velocity.speedX),
            Maths::intToFloatConservingDecimals(velocity.speedY));
        float angleOffset = static_cast<float>(360 / enemy.getAttack().numberOfMissiles);

        for (std::size_t i = 0; i < enemy.getAttack().numberOfMissiles; i++) {
            Types::Position missilePos = emitterPosition;
            Types::Velocity velocityy  = {
                Maths::floatToIntConservingDecimals(
                    enemy.getAttack().bulletSpeed * cos(Maths::degreesToRadians(angle))),
                Maths::floatToIntConservingDecimals(
                    enemy.getAttack().bulletSpeed * sin(Maths::degreesToRadians(angle)))};
            createEnemyMissile(missilePos, missileType, velocityy);
            angle += angleOffset;
        }
    }

    static void launchEnemyMissile(Types::Enemy &enemy, Types::Position &pos, std::size_t id)
    {
        if (enemy.getAttack().emitterId == "circle") {
            launchMissileInCircle(enemy, pos, id);
        }
        if (enemy.getAttack().emitterId == "line") {
            launchMissileInLine(enemy, pos, id);
        }
    }

    void updateEnemiesAttacks(std::size_t /* unused */, std::size_t /* unused */)
    {
        Registry::components<Types::Enemy> arrEnemies =
            Registry::getInstance().getComponents<Types::Enemy>();
        Registry::components<Types::Position> arrPositions =
            Registry::getInstance().getComponents<Types::Position>();
        auto ids = Registry::getInstance().getEntitiesByComponents(
            {typeid(Types::Position), typeid(Types::Enemy)});

        for (auto &id : ids) {
            auto &attack = arrEnemies[id].getAttack();
            if (attack.isAttacking) {
                if (Registry::getInstance().getClock().elapsedMillisecondsSince(attack.clockId)
                    >= static_cast<std::size_t>(attack.msBetweenMissiles)) {
                    launchEnemyMissile(arrEnemies[id], arrPositions[id], id);
                    Registry::getInstance().getClock().restart(attack.clockId);
                }
            }
        }
    }

    std::vector<std::function<void(std::size_t, std::size_t)>> getBulletsSystems()
    {
        return {updateEnemiesAttacks};
    }
} // namespace Systems
