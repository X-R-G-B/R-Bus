/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Systems
*/

#pragma once

#include <cstddef>
#include <functional>
#include <string>
#include <vector>
#include "Json.hpp"
#include "ECSSystems.hpp"
#include <nlohmann/json.hpp>
#include "ECSCustomTypes.hpp"
#include "Maths.hpp"
#include "Registry.hpp"
#include "SystemManagersDirector.hpp"
#include "IPlugin.hpp"

namespace Systems {

    static void checkOutsideWindow(std::size_t id, Types::Container &container)
    {
        Registry &registry                                = Registry::getInstance();
        Registry::components<Types::Position> arrPosition = registry.getComponents<Types::Position>();
        Registry::components<Types::CollisionRect> arrCollisionRect =
            registry.getComponents<Types::CollisionRect>();

        if (Maths::intToFloatConservingDecimals(arrPosition[id].x) < container.minX) {
            arrPosition[id].x = Maths::floatToIntConservingDecimals(container.minX);
        }
        if (Maths::intToFloatConservingDecimals(arrPosition[id].y) < container.minY) {
            arrPosition[id].y = Maths::floatToIntConservingDecimals(container.minY);
        }
        if (Maths::intToFloatConservingDecimals(arrPosition[id].x)
            + Maths::intToFloatConservingDecimals(arrCollisionRect[id].width)
            > container.maxX) {
            arrPosition[id].x = Maths::floatToIntConservingDecimals(
                container.maxX - Maths::intToFloatConservingDecimals(arrCollisionRect[id].width));
        }
        if (Maths::intToFloatConservingDecimals(arrPosition[id].y)
            + Maths::intToFloatConservingDecimals(arrCollisionRect[id].height)
            > container.maxY) {
            arrPosition[id].y = Maths::floatToIntConservingDecimals(
                container.maxY - Maths::intToFloatConservingDecimals(arrCollisionRect[id].height));
        }
    }

    static void containerCollision(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        Registry &registry           = Registry::getInstance();
        auto &arrContainers          = registry.getComponents<Types::Container>();
        auto ids = registry.getEntitiesByComponents({typeid(Types::Position), typeid(Types::CollisionRect), typeid(Types::Container)});

        for (auto &id : ids) {
            checkOutsideWindow(id, arrContainers[id]);
        }
    }

    static void giveDamages(std::size_t firstEntity, std::size_t secondEntity)
    {
        Registry::components<Types::Damage> arrDamage =
            Registry::getInstance().getComponents<Types::Damage>();
        Registry::components<struct health_s> arrHealth =
            Registry::getInstance().getComponents<struct health_s>();

        if (arrDamage.exist(firstEntity) && arrDamage[firstEntity].damage > 0) {
            if (arrHealth.exist(secondEntity)) {
                arrHealth[secondEntity].hp -= arrDamage[firstEntity].damage;
                Registry::getInstance().callback(Events::TAKE_DAMAGE, secondEntity);
            }
        }
    }

