/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** CreateServerSystems
*/

#include "CreateServerSystems.hpp"
#include "B-luga/SystemManagers/SystemManagersDirector.hpp"
#include "B-luga/SceneManager.hpp"
#include "ResourcesManager.hpp"
#include "Parallax.hpp"
#include "Menu.hpp"
#include "init.hpp"


namespace Systems::CreateServerSystems {
    void initCreateServer(std::size_t managerId, std::size_t systemId)
    {
        if (Scene::SceneManager::getInstance().getCurrentScene() != CREATE_SERVER_SCENE) {
            SystemManagersDirector::getInstance().getSystemManager(managerId).removeSystem(systemId);
            return;
        }
        try {
            Parallax::initParalax();
            nlohmann::json jsonData = Json::getInstance().getDataByJsonType<nlohmann::json>(
                ResourcesManager::getPathByJsonType(JsonType::CREATE_SERVER),
                "createServerMenu");
            ::Menu::MenuBuilder::getInstance().initMenuSceneEntity(
                    Json::getInstance().getDatasFromList(jsonData));
        } catch (std::runtime_error &err) {
            Logger::info(err.what());
        }
        SystemManagersDirector::getInstance().getSystemManager(managerId).removeSystem(systemId);
    }

    std::vector<std::function<void(std::size_t, std::size_t)>> getCreateServerSystems()
    {
        return {initCreateServer};
    }
}
