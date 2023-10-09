/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** GraphicSystems
*/

#include "GraphicSystems.hpp"
#include "AudioSystems.hpp"
#include "DeathSystems.hpp"
#include "ECSCustomTypes.hpp"
#include "SpriteSystems.hpp"
#include "TextSystems.hpp"
#include "ParallaxSystems.hpp"
#include "Registry.hpp"

namespace Systems {

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
                Types::RectangleShape rectShape = {arrCollisionRect[id].width, arrCollisionRect[id].height};
                Registry::getInstance().getComponents<Types::RectangleShape>().insert(id, rectShape);
            }
        }
    }
#endif

    std::vector<std::function<void(std::size_t, std::size_t)>> GraphicSystems::getGraphicsSystems()
    {
        std::vector<std::function<void(std::size_t, std::size_t)>> audioSystems  = getAudioSystems();
        std::vector<std::function<void(std::size_t, std::size_t)>> spriteSystems = getSpriteSystems();
        std::vector<std::function<void(std::size_t, std::size_t)>> textSystems   = getTextSystems();
        std::vector<std::function<void(std::size_t, std::size_t)>> deathSystems =
            DeathSystems::getDeathSystems();
        std::vector<std::function<void(std::size_t, std::size_t)>> parallaxSystems = ParallaxSystems::getParallaxSystems();


#ifndef NDEBUG
        audioSystems.insert(audioSystems.end(), debugCollisionRect);
#endif
        audioSystems.insert(audioSystems.end(), spriteSystems.begin(), spriteSystems.end());
        audioSystems.insert(audioSystems.end(), textSystems.begin(), textSystems.end());
        audioSystems.insert(audioSystems.end(), deathSystems.begin(), deathSystems.end());
        audioSystems.insert(audioSystems.end(), parallaxSystems.begin(), parallaxSystems.end());
        return audioSystems;
    }
} // namespace Systems
