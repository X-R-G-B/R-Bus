/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** SceneManager
*/

#pragma once

#include <list>
#include "ISystemManager.hpp"
#include "GraphicManager.hpp"

enum Scene {
    GAME,
    Scene_max
};

class SceneManager {
    public:
        static SceneManager &getInstance();
        void changeScene();
    private:
        SceneManager();
        std::list<ISystemManager> _logicManagers;
        std::list<ISystemManager> _eventManagers;
        GraphicManager _graphicManager;
        Scene _currentScene;

        // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
        static SceneManager _instance;
        // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
};
