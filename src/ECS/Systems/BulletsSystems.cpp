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

    static const std::map<missileTypes_e, std::string> missileTypeMap = {
        {CLASSIC,   "classic"  },
        {FAST,      "fast"     },
        {BOUNCE,    "bounce"   },
        {PERFORANT, "perforant"}
    };

    static const std::map<std::string, physicsType_e> physicsTypeMap = {
        {"bouncing", BOUNCING},
        {"zigzag",   ZIGZAG  }
    };

    static std::string getMissileId(missileTypes_e type)
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
        Registry::components<Raylib::Sound> arrSounds =
            Registry::getInstance().getComponents<Raylib::Sound>();
        nlohmann::json bulletData =
            json.getJsonObjectById(JsonType::BULLETS, getMissileId(typeOfMissile.type), "bullets");

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
            nlohmann::json animRectData = json.getDataFromJson<nlohmann::json>(bulletData, "animRect");
            Types::AnimRect animRect    = {
                spriteRect,
                json.getDataFromJson<std::vector<Types::Rect>>(animRectData, "move"),
                json.getDataFromJson<std::vector<Types::Rect>>(animRectData, "attack"),
                json.getDataFromJson<std::vector<Types::Rect>>(animRectData, "dead")};
            animRect.changeRectList(Types::RectListType::MOVE);
            Registry::getInstance().getComponents<Types::AnimRect>().insertBack(animRect);
        }
    }
#endif

    static void addPhysicsToBullet(nlohmann::json bulletData)
    {
        Json &json = Json::getInstance();
        if (!json.isDataExist(bulletData, "physics")) {
            return;
        }
        Types::Physics physicComp = {};
        std::vector<std::string> physics = json.getDataFromJson<std::vector<std::string>>(bulletData, "physics");
        for (const auto &it : physics) {
            auto foundedType = physicsTypeMap.find(it);
            if (foundedType != physicsTypeMap.end()) {
                physicComp.types.push_back(foundedType->second);
            } else {
                Logger::error("Unknown bullet physics type");
            }
        }
        if (physicComp.types.size() > 0) {
            Registry::getInstance().getComponents<Types::Physics>().insertBack(physicComp);
        }
    }

    void createMissile(Types::Position pos, Types::Missiles &typeOfMissile)
    {
        Json &json = Json::getInstance();
        Registry::getInstance().addEntity();
        nlohmann::json bulletData =
            json.getJsonObjectById(JsonType::BULLETS, getMissileId(typeOfMissile.type), "bullets");
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
        addSpriteRectsForBullet(bulletData, collisionRect);
        playBulletSound(typeOfMissile);
#endif
        addPhysicsToBullet(bulletData);
        Registry::getInstance().getComponents<Types::Position>().insertBack(position);
        Registry::getInstance().getComponents<Types::CollisionRect>().insertBack(collisionRect);
        Registry::getInstance().getComponents<Types::Missiles>().insertBack(missileType);
        Registry::getInstance().getComponents<Types::PlayerAllies>().insertBack(playerAlliesComp);
        Registry::getInstance().getComponents<Types::Velocity>().insertBack(velocity);
        Registry::getInstance().getComponents<struct health_s>().insertBack(healthComp);
        Registry::getInstance().getComponents<Types::Damage>().insertBack(damageComp);
        Registry::getInstance().getComponents<Types::Dead>().insertBack(deadComp);
    }

    static void updateBouncePhysics(std::vector<std::size_t> ids)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        Registry::components<Types::Velocity> velocities =
            Registry::getInstance().getComponents<Types::Velocity>();
        Registry::components<Types::CollisionRect> collisionRects =
            Registry::getInstance().getComponents<Types::CollisionRect>();
        Registry::components<Types::Position> positions =
            Registry::getInstance().getComponents<Types::Position>();

        for (std::size_t id : ids) {
            if (velocities.exist(id) && collisionRects.exist(id) && positions.exist(id)) {
                if (Maths::intToFloatConservingDecimals(positions[id].y) <= 0
                    || Maths::intToFloatConservingDecimals(positions[id].y)
                            + Maths::intToFloatConservingDecimals(collisionRects[id].height)
                        >= 100) {
                    velocities[id].speedY = -velocities[id].speedY;
                }
            }
        }
    }

    static void updateZigzagPhysics(std::vector<std::size_t> ids)
    {
        static constexpr float timeBetweenTwoUpdates = 300.F;
        static std::size_t clockId                   = Registry::getInstance().getClock().create(false);

        if (Registry::getInstance().getClock().elapsedMillisecondsSince(clockId) < timeBetweenTwoUpdates) {
            return;
        } else if (ids.size() > 0) {
            Registry::getInstance().getClock().restart(clockId);
        }
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        Registry::components<Types::Velocity> velocities =
            Registry::getInstance().getComponents<Types::Velocity>();

        for (std::size_t id : ids) {
            if (velocities.exist(id)) {
                if (velocities[id].speedY == 0) {
                    velocities[id].speedY = 100;
                }
                velocities[id].speedY = -velocities[id].speedY;
            }
        }
    }

    void updatePhysics(std::size_t, std::size_t)
    {
        std::vector<std::size_t> bouncingId;
        std::vector<std::size_t> zigzagId;
        Registry::components<Types::Physics> physicComps =
            Registry::getInstance().getComponents<Types::Physics>();
        std::vector<std::size_t> ids =
            Registry::getInstance().getEntitiesByComponents({typeid(Types::Physics)});

        for (std::size_t id : ids) {
            for (auto &type : physicComps[id].types) {
                if (type == BOUNCING) {
                    bouncingId.push_back(id);
                } else if (type == ZIGZAG) {
                    zigzagId.push_back(id);
                }
            }
        }
        updateBouncePhysics(bouncingId);
        updateZigzagPhysics(zigzagId);
    }

    std::vector<std::function<void(std::size_t, std::size_t)>> getBulletSystems()
    {
        return {updatePhysics};
    }

} // namespace Systems
