/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** CreateLobbySystems
*/

#include "CreateLobbySystems.hpp"
#include <algorithm>
#include <string>
#include "CustomTypes.hpp"
#include "ECSCustomTypes.hpp"
#include "Geometry.hpp"
#include "Graphics.hpp"
#include "Menu.hpp"
#include "MessageTypes.h"
#include "NitworkClient.hpp"
#include "SceneManager.hpp"
#include "SystemManagersDirector.hpp"
#include "Systems.hpp"

namespace Systems {
    namespace CreateLobby {

        void initCreateLobby(std::size_t managerId, std::size_t systemId)
        {
            if (Scene::SceneManager::getInstance().getCurrentScene() != Scene::Scene::CREATE_LOBBY) {
                SystemManagersDirector::getInstance().getSystemManager(managerId).removeSystem(systemId);
                return;
            }
            try {
                nlohmann::json jsonData = Json::getInstance().getDataByJsonType<nlohmann::json>(
                    "createLobbyMenu",
                    JsonType::CREATE_LOBBY);
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
            return {initCreateLobby, Systems::moveEntities};
        }

    } // namespace CreateLobby
} // namespace Systems