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

namespace Systems {
    std::vector<std::function<void(std::size_t, std::size_t)>>
    GraphicSystems::getGraphicsSystems()
    {
        std::vector<std::function<void(std::size_t, std::size_t)>>
            graphicSystems;

        std::vector<std::function<void(std::size_t, std::size_t)>>
            audioSystems = getAudioSystems();
        std::vector<std::function<void(std::size_t, std::size_t)>>
            spriteSystems = getSpriteSystems();
        std::vector<std::function<void(std::size_t, std::size_t)>> textSystems =
            getTextSystems();
        std::vector<std::function<void(std::size_t, std::size_t)>>
            deathSystems = DeathSystems::getDeathSystems();

        graphicSystems.insert(
            graphicSystems.end(),
            audioSystems.begin(),
            audioSystems.end());
        graphicSystems.insert(
            graphicSystems.end(),
            spriteSystems.begin(),
            spriteSystems.end());
        graphicSystems.insert(
            graphicSystems.end(),
            textSystems.begin(),
            textSystems.end());
        graphicSystems.insert(
            graphicSystems.end(),
            deathSystems.begin(),
            deathSystems.end());
        return graphicSystems;
    }
} // namespace Systems
