/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** PhysicSystems
*/

#define _USE_MATH_DEFINES
#include <cmath>
#include <fstream>
#include "ECSCustomTypes.hpp"
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
            std::size_t clockId = physicComps[id].getClockId(ZIGZAG);
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

    void updatePhysics(std::size_t, std::size_t)
    {
        std::vector<std::size_t> bouncingId;
        std::vector<std::size_t> zigzagId;
        Registry::components<Types::Physics> physicComps =
            Registry::getInstance().getComponents<Types::Physics>();
        std::vector<std::size_t> ids = Registry::getInstance().getEntitiesByComponents(
            {typeid(Types::Physics), typeid(Types::Position), typeid(Types::Velocity)});

        for (std::size_t id : ids) {
            if (physicComps[id].hasPhysics(BOUNCING)) {
                bouncingId.push_back(id);
            } else if (physicComps[id].hasPhysics(ZIGZAG)) {
                zigzagId.push_back(id);
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
