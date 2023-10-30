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
#include "ECSCustomTypes.hpp"
#include "Geometry.hpp"
#include "Graphics.hpp"
#include "Menu.hpp"
#include "MessageTypes.h"
#include "NitworkClient.hpp"
#include "SceneManager.hpp"
#include "SystemManagersDirector.hpp"

namespace Systems::SelectLobbySystems {

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

    void createLobbyRow(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        static std::size_t clockId = Registry::getInstance().getClock().create();

        if (Registry::getInstance().getClock().elapsedSecondsSince(clockId) < 1) {
            return;
        }
        Registry::getInstance().getClock().decreaseSeconds(clockId, 1);
        // already created lobby
        auto idsLobbyStatus =
            Registry::getInstance().getEntitiesByComponents({typeid(LobbyStatus), typeid(Raylib::Text)});
        auto &arrLobbyStatus = Registry::getInstance().getComponents<LobbyStatus>();
        auto &arrLobbyText   = Registry::getInstance().getComponents<Raylib::Text>();
        // list of all lobby
        auto ids       = Registry::getInstance().getEntitiesByComponents({typeid(struct lobby_s)});
        auto &arrLobby = Registry::getInstance().getComponents<struct lobby_s>();
        float x        = 10;
        float y        = 50;

        for (auto id : ids) {
            bool found = false;
            for (auto idLobbyStatus : idsLobbyStatus) {
                if (arrLobby[id].lobbyInfos.port == arrLobbyStatus[idLobbyStatus].port
                    && std::string(arrLobby[id].lobbyInfos.ip) == arrLobbyStatus[idLobbyStatus].ip) {
                    x     = arrLobbyText[idLobbyStatus].x();
                    y     = arrLobbyText[idLobbyStatus].y();
                    found = true;
                }
            }
            if (!found) {
                Registry::getInstance().addEntity();
                LobbyStatus NewLobby(arrLobby[id].lobbyInfos.ip, arrLobby[id].lobbyInfos.port);
                arrLobbyStatus.insertBack(NewLobby);
                std::string text_t = std::string(arrLobby[id].name) + " | "
                    + std::to_string(arrLobby[id].maxNbPlayer) + " | "
                    + gameTypeToString(arrLobby[id].gameType);
                y += 5;
                Raylib::Text text(text_t, Raylib::Vector2(x, y), 2, Raylib::Red);
                arrLobbyText.insertBack(text);
            }
        }
    }

    std::vector<std::function<void(std::size_t, std::size_t)>> getLobbySystems()
    {
            return {initSelectLoby, sendListLobby, createLobbyRow};
    }
} // namespace Systems::SelectLobbySystems