    static void checkSide(std::size_t firstEntity, std::size_t secondEntity)
    {
        if (Registry::getInstance().checkAllies(firstEntity, secondEntity)) {
            return;
        }
        giveDamages(firstEntity, secondEntity);
        giveDamages(secondEntity, firstEntity);
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
                if (Maths::intToFloatConservingDecimals(entityPos.x)
                    < (Maths::intToFloatConservingDecimals(sndEntityPos.x)
                       + Maths::intToFloatConservingDecimals(sndEntityRect.width))
                    && (Maths::intToFloatConservingDecimals(entityPos.x)
                        + Maths::intToFloatConservingDecimals(entityColl.width))
                       > Maths::intToFloatConservingDecimals(sndEntityPos.x)
                    && Maths::intToFloatConservingDecimals(entityPos.y)
                       < (Maths::intToFloatConservingDecimals(sndEntityPos.y)
                          + Maths::intToFloatConservingDecimals(sndEntityRect.height))
                    && (Maths::intToFloatConservingDecimals(entityPos.y)
                        + Maths::intToFloatConservingDecimals(entityColl.height))
                       > Maths::intToFloatConservingDecimals(sndEntityPos.y)) {
                    checkSide(id, *itIds);
                }
            }
            itIds++;
        }
    }

    static void entitiesCollision(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        Registry &registry                                = Registry::getInstance();
        Registry::components<Types::Position> arrPosition = registry.getComponents<Types::Position>();
        Registry::components<Types::CollisionRect> arrCollisionRect =
            registry.getComponents<Types::CollisionRect>();
        std::vector<std::size_t> ids =
            registry.getEntitiesByComponents({typeid(Types::CollisionRect), typeid(Types::Position)});

        for (auto itIds = ids.begin(); itIds != ids.end(); itIds++) {
            checkCollisionEntity(itIds, ids, arrPosition, arrCollisionRect);
        }
    }

    const std::size_t moveTime = 20;

    static void moveEntities(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        Registry::components<Types::Position> arrPosition =
            Registry::getInstance().getComponents<Types::Position>();
        Registry::components<Types::Velocity> arrVelocity =
            Registry::getInstance().getComponents<Types::Velocity>();
        Clock &clock                 = Registry::getInstance().getClock();
        static std::size_t clockId   = clock.create();
        std::vector<std::size_t> ids = Registry::getInstance().getEntitiesByComponents(
            {typeid(Types::Position), typeid(Types::Velocity)});

        while (clock.elapsedMillisecondsSince(clockId) >= moveTime) {
            for (auto &id : ids) {
                Maths::addFloatToDecimalInt(
                    arrPosition[id].x,
                    Maths::intToFloatConservingDecimals(arrVelocity[id].speedX));
                Maths::addFloatToDecimalInt(
                    arrPosition[id].y,
                    Maths::intToFloatConservingDecimals(arrVelocity[id].speedY));
            }
            clock.decreaseMilliseconds(clockId, moveTime);
        }
    }

    static void checkDestroyAfterDeathCallBack(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        Registry &registry   = Registry::getInstance();
        auto deadList        = registry.getComponents<Types::Dead>();
        auto deadIdList      = deadList.getExistingsId();
        Clock &clock         = registry.getClock();
        std::size_t decrease = 0;

        std::sort(deadIdList.begin(), deadIdList.end());
        for (auto id : deadIdList) {
            auto tmpId        = id - decrease;
            Types::Dead &dead = deadList[tmpId];
            if (static_cast<int>(dead.clockId) > -1
                && clock.elapsedMillisecondsSince(dead.clockId) >= dead.timeToWait) {
                registry.removeEntity(tmpId);
                decrease++;
            }
        }
    }

    static void
    executeDeathFunction(std::size_t id, Registry::components<Types::Dead> arrDead, std::size_t &decrease)
    {
        if (arrDead.exist(id) && arrDead[id].deathFunction != std::nullopt) {
            Types::Dead &deadComp = arrDead[id];
            if (!deadComp.launched) {
                Registry::getInstance().callback(Events::ENTITY_DEATH, id);
                deadComp.deathFunction.value()(id);
                deadComp.clockId  = Registry::getInstance().getClock().create();
                deadComp.launched = true;
            }
        } else {
            Registry::getInstance().callback(Events::ENTITY_DEATH, id);
            Registry::getInstance().removeEntity(id);
            decrease++;
        }
    }

    static void deathChecker(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        Registry::components<struct health_s> arrHealth =
            Registry::getInstance().getComponents<struct health_s>();
        Registry::components<Types::Dead> arrDead = Registry::getInstance().getComponents<Types::Dead>();
        std::size_t decrease                      = 0;

        std::vector<std::size_t> ids = arrHealth.getExistingsId();
        std::sort(ids.begin(), ids.end());
        for (auto &id : ids) {
            auto tmpId = id - decrease;
            if (arrHealth.exist(tmpId) && arrHealth[tmpId].hp <= 0) {
                executeDeathFunction(tmpId, arrDead, decrease);
            }
        }
    }

    constexpr float outsideWindowTopLeft  = -25.0F;
    constexpr float outsideWindowBotRigth = 125.0F;

    static bool isOutsideWindow(const Types::Position &pos)
    {
        if (Maths::intToFloatConservingDecimals(pos.x) < outsideWindowTopLeft
            || Maths::intToFloatConservingDecimals(pos.x) > outsideWindowBotRigth
            || Maths::intToFloatConservingDecimals(pos.y) < outsideWindowTopLeft
            || Maths::intToFloatConservingDecimals(pos.y) > outsideWindowBotRigth) {
            return (true);
        }
        return (false);
    }

    static void destroyOutsideWindow(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        Registry::components<Types::Position> arrPosition =
            Registry::getInstance().getComponents<Types::Position>();

        auto arrNoRemove = Registry::getInstance().getComponents<Types::NoRemoveOutside>();
        std::vector<std::size_t> ids =
            Registry::getInstance().getEntitiesByComponents({typeid(Types::Position)});
        std::size_t decrease = 0;

        std::sort(ids.begin(), ids.end());
        for (auto &id : ids) {
            auto tmpId = id - decrease;
            bool destroyable = !arrNoRemove.exist(tmpId);
            if (destroyable && isOutsideWindow(arrPosition[tmpId])) {
                Registry::getInstance().removeEntity(tmpId);
                decrease++;
            }
        }
    }

    class ECSPlugin : public IPlugin {
        public:
            void initPlugin() override
            {}
            std::vector<std::function<void(std::size_t, std::size_t)>> getSystems() override
            {
                return {
                    containerCollision,
                    checkDestroyAfterDeathCallBack,
                    entitiesCollision,
                    destroyOutsideWindow,
                    deathChecker,
                    moveEntities};
            }
    };
} // namespace Systems
