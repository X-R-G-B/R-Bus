/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** ButtonCallbacks
*/

#pragma once

#include <functional>
#include <unordered_map>
#include "B-luga/Json.hpp"

namespace Menu {
    namespace Callback {

        enum CallbackType {
            DEFAULT_CALLBACK,
            INIT_CONNECTION,
            CONNECT,
            CREATE,
            GO_BACK,
            GO_NEXT,
            GO_SELECT_LOBBY,
            CREATE_NORMAL,
            SEND_READY,
            GO_CREATE_SERVER,
            CREATE_SERVER,
            GO_MENU,
            CHANGE_PARALLAX,
            EXIT_GAME
        };

        NLOHMANN_JSON_SERIALIZE_ENUM(
            CallbackType,
            {
                {DEFAULT_CALLBACK, nullptr                           },
                {INIT_CONNECTION,  "initConnection"                  },
                {CONNECT,          "connectLobbySelected"            },
                {CREATE,           "onButtonGoToCreateLobbyClicked"  },
                {GO_BACK,          "goBackPage"                      },
                {GO_NEXT,          "goNextPage"                      },
                {GO_SELECT_LOBBY,  "goToSelectLobby"                 },
                {CREATE_NORMAL,    "onButtonCreateLobbyNormalClicked"},
                {SEND_READY,       "sendReadyPacket"                 },
                {GO_CREATE_SERVER, "goCreateServer"                  },
                {CREATE_SERVER,    "createServer"                    },
                {GO_MENU,          "goToMenu"                        },
                {CHANGE_PARALLAX,  "changeParallax"                  },
                {EXIT_GAME,  "exitGame"                  },
        });

        void initConnection();

        void defaultCallBack();

        void connectLobbySelected();

        void goBackPage();

        void goNextPage();

        void onButtonGotoCreateLobbyClicked();

        void gotToSelectLobby();

        void onButtonCreateLobbyNormalClicked();

        void sendReadyPacket();

        void goCreateServer();

        void goMenu();

        void createServer();

        void changeParallax();

        void exitGame();

        const std::unordered_map<CallbackType, std::function<void()>> callbacks = {
            {CallbackType::DEFAULT_CALLBACK, &defaultCallBack},
            {CallbackType::INIT_CONNECTION, &initConnection},
            {CallbackType::CONNECT, &connectLobbySelected},
            {CallbackType::CREATE, &onButtonGotoCreateLobbyClicked},
            {CallbackType::GO_SELECT_LOBBY, &gotToSelectLobby},
            {CallbackType::GO_BACK, &goBackPage},
            {CallbackType::GO_NEXT, &goNextPage},
            {CallbackType::CREATE_NORMAL, &onButtonCreateLobbyNormalClicked},
            {CallbackType::SEND_READY, &sendReadyPacket},
            {CallbackType::GO_CREATE_SERVER, &goCreateServer},
            {CallbackType::CREATE_SERVER, &createServer},
            {CallbackType::EXIT_GAME, &exitGame},
            {
             CallbackType::GO_MENU,
             &goMenu,
             },
            {
             CallbackType::CHANGE_PARALLAX,
             &changeParallax,
             },
        };
    } // namespace Callback
} // namespace Menu
