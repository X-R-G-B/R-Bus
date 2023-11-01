/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** SelectLobby
*/

#include "SelectLobbySystems.hpp"
#include <algorithm>
#include <string>
#include "CustomTypes.hpp"
#include "Geometry.hpp"
#include "Graphics.hpp"
#include "Systems.hpp"
#include "Menu.hpp"
#include "MessageTypes.h"
#include "NitworkClient.hpp"
#include "SceneManager.hpp"
#include "SystemManagersDirector.hpp"

namespace Systems::SelectLobbySystems {
    
    std::size_t LobbyStatus::pageNbr = 1;

    LobbyStatus::LobbyStatus(const std::string &ip, n_port_t port): ip(ip), port(port)
    {
    }

    void onButtonGotoCreateLobbyClicked()
    {
        Scene::SceneManager::getInstance().changeScene(Scene::Scene::CREATE_LOBBY);
    }

    void initSelectLoby(std::size_t managerId, std::size_t systemId)
    {
        if (Scene::SceneManager::getInstance().getCurrentScene() != Scene::Scene::SELECT_LOBBY) {
            SystemManagersDirector::getInstance().getSystemManager(managerId).removeSystem(systemId);
            return;
        }
        try {
            nlohmann::json bigBox = Json::getInstance().getDataByVector({"lobbyMenu", "bigBox"}, JsonType::SELECT_LOBBY);
            nlohmann::json createLobbyNormalButton =
                Json::getInstance().getDataByVector({"lobbyMenu", "gotoCreateLobby"}, JsonType::SELECT_LOBBY);
            // nlohmann::json lobbyButton = Json::getInstance().getDataByVector({"menu", "lobbyBox"}, JsonType::SELECT_LOBBY);
            Menu::MenuBuilder::getInstance().initMenuEntity(
                createLobbyNormalButton,
                onButtonGotoCreateLobbyClicked);
            // Menu::MenuBuilder::getInstance().initMenuEntity(lobbyButton);
            Menu::MenuBuilder::getInstance().initMenuEntity(bigBox);
        } catch (const std::exception &err) {
            Logger::error(
                "Counldn't load menu correctly, verify your json data : " + std::string(err.what()));
        }

        SystemManagersDirector::getInstance().getSystemManager(managerId).removeSystem(systemId);
    }

    void sendListLobby(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        static std::size_t clockId = Registry::getInstance().getClock().create();

        if (Registry::getInstance().getClock().elapsedSecondsSince(clockId) < 2) {
            return;
        }
        Registry::getInstance().getClock().decreaseSeconds(clockId, 2);
        Nitwork::NitworkClient::getInstance().addListLobbyMsg();
    }

    static std::string gameTypeToString(enum gameType_e gameType)
    {
        switch (gameType) {
            case CLASSIC_GAME: return "CLASSIC_GAME";
            default: return "Unknow Game Type";
        }
    }

    void createLobbyRow(std::size_t managerId, std::size_t systemId)
    {
        static std::size_t clockId = Registry::getInstance().getClock().create();

        if (Registry::getInstance().getClock().elapsedSecondsSince(clockId) < 1) {
            return;
        }
        Registry::getInstance().getClock().decreaseSeconds(clockId, 1);
        // list of all lobby
        auto idsLobbyStatus = Registry::getInstance().getEntitiesByComponents({typeid(LobbyStatus), typeid(Raylib::Text), typeid(Types::InputBox)});
        auto ids       = Registry::getInstance().getEntitiesByComponents({typeid(struct lobby_s)});
        auto &arrLobbyText = Registry::getInstance().getComponents<Raylib::Text>();
        auto &arrLobbyStatus = Registry::getInstance().getComponents<LobbyStatus>();
        auto &arrLobby = Registry::getInstance().getComponents<struct lobby_s>();
        static std::size_t nbrOfIt = 0;
        std::size_t pageNbr = 1;
        bool firstInit = false;

        if (idsLobbyStatus.size() != 0) {
            pageNbr = arrLobbyStatus[idsLobbyStatus[0]].pageNbr;
            std::cout << "Oui oui baguette" << std::endl;
        } else {
            std::cout << "Enchanté" << std::endl;
            firstInit = true;
        }
        std::cout << "nbrOfIt nbr : " << nbrOfIt << std::endl;
        std::cout << "page nbr : " << pageNbr * 5 << std::endl;
        std::cout << "FYUBZAIUYFBAZUYFBAUYOZFBIOAZBYHFOIUAZFBOO" << std::endl;
        std::cout << "nbrOfIt nbr : " << nbrOfIt << std::endl;
        std::cout << "size nbr : " << ids.size() << std::endl;

        for (auto it = idsLobbyStatus.begin(); nbrOfIt < pageNbr * 5 && nbrOfIt < ids.size();) {
            if (firstInit) {
                try {
                    for (std::size_t i = 0; i < pageNbr * 5; i++) {
                        nlohmann::json lobbyBox = Json::getInstance().getDataByVector({"lobbyMenu", "lobbyBox"}, JsonType::SELECT_LOBBY);
                        std::size_t id = ::Menu::MenuBuilder::getInstance().initMenuEntity(lobbyBox);

                        if (arrLobby.exist(id)) {
                            std::string text_t = std::string(arrLobby[nbrOfIt].name) + " | "
                                + std::to_string(arrLobby[nbrOfIt].maxNbPlayer) + " | "
                                + gameTypeToString(arrLobby[nbrOfIt].gameType);
                            if (arrLobbyText.exist(id)) {
                                arrLobbyText[id].setCurrentText(text_t);
                            }
                        }
                    }
                    return;
                } catch (std::runtime_error &err) {
                    Logger::error(
                        "Counldn't load menu correctly, verify your json data : " + std::string(err.what()));
                }
                LobbyStatus newLobby(std::string(arrLobby[nbrOfIt].lobbyInfos.ip), arrLobby[nbrOfIt].lobbyInfos.port);
                Registry::getInstance().getComponents<LobbyStatus>().insertBack(newLobby);
            } else {
                if (it != idsLobbyStatus.end()) {
                    std::string text_t = std::string(arrLobby[nbrOfIt].name) + " | "
                        + std::to_string(arrLobby[nbrOfIt].maxNbPlayer) + " | "
                        + gameTypeToString(arrLobby[nbrOfIt].gameType);
                    Raylib::Text text(text_t);
                    // arrLobbyText.insert();
                    *it++;
                }
            } 
        }
    }

    std::vector<std::function<void(std::size_t, std::size_t)>> getLobbySystems()
    {
            return {initSelectLoby, sendListLobby, createLobbyRow, Systems::moveEntities};
    }
} // namespace Systems::SelectLobbySystems
