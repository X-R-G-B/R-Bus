/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** SelectLobby
*/

#include "SelectLobbySystems.hpp"
#include <algorithm>
#include <string>
#include "B-luga-graphics/GraphicsCustomTypes.hpp"
#include "B-luga-physics/ECSCustomTypes.hpp"
#include "B-luga/Maths/Maths.hpp"
#include "B-luga/SceneManager.hpp"
#include "B-luga/SystemManagers/SystemManagersDirector.hpp"
#include "Menu.hpp"
#include "MessageTypes.h"
#include "NitworkClient.hpp"
#include "Parallax.hpp"
#include "ResourcesManager.hpp"
#include "init.hpp"

namespace Systems::SelectLobbySystems {

    std::size_t LobbyStatus::pageNbr = 1;
    std::size_t LobbyStatus::pageMax = 1;

    LobbyStatus::LobbyStatus(const std::string &ip, n_port_t port) : ip(ip), port(port)
    {
    }

    void initSelectLoby(std::size_t managerId, std::size_t systemId)
    {
        if (Scene::SceneManager::getInstance().getCurrentScene() != SELECT_LOBBY) {
            SystemManagersDirector::getInstance().getSystemManager(managerId).removeSystem(systemId);
            return;
        }
        try {
            Parallax::initParalax();
            nlohmann::json jsonData = Json::getInstance().getDataByJsonType<nlohmann::json>(
                ResourcesManager::getPathByJsonType(JsonType::SELECT_LOBBY),
                "lobbyMenu");
            ::Menu::MenuBuilder::getInstance().initMenuSceneEntity(
                Json::getInstance().getDatasFromList(jsonData));
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
        auto &arrLobbyText = Registry::getInstance().getComponents<Raylib::TextShared>();
        auto &arrPosition  = Registry::getInstance().getComponents<Types::Position>();
        int offset         = 1000;

        if (Scene::SceneManager::getInstance().getCurrentScene() != SELECT_LOBBY) {
            SystemManagersDirector::getInstance().getSystemManager(managerId).removeSystem(systemId);
            return;
        }
        try {
            for (std::size_t i = 0; i < 5; i++) {
                nlohmann::json lobbyBox = Json::getInstance().getDataByJsonType<nlohmann::json>(
                    ResourcesManager::getPathByJsonType(JsonType::SELECT_LOBBY),
                    "inputBoxes");
                std::size_t id = ::Menu::MenuBuilder::getInstance().initMenuEntity(lobbyBox);
                if (arrPosition.exist(id)) {
                    arrPosition[id].y += offset * static_cast<int>(i);
                }
                if (arrLobbyText.exist(id)) {
                    Raylib::Vector2 pos(
                        arrLobbyText[id]->getPosition().x,
                        arrLobbyText[id]->getPosition().y);
                    pos.y += Maths::intToFloatConservingDecimals(offset) * i;
                    arrLobbyText[id]->setPosition(pos);
                }
                LobbyStatus newLobby;
                Registry::getInstance().getComponents<LobbyStatus>().insertBack(newLobby);
            }
        } catch (std::runtime_error &err) {
            Logger::error(
                "Counldn't load menu correctly, verify your json data : " + std::string(err.what()));
        }
        SystemManagersDirector::getInstance().getSystemManager(managerId).removeSystem(systemId);
    }

    static void updateBox(
        const std::size_t &index,
        const std::size_t &lobbyStatusId,
        const std::vector<std::size_t> &ids,
        Registry::components<LobbyStatus> &arrLobbyStatus)
    {
        auto &arrLobby     = Registry::getInstance().getComponents<struct lobby_s>();
        auto &arrLobbyText = Registry::getInstance().getComponents<Raylib::TextShared>();

        std::string name(arrLobby[ids[index]].name);
        if (name.size() > 15) {
            name = name.substr(0, 15);
            name += "...";
        }
        std::string text_t = name + " | " + std::to_string(arrLobby[ids[index]].maxNbPlayer) + " | "
            + gameTypeToString(arrLobby[ids[index]].gameType);
        arrLobbyStatus[lobbyStatusId].ip   = std::string(arrLobby[ids[index]].lobbyInfos.ip);
        arrLobbyStatus[lobbyStatusId].port = arrLobby[ids[index]].lobbyInfos.port;
        Raylib::TextShared text            = Raylib::Text::fromText(text_t);
        if (arrLobbyText.exist(lobbyStatusId)) {
            arrLobbyText[lobbyStatusId]->setCurrentText(text_t);
        }
    }

    static void updateMaxPage()
    {
        auto ids = Registry::getInstance().getEntitiesByComponents({typeid(struct lobby_s)});

        LobbyStatus::pageMax = 1;
        LobbyStatus::pageMax += (ids.size() / 5);
    }

    void updateLobbyRow(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        static std::size_t clockId = Registry::getInstance().getClock().create();

        if (Registry::getInstance().getClock().elapsedMillisecondsSince(clockId) < 5000) {
            return;
        }
        Registry::getInstance().getClock().decreaseSeconds(clockId, 1);
        // list of all lobby
        auto idsLobbyStatus = Registry::getInstance().getEntitiesByComponents(
            {typeid(LobbyStatus), typeid(Raylib::TextShared)});
        auto idsClickableLobbys = Registry::getInstance().getEntitiesByComponents(
            {typeid(LobbyStatus), typeid(Raylib::TextShared), typeid(Types::InputBox)});
        auto ids             = Registry::getInstance().getEntitiesByComponents({typeid(struct lobby_s)});
        auto &arrLobbyStatus = Registry::getInstance().getComponents<LobbyStatus>();
        auto &arrLobbyText   = Registry::getInstance().getComponents<Raylib::TextShared>();
        static std::size_t nbrOfIt = 0;
        std::size_t pageNbr        = LobbyStatus::pageNbr;

        nbrOfIt = pageNbr * 5 - 5;
        updateMaxPage();
        for (; nbrOfIt < pageNbr * 5 && nbrOfIt < ids.size();) {
            for (auto &id : idsLobbyStatus) {
                if (nbrOfIt < ids.size()) {
                    updateBox(nbrOfIt, id, ids, arrLobbyStatus);
                    nbrOfIt++;
                } else {
                    arrLobbyStatus[id].ip   = "";
                    arrLobbyStatus[id].port = -1;
                    arrLobbyText[id]->setCurrentText("");
                }
            }
        }
    }

    std::vector<std::function<void(std::size_t, std::size_t)>> getLobbySystems()
    {
        return {initSelectLoby, initLobbyRow, updateLobbyRow, sendListLobby};
    }
} // namespace Systems::SelectLobbySystems
