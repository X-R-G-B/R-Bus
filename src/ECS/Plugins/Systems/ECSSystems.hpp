/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Systems
*/

#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
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
        auto &deadList        = registry.getComponents<Types::Dead>();
        auto deadIdList      = deadList.getExistingsId();
        Clock &clock         = registry.getClock();

        for (auto id : deadIdList) {
            Types::Dead &dead = deadList[id];
            if (static_cast<int>(dead.clockId) > -1
                && clock.elapsedMillisecondsSince(dead.clockId) >= dead.timeToWait) {
                Logger::debug("destroy after time");
                registry.addToRemove(id);
            }
        }
    }

    static void
    executeDeathFunction(std::size_t id, Registry::components<Types::Dead> arrDead)
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
            Registry::getInstance().addToRemove(id);
        }
    }

    static void deathChecker(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        Registry::components<struct health_s> arrHealth =
            Registry::getInstance().getComponents<struct health_s>();
        Registry::components<Types::Dead> arrDead = Registry::getInstance().getComponents<Types::Dead>();

        std::vector<std::size_t> ids = arrHealth.getExistingsId();
        std::sort(ids.begin(), ids.end());
        for (auto &id : ids) {
            if (arrHealth.exist(id) && arrHealth[id].hp <= 0) {
                executeDeathFunction(id, arrDead);
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

        std::sort(ids.begin(), ids.end());
        for (auto &id : ids) {
            bool destroyable = !arrNoRemove.exist(id);
            if (destroyable && isOutsideWindow(arrPosition[id])) {
                Registry::getInstance().addToRemove(id);
            }
        }
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
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        Registry::components<Types::Velocity> velocities =
            Registry::getInstance().getComponents<Types::Velocity>();
        Registry::components<Types::Physics> physicComps =
            Registry::getInstance().getComponents<Types::Physics>();
        Registry::components<Types::Position> positionComp =
            Registry::getInstance().getComponents<Types::Position>();

        for (std::size_t id : ids) {
            std::size_t clockId = physicComps[id].getClockId(Types::PhysicsType::ZIGZAG);
            std::size_t elapsedTimeInMs =
                Registry::getInstance().getClock().elapsedMillisecondsSince(clockId);
            if (elapsedTimeInMs == static_cast<std::size_t>(-1)) {
                Registry::getInstance().getClock().restart(clockId);
                elapsedTimeInMs = 0;
            }
            // Height of the wave = 10% of the screen
            float amplitude = 10.0F;
            // Time for the complete zigzag cycle 400ms
            float period = 400.0F;
            float WavePosY =
                amplitude * std::sin(2.0F * static_cast<float>(M_PI) * elapsedTimeInMs / period);
            positionComp[id].y =
                physicComps[id].getOriginPos().y + Maths::floatToIntConservingDecimals(WavePosY);
            velocities[id].speedY = 0;
        }
    }

    static void updatePhysics(std::size_t, std::size_t)
    {
        std::vector<std::size_t> bouncingId;
        std::vector<std::size_t> zigzagId;
        Registry::components<Types::Physics> physicComps =
            Registry::getInstance().getComponents<Types::Physics>();
        std::vector<std::size_t> ids = Registry::getInstance().getEntitiesByComponents(
            {typeid(Types::Physics), typeid(Types::Position), typeid(Types::Velocity)});

        for (std::size_t id : ids) {
            if (physicComps[id].hasPhysics(Types::PhysicsType::BOUNCING)) {
                bouncingId.push_back(id);
            } else if (physicComps[id].hasPhysics(Types::PhysicsType::ZIGZAG)) {
                zigzagId.push_back(id);
            }
        }
        updateBouncePhysics(bouncingId);
        updateZigzagPhysics(zigzagId);
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
                    moveEntities,
                    updatePhysics
                };
            }
    };
} // namespace Systems
