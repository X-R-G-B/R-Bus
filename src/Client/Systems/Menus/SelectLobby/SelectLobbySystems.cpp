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
#include "Logger.hpp"
#include "SystemManagersDirector.hpp"
#include "Maths.hpp"
#include "ButtonCallbacks.hpp"

namespace Systems::SelectLobbySystems {
    
    std::size_t LobbyStatus::pageNbr = 1;

    LobbyStatus::LobbyStatus(const std::string &ip = "", n_port_t port = -1): ip(ip), port(port)
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
            nlohmann::json joinLobbyButton =
                Json::getInstance().getDataByVector({"lobbyMenu", "joinLobby"}, JsonType::SELECT_LOBBY);
            Menu::MenuBuilder::getInstance().initMenuEntity(
                joinLobbyButton,
                Menu::Callback::connectLobbySelected);
            Menu::MenuBuilder::getInstance().initMenuEntity(
                createLobbyNormalButton,
                onButtonGotoCreateLobbyClicked);
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

    void initLobbyRow(std::size_t managerId, std::size_t systemId)
    {
        auto &arrLobbyText = Registry::getInstance().getComponents<Raylib::Text>();
        auto &arrPosition = Registry::getInstance().getComponents<Types::Position>();
        int offset = 1000;

        try {
            for (std::size_t i = 0; i < 5; i++) {
                Logger::info("INIT LOBBYSTATUS....");
                nlohmann::json lobbyBox = Json::getInstance().getDataByVector({"lobbyMenu", "lobbyBox"}, JsonType::SELECT_LOBBY);
                std::size_t id = ::Menu::MenuBuilder::getInstance().initMenuEntity(lobbyBox);
                Logger::info("MENU ENTITY CORRECTLY INIT");
                if (arrPosition.exist(id)) {
                    Logger::info("ACTUAL OFFSET FOR LOBBY : " + std::to_string(offset));
                    arrPosition[id].y += offset * i;
                }
                if (arrLobbyText.exist(id)) {
                    Raylib::Vector2 pos(arrLobbyText[id].getPosition().x, arrLobbyText[id].getPosition().y);
                    pos.y += Maths::intToFloatConservingDecimals(offset) * i;
                    arrLobbyText[id].setPosition(pos);
                }
                LobbyStatus newLobby;
                Registry::getInstance().getComponents<LobbyStatus>().insertBack(newLobby);
            }
        } catch (std::runtime_error &err) {
            Logger::error(
                "Counldn't load menu correctly, verify your json data : " + std::string(err.what()));
        }
        Logger::info("OUI OUI OUI");
        SystemManagersDirector::getInstance().getSystemManager(managerId).removeSystem(systemId);
    }

    void updateLobbyRow(std::size_t managerId, std::size_t systemId)
    {
        static std::size_t clockId = Registry::getInstance().getClock().create();
        
        if (Registry::getInstance().getClock().elapsedSecondsSince(clockId) < 1) {
            return;
        }
        Registry::getInstance().getClock().decreaseSeconds(clockId, 1);
        // list of all lobby
        auto idsLobbyStatus = Registry::getInstance().getEntitiesByComponents({typeid(LobbyStatus), typeid(Raylib::Text)});
        auto idsClickableLobbys = Registry::getInstance().getEntitiesByComponents({typeid(LobbyStatus), typeid(Raylib::Text), typeid(Types::InputBox)});
        auto ids       = Registry::getInstance().getEntitiesByComponents({typeid(struct lobby_s)});
        auto &arrLobbyStatus = Registry::getInstance().getComponents<LobbyStatus>();
        auto &arrLobbyText = Registry::getInstance().getComponents<Raylib::Text>();
        auto &arrLobby = Registry::getInstance().getComponents<struct lobby_s>();
        static std::size_t nbrOfIt = 0;
        static std::size_t pageNbr = 1;

        if (idsLobbyStatus.size() != 0) {
            pageNbr = arrLobbyStatus[idsLobbyStatus[0]].pageNbr;
        }
        for (; nbrOfIt < pageNbr * 5 && nbrOfIt < ids.size();) {
            for (auto &id : idsLobbyStatus) {
                if (arrLobbyStatus[id].ip == "" && nbrOfIt < ids.size()) {
                    std::string name(arrLobby[ids[nbrOfIt]].name);
                    if (name.size() > 15) {
                        name = name.substr(0, 15);
                        name += "...";
                    }
                    std::string text_t = name + " | "
                        + std::to_string(arrLobby[ids[nbrOfIt]].maxNbPlayer) + " | "
                        + gameTypeToString(arrLobby[ids[nbrOfIt]].gameType);
                    arrLobbyStatus[id].ip = std::string(arrLobby[ids[nbrOfIt]].lobbyInfos.ip);
                    arrLobbyStatus[id].port = arrLobby[ids[nbrOfIt]].lobbyInfos.port;
                    Raylib::Text text(text_t);
                    if (arrLobbyText.exist(id)) {
                        arrLobbyText[id].setCurrentText(text_t);
                    }
                    nbrOfIt++;
                }
            }
        }
    }

    std::vector<std::function<void(std::size_t, std::size_t)>> getLobbySystems()
    {
            return {initSelectLoby, initLobbyRow, updateLobbyRow, sendListLobby, Systems::moveEntities};
    }
} // namespace Systems::SelectLobbySystems
