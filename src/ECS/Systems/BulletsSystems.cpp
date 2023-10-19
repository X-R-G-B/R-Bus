/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Bullets systems implementation
*/

#include <fstream>
#include "Registry.hpp"
#include "Systems.hpp"

#ifdef CLIENT
    #include "CustomTypes.hpp"
    #include "NitworkClient.hpp"
#else
    #include "NitworkServer.hpp"
#endif

namespace Systems {

    void createMissile(Types::Position &pos, Types::Missiles &typeOfMissile)
    {
        Json &json = Json::getInstance();
        Registry::getInstance().addEntity();
        nlohmann::json bulletData = json.getJsonObjectById(JsonType::BULLETS, "default");

        Types::CollisionRect collisionRect = json.getDataFromJson<Types::CollisionRect>(bulletData, "collisionRect");
        Types::Velocity velocity           = json.getDataFromJson<Types::Velocity>(bulletData, "velocity");
        Types::Missiles missileType          = typeOfMissile;
        Types::Dead deadComp                 = {};
        Types::PlayerAllies playerAlliesComp = {};
        Types::Position position             = {pos.x, pos.y};
        struct health_s healthComp = {1};
        Types::Damage damageComp   = {10};

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

} // namespace Systems
