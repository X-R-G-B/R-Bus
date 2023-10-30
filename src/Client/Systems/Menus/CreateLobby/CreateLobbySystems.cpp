/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** CreateLobbySystems
*/

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
#include "CreateLobbySystems.hpp"

namespace Systems {
    namespace CreateLobby {

        static bool isNumber(const std::string &str)
        {
            return std::all_of(str.begin(), str.end(), ::isdigit);
        }

        static bool getNameAndMaxNb(std::string &name, std::string &maxNbPlayer)
        {
            Registry::components<Types::InputBox> arrInputBox =
                Registry::getInstance().getComponents<Types::InputBox>();
            std::vector<std::size_t> ids =
                Registry::getInstance().getEntitiesByComponents({typeid(Types::InputBox)});

            for (auto id : ids) {
                if (arrInputBox[id].name == "name" && name.empty()) {
                    name = arrInputBox[id].text;
                }
                if (arrInputBox[id].name == "maxNb" && maxNbPlayer.empty()) {
                    if (!isNumber(arrInputBox[id].text)) {
                        Logger::error("Max nb player is not a number (" + arrInputBox[id].text + ")");
                        return false;
                    }
                    if (std::stoi(arrInputBox[id].text) > 0) {
                        maxNbPlayer = arrInputBox[id].text;
                    }
                }
            }
            return !name.empty() && !maxNbPlayer.empty();
        }

        void onButtonCreateLobbyNormalClicked()
        {
            std::string name;
            std::string maxNvPlayer;

            if (getNameAndMaxNb(name, maxNvPlayer)) {
                unsigned int nbPlayer = static_cast<unsigned int>(std::stoi(maxNvPlayer));
                Nitwork::NitworkClient::getInstance().addCreateLobbyMsg(name, CLASSIC_GAME, nbPlayer);
            }
        }

        void initCreateLobby(std::size_t managerId, std::size_t systemId)
        {
            if (Scene::SceneManager::getInstance().getCurrentScene() != Scene::Scene::CREATE_LOBBY) {
                SystemManagersDirector::getInstance().getSystemManager(managerId).removeSystem(systemId);
                return;
            }
            try {
                nlohmann::json createLobbyNormalButton =
                    Json::getInstance().getDataByVector({"createLobbyMenu", "gametype_normal"}, JsonType::CREATE_LOBBY);
                nlohmann::json lobbyName =
                    Json::getInstance().getDataByVector({"createLobbyMenu", "name"}, JsonType::CREATE_LOBBY);
                nlohmann::json maxNbPlayer =
                    Json::getInstance().getDataByVector({"createLobbyMenu", "maxNb"}, JsonType::CREATE_LOBBY);
                Menu::MenuBuilder::getInstance().initMenuEntity(
                    createLobbyNormalButton,
                    onButtonCreateLobbyNormalClicked);
                Menu::MenuBuilder::getInstance().initMenuEntity(lobbyName);
                Menu::MenuBuilder::getInstance().initMenuEntity(maxNbPlayer);
            } catch (const std::exception &err) {
                Logger::error(
                    "Counldn't load menu correctly, verify your json data : " + std::string(err.what()));
            }

            SystemManagersDirector::getInstance().getSystemManager(managerId).removeSystem(systemId);
        }

        std::vector<std::function<void(std::size_t /*unused*/, std::size_t /*unused*/)>> getCreateLobbySystems()
        {
            return {initCreateLobby};
        }

    } // namespace CreateLobby
} // namespace Systems