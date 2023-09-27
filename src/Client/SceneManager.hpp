/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** SceneManager
*/

#pragma once

#include <vector>
#include <functional>
#include <array>
#include <cstddef>

enum Scene {
    MAIN_GAME,
    SCENE_MAX
};

enum SystemManagers {
    GAME,
    EVENTS,
    DISPLAY,
    MANAGER_MAX
};

class SceneManager {
    public:
        static SceneManager &getInstance();
        void changeScene(Scene scene);
        void stop();
    private:
        SceneManager();

        Scene _currentScene;
        bool _stop;
        const std::array<std::vector<SystemManagers>, 1> _scenes = {
            std::vector<SystemManagers>{EVENTS, GAME, DISPLAY}
        };

        // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
        static SceneManager _instance;
        // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
};
