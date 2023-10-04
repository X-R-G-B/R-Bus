/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Systems implementation
*/

#include "Systems.hpp"
#include <cstddef>
#include "CustomTypes.hpp"
#include "Raylib.hpp"
#include "Registry.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include "SystemManagersDirector.hpp"

namespace Systems {

    void windowCollision(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        Registry &registry = Registry::getInstance();
        Registry::components<Types::Position> arrPosition =
            registry.getComponents<Types::Position>();
        Registry::components<Types::CollisionRect> arrCollisionRect =
            registry.getComponents<Types::CollisionRect>();
        std::vector<std::size_t> ids = registry.getEntitiesByComponents(
            {typeid(Types::Player),
             typeid(Types::Position),
             typeid(Types::CollisionRect)});

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

    static void giveDamages(std::size_t firstEntity, std::size_t secondEntity)
    {
        Registry::components<Types::Damage> arrDamage =
            Registry::getInstance().getComponents<Types::Damage>();
        Registry::components<Types::Health> arrHealth =
            Registry::getInstance().getComponents<Types::Health>();

        if (arrDamage.exist(firstEntity) && arrDamage[firstEntity].damage > 0) {
            if (arrHealth.exist(secondEntity)) {
                arrHealth[secondEntity].hp -= arrDamage[firstEntity].damage;
            }
        }
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
                    giveDamages(id, *itIds);
                    giveDamages(*itIds, id);
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
        std::cout << "Could not load the json data : An error occured." << std::endl;
        return jsonData;
    }

    static void initParallaxEntity(nlohmann::json_abi_v3_11_2::basic_json<> &parallaxData)
    {
        std::size_t id = Registry::getInstance().addEntity();

        Registry::getInstance().getComponents<Raylib::Sprite>().insertBack(
            {parallaxData["spritePath"], parallaxData["width"], parallaxData["height"], id}
        );
        Registry::getInstance().getComponents<Types::Position>().insertBack(
            {Types::Position(parallaxData["position"])}
        );
        Registry::getInstance().getComponents<Types::Parallax>().insertBack(
            {}
        );
        Registry::getInstance().setToBackLayers(id);
    }

    void initParalax(std::size_t managerId, std::size_t systemId)
    {
        nlohmann::json jsonData = openJsonData("assets/Json/parallaxData.json");

        for (auto &e : jsonData["parallax"]) {
            initParallaxEntity(e);
        }

        SystemManagersDirector::getInstance()
            .getSystemManager(managerId)
            .removeSystem(systemId);
    }

    void entitiesCollision(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        Registry &registry = Registry::getInstance();
        Registry::components<Types::Position> arrPosition =
            registry.getComponents<Types::Position>();
        Registry::components<Types::CollisionRect> arrCollisionRect =
            registry.getComponents<Types::CollisionRect>();
        std::vector<std::size_t> ids = registry.getEntitiesByComponents(
            {typeid(Types::CollisionRect), typeid(Types::Position)});


        for (auto itIds = ids.begin(); itIds != ids.end(); itIds++) {
            checkCollisionEntity(itIds, ids, arrPosition, arrCollisionRect);
        }
    }

#ifndef NDEBUG
    void debugCollisionRect(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        Registry::components<Types::CollisionRect> arrCollisionRect =
            Registry::getInstance().getComponents<Types::CollisionRect>();
        Registry::components<Types::Position> arrPosition =
            Registry::getInstance().getComponents<Types::Position>();
        Registry::components<Types::RectangleShape> arrRectangleShape =
            Registry::getInstance().getComponents<Types::RectangleShape>();

        std::vector<std::size_t> ids = arrCollisionRect.getExistingsId();

        for (auto &id : ids) {
            if (arrPosition.exist(id) && !arrRectangleShape.exist(id)) {
                Registry::getInstance()
                    .getComponents<Types::RectangleShape>()
                    .insert(
                        id,
                        {arrCollisionRect[id].width,
                         arrCollisionRect[id].height});
            }
        }
    }
#endif

    void moveEntities(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        Registry::components<Types::Position> arrPosition =
            Registry::getInstance().getComponents<Types::Position>();
        Registry::components<Types::Velocity> arrVelocity =
            Registry::getInstance().getComponents<Types::Velocity>();

        std::vector<std::size_t> ids = arrPosition.getExistingsId();

        for (auto &id : ids) {
            if (arrVelocity.exist(id)) {
                arrPosition[id].x += arrVelocity[id].speedX;
                arrPosition[id].y += arrVelocity[id].speedY;
            }
        }
    }

    static void executeDeathFunction(
        std::size_t id,
        Registry::components<Types::Dead> arrDead)
    {
        if (arrDead[id].deathFunction != std::nullopt) {
            arrDead[id].deathFunction.value()(id);
        } else {
            Registry::getInstance().removeEntity(id);
        }
    }

    static void checkResetParralax(std::size_t id, Registry::components<Types::Position> &arrPos)
    {
        if (arrPos[id].x >= 100) {
            arrPos[id].x = -100;
        } else {
            arrPos[id].x += 1;
        }
    }

    void manageParallax(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        Registry::components<Types::Position> arrPosition =
            Registry::getInstance().getComponents<Types::Position>();
        Registry::components<Types::Parallax> arrParralax = 
            Registry::getInstance().getComponents<Types::Parallax>();

        std::vector<std::size_t> ids = arrParralax.getExistingsId();

        for (auto &id : ids) {
            if (arrPosition.exist(id)) {
                checkResetParralax(id, arrPosition);
            }
        }
    }

    void deathChecker(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        Registry::components<Types::Health> arrHealth =
            Registry::getInstance().getComponents<Types::Health>();
        Registry::components<Types::Dead> arrDead =
            Registry::getInstance().getComponents<Types::Dead>();

        std::vector<std::size_t> ids = arrHealth.getExistingsId();
        for (auto itIds = ids.begin(); itIds != ids.end(); itIds++) {
            if (arrHealth.exist(*itIds) && arrHealth[*itIds].hp <= 0
                && arrDead.exist(*itIds)) {
                executeDeathFunction(*itIds, arrDead);
            }
        }
    }

    void initPlayer(std::size_t managerId, std::size_t systemId)
    {
        nlohmann::json jsonData = openJsonData("assets/Json/playerData.json");
        std::size_t id = Registry::getInstance().addEntity();

        if (jsonData["player"] == nullptr) {
            return;
        }
        jsonData = jsonData["player"];
        Registry::getInstance().getComponents<Raylib::Sprite>().insertBack(
            {jsonData["spritePath"], jsonData["width"], jsonData["height"], id}
        );
        Registry::getInstance().getComponents<Types::Position>().insertBack(
            {Types::Position(jsonData["position"])}
        );
        Registry::getInstance().getComponents<Types::Rect>().insertBack(
            {Types::Rect(jsonData["rect"])}
        );
        Registry::getInstance().getComponents<Types::CollisionRect>().insertBack(
            {Types::CollisionRect(jsonData["collisionRect"])}
        );
        Registry::getInstance().getComponents<Types::AnimRect>().insertBack(
            Types::AnimRect(Types::Rect(jsonData["rect"]),
            std::vector<Types::Rect>(jsonData["animRect"]["move"]),
            std::vector<Types::Rect>(jsonData["animRect"]["attack"]),
            std::vector<Types::Rect>(jsonData["animRect"]["dead"]))
        );
        Registry::getInstance().getComponents<Types::Player>().insertBack({});
        Registry::getInstance().getComponents<Types::Damage>().insertBack(
            {jsonData["damage"]}
        );
        Registry::getInstance().getComponents<Types::Health>().insertBack(
            {jsonData["health"]}
        );
        Registry::getInstance().getComponents<Types::Dead>().insertBack(
            {std::nullopt}
        );
        Registry::getInstance().setToFrontLayers(id);
        SystemManagersDirector::getInstance()
            .getSystemManager(managerId)
            .removeSystem(systemId);
    }

#ifndef NDEBUG
    std::vector<std::function<void(std::size_t, std::size_t)>> getECSSystems()
    {
        return {
            windowCollision,
            initPlayer,
            initParalax,
            entitiesCollision,
            moveEntities,
            debugCollisionRect,
            deathChecker,
            manageParallax};
    }
#else
    std::vector<std::function<void(std::size_t, std::size_t)>> getECSSystems()
    {
        return {
            windowCollision,
            initPlayer,
            initParalax,
            entitiesCollision,
            moveEntities,
            deathChecker,
            manageParallax};
    }
#endif
} // namespace Systems
