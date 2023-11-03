/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** SelectLobby
*/

#include "SelectLobbySystems.hpp"
#include <algorithm>
#include <string>
#include "B-luga-physics/ECSCustomTypes.hpp"
#include "B-luga/SceneManager.hpp"
#include "B-luga/SystemManagers/SystemManagersDirector.hpp"
#include "B-luga-graphics/GraphicsCustomTypes.hpp"
#include "ResourcesManager.hpp"
#include "init.hpp"
#include "Menu.hpp"
#include "MessageTypes.h"
#include "NitworkClient.hpp"

namespace Systems::SelectLobbySystems {

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

    void initSelectLoby(std::size_t managerId, std::size_t systemId)
    {
        if (Scene::SceneManager::getInstance().getCurrentScene() != Scenes::SELECT_LOBBY) {
            SystemManagersDirector::getInstance().getSystemManager(managerId).removeSystem(systemId);
            return;
        }
        try {
            nlohmann::json createLobbyNormalButton =
                Json::getInstance().getDataByVector(ResourcesManager::getPathByJsonType(JsonType::SELECT_LOBBY), {"menu", "gametype_normal"});
            nlohmann::json lobbyName =
                Json::getInstance().getDataByVector(ResourcesManager::getPathByJsonType(JsonType::SELECT_LOBBY), {"menu", "name"});
            nlohmann::json maxNbPlayer =
                Json::getInstance().getDataByVector(ResourcesManager::getPathByJsonType(JsonType::SELECT_LOBBY), {"menu", "maxNb"});
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
                auto text = Raylib::Text::fromText(text_t, Raylib::Vector2(x, y), 2, Raylib::Red);
                arrLobbyText.insertBack(*text);
            }
        }
    }

    std::vector<std::function<void(std::size_t, std::size_t)>> getLobbySystems()
    {
        return {initSelectLoby, sendListLobby, createLobbyRow};
    }
} // namespace Systems::SelectLobbySystems
