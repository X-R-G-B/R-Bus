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
#include <vector>

enum class ReturnValue { OK = 0, ERROR = 84};

enum class Scene { MENU, MAIN_GAME, SCENE_MAX };

enum class SystemManagers { GAME, EVENTS, DISPLAY };

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
        const std::array<std::vector<SystemManagers>, 2> _scenes = {{
            {SystemManagers::EVENTS, SystemManagers::GAME, SystemManagers::DISPLAY},
            {SystemManagers::EVENTS, SystemManagers::GAME, SystemManagers::DISPLAY}
        }};

        // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
        static bool _init;
        static SceneManager _instance;
        // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
};
