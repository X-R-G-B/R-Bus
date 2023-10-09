/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** GraphicSystems
*/

#include "GraphicSystems.hpp"
#include "AudioSystems.hpp"
#include "DeathSystems.hpp"
#include "SpriteSystems.hpp"
#include "TextSystems.hpp"
#include "ParallaxSystems.hpp"

namespace Systems {
    std::vector<std::function<void(std::size_t, std::size_t)>> GraphicSystems::getGraphicsSystems()
    {
        std::vector<std::function<void(std::size_t, std::size_t)>> audioSystems  = getAudioSystems();
        std::vector<std::function<void(std::size_t, std::size_t)>> spriteSystems = getSpriteSystems();
        std::vector<std::function<void(std::size_t, std::size_t)>> textSystems   = getTextSystems();
        std::vector<std::function<void(std::size_t, std::size_t)>> deathSystems =
            DeathSystems::getDeathSystems();
        std::vector<std::function<void(std::size_t, std::size_t)>> parallaxSystems = ParallaxSystems::getParallaxSystems();


        audioSystems.insert(audioSystems.end(), spriteSystems.begin(), spriteSystems.end());
        audioSystems.insert(audioSystems.end(), textSystems.begin(), textSystems.end());
        audioSystems.insert(audioSystems.end(), deathSystems.begin(), deathSystems.end());
        audioSystems.insert(audioSystems.end(), parallaxSystems.begin(), parallaxSystems.end());
        return audioSystems;
    }
} // namespace Systems
