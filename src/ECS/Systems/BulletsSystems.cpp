/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Bullets systems implementation
*/

#include <fstream>
#include "Logger.hpp"
#include "Maths.hpp"
#include "MessageTypes.h"
#include "Registry.hpp"
#include "Systems.hpp"

#ifdef CLIENT
    #include "CustomTypes.hpp"
    #include "NitworkClient.hpp"
    #include "Raylib.hpp"
#else
    #include "NitworkServer.hpp"
#endif

namespace Systems {

    static std::string getMissileId(missileTypes_e type)
    {
        switch (type) {
            case CLASSIC: return "classic";
            case FAST: return "fast";
            case BOUNCE: return "bounce";
            case PERFORANT: return "perforant";
            default: break;
        }
        throw std::runtime_error("Unknown missile type");
    }

#ifdef CLIENT
    static void playBulletSound(Types::Missiles &typeOfMissile)
    {
        Json &json = Json::getInstance();
        Registry::components<Raylib::Sound> arrSounds =
            Registry::getInstance().getComponents<Raylib::Sound>();
        nlohmann::json bulletData =
            json.getJsonObjectById(JsonType::BULLETS, getMissileId(typeOfMissile.type));

        const std::string soundPathShoot = json.getDataFromJson<std::string>(bulletData, "soundPath");

        for (auto &sound : arrSounds) {
            if (sound.getPath() == soundPathShoot) {
                sound.setNeedToPlay(true);
                break;
            }
        }
    }
#endif

    static Types::Position adjustMissilePosition(Types::Position &pos, Types::CollisionRect &collisionRect)
    {
        Types::Position newPos = pos;
        int halfSprite         = Maths::divisionWithTwoIntDecimals(collisionRect.width, 200);
        newPos.y               = Maths::subtractionWithTwoIntDecimals(newPos.y, halfSprite);
        newPos.x               = pos.x;
        return newPos;
    }

    void createMissile(Types::Position pos, Types::Missiles &typeOfMissile)
    {
        Json &json = Json::getInstance();
        Registry::getInstance().addEntity();
        nlohmann::json bulletData =
            json.getJsonObjectById(JsonType::BULLETS, getMissileId(typeOfMissile.type));
        Types::CollisionRect collisionRect =
            json.getDataFromJson<Types::CollisionRect>(bulletData, "collisionRect");
        Types::Velocity velocity    = json.getDataFromJson<Types::Velocity>(bulletData, "velocity");
        Types::Missiles missileType = typeOfMissile;
        Types::Dead deadComp        = {};
        Types::PlayerAllies playerAlliesComp = {};
        Types::Position position             = adjustMissilePosition(pos, collisionRect);
        struct health_s healthComp           = {json.getDataFromJson<int>(bulletData, "health")};
        Types::Damage damageComp             = {json.getDataFromJson<int>(bulletData, "damage")};

#ifdef CLIENT
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
            nlohmann::json animRectData = json.getDataFromJson<nlohmann::json>(bulletData, "animRect");
            Types::AnimRect animRect    = {
                spriteRect,
                json.getDataFromJson<std::vector<Types::Rect>>(animRectData, "move"),
                json.getDataFromJson<std::vector<Types::Rect>>(animRectData, "attack"),
                json.getDataFromJson<std::vector<Types::Rect>>(animRectData, "dead")};
            animRect.changeRectList(Types::RectListType::MOVE);
            Registry::getInstance().getComponents<Types::AnimRect>().insertBack(animRect);
        }
        playBulletSound(typeOfMissile);

#endif

        Registry::getInstance().getComponents<Types::Position>().insertBack(position);
        Registry::getInstance().getComponents<Types::CollisionRect>().insertBack(collisionRect);
        Registry::getInstance().getComponents<Types::Missiles>().insertBack(missileType);
        Registry::getInstance().getComponents<Types::PlayerAllies>().insertBack(playerAlliesComp);
        Registry::getInstance().getComponents<Types::Velocity>().insertBack(velocity);
        Registry::getInstance().getComponents<struct health_s>().insertBack(healthComp);
        Registry::getInstance().getComponents<Types::Damage>().insertBack(damageComp);
        Registry::getInstance().getComponents<Types::Dead>().insertBack(deadComp);
    }

    void updateSpecialBullets(std::size_t, std::size_t)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        Registry::components<Types::Missiles> missiles =
            Registry::getInstance().getComponents<Types::Missiles>();
        Registry::components<Types::Velocity> velocities =
            Registry::getInstance().getComponents<Types::Velocity>();
        Registry::components<Types::CollisionRect> collisionRects =
            Registry::getInstance().getComponents<Types::CollisionRect>();
        Registry::components<Types::Position> positions =
            Registry::getInstance().getComponents<Types::Position>();
        std::vector<std::size_t> ids = Registry::getInstance().getEntitiesByComponents(
            {typeid(Types::Position),
             typeid(Types::Missiles),
             typeid(Types::Velocity),
             typeid(Types::CollisionRect)});

        for (std::size_t id : ids) {
            if (missiles[id].type == BOUNCE) {
                if (Maths::intToFloatConservingDecimals(positions[id].y) <= 0
                    || Maths::intToFloatConservingDecimals(positions[id].y)
                            + Maths::intToFloatConservingDecimals(collisionRects[id].height)
                        >= 100) {
                    velocities[id].speedY = -velocities[id].speedY;
                }
            }
        }
    }

    std::vector<std::function<void(std::size_t, std::size_t)>> getBulletSystems()
    {
        return {updateSpecialBullets};
    }

} // namespace Systems
