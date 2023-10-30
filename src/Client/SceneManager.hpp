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

namespace Scene {

    enum class ReturnValue { OK = 0, RET_ERROR = 84 };

    enum class Scene { MENU, MAIN_GAME, SELECT_LOBBY, CREATE_LOBBY, SCENE_MAX };

    enum class SystemManagers { GAME, EVENTS, DISPLAY, NETWORK, MENU, NETWORK_MENU, NETWORK_CREATE_LOBBY };

    class SceneManager {
        public:
            static SceneManager &getInstance();
            int run();
            void changeScene(Scene scene);
            [[nodiscard]] Scene getCurrentScene() const;
            void stop();

        private:
            SceneManager();

            Scene _currentScene;
            bool _stop;
            const std::array<std::vector<SystemManagers>, 4> _scenes = {
                {{SystemManagers::DISPLAY, SystemManagers::MENU},
                 {SystemManagers::EVENTS,
                  SystemManagers::GAME,
                  SystemManagers::DISPLAY,
                  SystemManagers::NETWORK},
                 {SystemManagers::DISPLAY,
                  SystemManagers::NETWORK,
                  SystemManagers::NETWORK_MENU,
                  SystemManagers::MENU},
                 {SystemManagers::DISPLAY,
                  SystemManagers::NETWORK,
                  SystemManagers::NETWORK_CREATE_LOBBY,
                  SystemManagers::MENU}}
            };

            // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
            static bool _init;
            static SceneManager _instance;
            // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
    };
} // namespace Scene
