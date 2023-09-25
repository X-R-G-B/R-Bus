/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** SceneManager implementation
*/

#include "SceneManager.hpp"
#include "GameManager.hpp"
#include "SystemEventManager.hpp"

SceneManager SceneManager::_instance = SceneManager();

SceneManager::SceneManager() : _currentScene(Scene::GAME)
{
    _logicManagers.push_back(GameManager());
    _eventManagers.push_back(SystemEventManager());
}

SceneManager &SceneManager::getInstance()
{
    return _instance;
}

void SceneManager::changeScene()
{

}
