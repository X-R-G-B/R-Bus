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

    class SceneManager {
        public:
            static SceneManager &getInstance();
            int run();
            void changeScene(std::size_t scene);
            std::size_t getCurrentScene() const;
            void stop();
            void setScenes(std::vector<std::vector<std::size_t>> scenes);
            void addScene(std::vector<std::size_t> scene);

        private:
            SceneManager();

            std::size_t _currentScene;
            bool _stop;
            std::vector<std::vector<std::size_t>> _scenes;

            // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
            static bool _init;
            static SceneManager _instance;
            // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
    };
} // namespace Scene
