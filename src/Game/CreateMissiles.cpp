/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Bullets systems implementation
*/

#include "B-luga-physics/ECSCustomTypes.hpp"
#include "B-luga/Maths/Maths.hpp"
#include "MessageTypes.h"
#include "B-luga/Registry.hpp"
#include "ResourcesManager.hpp"
#include "CreateMissiles.hpp"
#include "B-luga/Json.hpp"
#ifdef CLIENT
    #include "B-luga-graphics/GraphicsCustomTypes.hpp"
    #include "NitworkClient.hpp"
    #include "B-luga-graphics/AnimRect.hpp"
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

#ifdef CLIENT
    static void playBulletSound(Types::Missiles &typeOfMissile)
    {
        Json &json = Json::getInstance();
        Registry::components<Raylib::SoundShared> arrSounds =
            Registry::getInstance().getComponents<Raylib::SoundShared>();
        nlohmann::json bulletData =
            json.getJsonObjectById(ResourcesManager::getPathByJsonType(JsonType::BULLETS), getMissileId(typeOfMissile.type), "bullets");

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
        Registry::getInstance().addEntity();
        const std::string bulletPath = json.getDataFromJson<std::string>(bulletData, "spritePath");
        Types::Rect spriteRect       = json.getDataFromJson<Types::Rect>(bulletData, "spriteRect");
        Types::SpriteDatas bulletDatas(
            bulletPath,
            collisionRect.width,
            collisionRect.height,
            FRONTLAYER,
            static_cast<std::size_t>(FRONT));
        Registry::getInstance().getComponents<Types::SpriteDatas>().insertBack(bulletDatas);
        Registry::getInstance().getComponents<Types::Rect>().insertBack(spriteRect);

        if (json.isDataExist(bulletData, "animRect")) {
            nlohmann::basic_json<> animRectData =
                Json::getInstance().getDataFromJson<nlohmann::basic_json<>>(bulletData, "animRect");
            Types::AnimRect animRect(spriteRect, animRectData, Types::RectListType::MOVE);
            Registry::getInstance().getComponents<Types::AnimRect>().insertBack(animRect);
        }
    }
#endif

    static void addPhysicsToBullet(nlohmann::json bulletData, Types::Position &position)
    {
        Json &json = Json::getInstance();
        if (!json.isDataExist(bulletData, "physics")) {
            return;
        }
        Types::Physics physicComp(position);
        std::vector<std::string> physics =
            json.getDataFromJson<std::vector<std::string>>(bulletData, "physics");
        for (const auto &it : physics) {
            physicComp.addPhysic(it);
        }
        if (physicComp.hasPhysics()) {
            Registry::getInstance().getComponents<Types::Physics>().insertBack(physicComp);
        }
    }

    void createMissile(Types::Position pos, Types::Missiles &typeOfMissile)
    {
        Json &json = Json::getInstance();
        Registry::getInstance().addEntity();
        nlohmann::json bulletData =
            json.getJsonObjectById(ResourcesManager::getPathByJsonType(JsonType::BULLETS), getMissileId(typeOfMissile.type), "bullets");
        Types::CollisionRect collisionRect =
            json.getDataFromJson<Types::CollisionRect>(bulletData, "collisionRect");
        Types::Velocity velocity    = json.getDataFromJson<Types::Velocity>(bulletData, "velocity");
        Types::Missiles missileType = typeOfMissile;
        Types::Dead deadComp        = {};
        Types::PlayerAllies playerAlliesComp = {};
        Types::Position position             = pos;
        struct health_s healthComp           = {json.getDataFromJson<int>(bulletData, "health")};
        Types::Damage damageComp             = {json.getDataFromJson<int>(bulletData, "damage")};

#ifdef CLIENT
        addSpriteRectsForBullet(bulletData, collisionRect);
        playBulletSound(typeOfMissile);
#endif
        addPhysicsToBullet(bulletData, position);
        Registry::getInstance().getComponents<Types::Position>().insertBack(position);
        Registry::getInstance().getComponents<Types::CollisionRect>().insertBack(collisionRect);
        Registry::getInstance().getComponents<Types::Missiles>().insertBack(missileType);
        Registry::getInstance().getComponents<Types::PlayerAllies>().insertBack(playerAlliesComp);
        Registry::getInstance().getComponents<Types::Velocity>().insertBack(velocity);
        Registry::getInstance().getComponents<struct health_s>().insertBack(healthComp);
        Registry::getInstance().getComponents<Types::Damage>().insertBack(damageComp);
        Registry::getInstance().getComponents<Types::Dead>().insertBack(deadComp);
    }

} // namespace Systems
