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

enum CustomIndex { BULLET, PLAYER, ENNEMY };

enum ReturnValue { OK = 0, ERROR = 84 };

enum Scene { MENU, MAIN_GAME, SCENE_MAX };

enum SystemManagers { GAME, EVENTS, DISPLAY };

class SceneManager {
    public:
        static SceneManager &getInstance();
        int run();
        void changeScene(Scene scene);
        Scene getCurrentScene() const;
        void stop();

    private:
        SceneManager();

        Scene _currentScene;
        bool _stop;
        const std::array<std::vector<SystemManagers>, 2> _scenes = {
            std::vector<SystemManagers> {EVENTS, GAME, DISPLAY},
            std::vector<SystemManagers> {EVENTS, GAME, DISPLAY}
        };
        const std::array<std::vector<CustomIndex>, 2> _scenesCustomIndexes = {
            std::vector<CustomIndex> {PLAYER },
            std::vector<CustomIndex> { PLAYER, BULLET, ENNEMY}
        };

        // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
        static bool _init;
        static SceneManager _instance;
        // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
};
