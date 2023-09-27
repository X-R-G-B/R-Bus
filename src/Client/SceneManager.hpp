/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** SceneManager
*/

#pragma once

#include <array>
#include <cstddef>
#include <functional>
#include <optional>
#include <vector>

enum ReturnValue { OK = 0, ERROR = 84 };

enum Scene { MAIN_GAME, MENU, SCENE_MAX };

enum SystemManagers { GAME, EVENTS, DISPLAY, MANAGER_MAX };

class SceneManager {
    public:
        static SceneManager &getInstance();
        int run();
        void changeScene(Scene scene);
        void stop();

    private:
        SceneManager();

        Scene _currentScene;
        bool _stop;
        const std::array<std::vector<SystemManagers>, 2> _scenes = {
            std::vector<SystemManagers> {EVENTS, GAME, DISPLAY},
            std::vector<SystemManagers> {EVENTS, DISPLAY}
        };

        // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
        static bool _init;
        static SceneManager _instance;
        // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
};
