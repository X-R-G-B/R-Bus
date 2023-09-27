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
#include <optional>

#include <iostream>

enum Scene {
    MAIN_GAME,
    MENU,
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
        static std::optional<SceneManager> _instance;
        const std::array<std::vector<SystemManagers>, 2> _scenes = {
            std::vector<SystemManagers>{EVENTS, GAME, DISPLAY},
            std::vector<SystemManagers>{EVENTS, DISPLAY}
        };
};
