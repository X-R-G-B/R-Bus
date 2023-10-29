/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** SelectLobby
*/

#include "SelectLobbySystems.hpp"
#include <string>
#include "CustomTypes.hpp"
#include "Menu.hpp"
#include "MessageTypes.h"
#include "NitworkClient.hpp"
#include "SceneManager.hpp"
#include "SystemManagersDirector.hpp"

namespace Systems::SelectLobbySystems {

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
                if (std::stoi(arrInputBox[id].text) > 0) {
                    maxNbPlayer = arrInputBox[id].text;
                }
            }
        }
        return !name.empty() && !maxNbPlayer.empty();
    }

    static void setNameAndMaxNbEmpty()
    {
        Registry::components<Types::InputBox> arrInputBox =
            Registry::getInstance().getComponents<Types::InputBox>();
        std::vector<std::size_t> ids =
            Registry::getInstance().getEntitiesByComponents({typeid(Types::InputBox)});

        for (auto id : ids) {
            if (arrInputBox[id].name == "name" && !arrInputBox[id].text.empty()) {
                arrInputBox[id].text = "";
            }
            if (arrInputBox[id].name == "maxNb" && !arrInputBox[id].text.empty()) {
                arrInputBox[id].text = "";
            }
        }
    }

    void onButtonCreateLobbyNormalClicked()
    {
        std::string name;
        std::string maxNvPlayer;

        if (getNameAndMaxNb(name, maxNvPlayer)) {
            unsigned int nbPlayer = static_cast<unsigned int>(std::stoi(maxNvPlayer));
            Nitwork::NitworkClient::getInstance().addCreateLobbyMsg(name, CLASSIC_GAME, nbPlayer);
            setNameAndMaxNbEmpty();
        }
    }

    void initSelectLoby(std::size_t managerId, std::size_t systemId)
    {
        if (Scene::SceneManager::getInstance().getCurrentScene() != Scene::Scene::SELECT_LOBBY) {
            SystemManagersDirector::getInstance().getSystemManager(managerId).removeSystem(systemId);
            return;
        }
        nlohmann::json createLobbyNormalButton =
            Json::getInstance().getDataByVector({"menu", "gametype_normal"}, JsonType::SELECT_LOBBY);
        nlohmann::json lobbyName =
            Json::getInstance().getDataByVector({"menu", "name"}, JsonType::SELECT_LOBBY);
        nlohmann::json maxNbPlayer =
            Json::getInstance().getDataByVector({"menu", "maxNb"}, JsonType::SELECT_LOBBY);

        try {
            Menu::MenuBuilder::getInstance().initMenuEntity(createLobbyNormalButton, onButtonCreateLobbyNormalClicked);
            Menu::MenuBuilder::getInstance().initMenuEntity(lobbyName);
            Menu::MenuBuilder::getInstance().initMenuEntity(maxNbPlayer);
        } catch (const std::exception &err) {
            Logger::error(
                "Counldn't load menu correctly, verify your json data : " + std::string(err.what()));
        }

        SystemManagersDirector::getInstance().getSystemManager(managerId).removeSystem(systemId);
    }

    void sendListLobby(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        static bool first = true;

        if (first) {
            first = false;
            Nitwork::NitworkClient::getInstance().addListLobbyMsg();
        }
    }
    std::vector<std::function<void(std::size_t, std::size_t)>> getLobbySystems()
    {
        return {initSelectLoby, sendListLobby};
    }
}
