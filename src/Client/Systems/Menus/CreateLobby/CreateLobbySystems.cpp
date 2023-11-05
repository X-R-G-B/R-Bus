/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** CreateLobbySystems
*/

#include "CreateLobbySystems.hpp"
#include <algorithm>
#include <string>
// #include "CustomTypes.hpp"
#include "B-luga-physics/ECSCustomTypes.hpp"
#include "B-luga-graphics/Raylib/Raylib.hpp"
#include "Menu.hpp"
#include "MessageTypes.h"
#include "NitworkClient.hpp"
#include "B-luga/SystemManagers/SystemManagersDirector.hpp"
#include "B-luga/SceneManager.hpp"
#include "ResourcesManager.hpp"
#include "init.hpp"

namespace Systems {
    namespace CreateLobby {

        void initCreateLobby(std::size_t managerId, std::size_t systemId)
        {
            if (Scene::SceneManager::getInstance().getCurrentScene() != CREATE_LOBBY) {
                SystemManagersDirector::getInstance().getSystemManager(managerId).removeSystem(systemId);
                return;
            }
            try {
                nlohmann::json jsonData = Json::getInstance().getDataByJsonType<nlohmann::json>(
                    ResourcesManager::getPathByJsonType(JsonType::CREATE_LOBBY),
                    "createLobbyMenu");
                ::Menu::MenuBuilder::getInstance().initMenuSceneEntity(
                    Json::getInstance().getDatasFromList(jsonData));
            } catch (const std::exception &err) {
                Logger::error(
                    "Counldn't load menu correctly, verify your json data : " + std::string(err.what()));
            }

            SystemManagersDirector::getInstance().getSystemManager(managerId).removeSystem(systemId);
        }

        std::vector<std::function<void(std::size_t /*unused*/, std::size_t /*unused*/)>>
        getCreateLobbySystems()
        {
            return {initCreateLobby};
        }

    } // namespace CreateLobby
} // namespace Systems
