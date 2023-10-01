/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** GraphicSystems
*/

#include "GraphicSystems.hpp"
#include "AudioSystems.hpp"
#include "SpriteSystems.hpp"
#include "TextSystems.hpp"

namespace Systems {
    std::vector<std::function<void(std::size_t, std::size_t)>>
    GraphicSystems::getGraphicsSystems()
    {
        std::vector<std::function<void(std::size_t, std::size_t)>>
            audioSystems = getAudioSystems();
        std::vector<std::function<void(std::size_t, std::size_t)>>
            spriteSystems = getSpriteSystems();
        std::vector<std::function<void(std::size_t, std::size_t)>> textSystems =
            getTextSystems();

        audioSystems.insert(
            audioSystems.end(),
            spriteSystems.begin(),
            spriteSystems.end());
        audioSystems.insert(
            audioSystems.end(),
            textSystems.begin(),
            textSystems.end());
        return audioSystems;
    }
} // namespace Systems
