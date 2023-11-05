#pragma once

enum SystemManagers {
    EVENTS,
    ECSSYSTEMS,
    GAME_LOGIC,
    CLIENTNETWORK,
    GRAPHICS,
    MENU_SYSTEMS,
    NETWORK_MENU,
    NETWORK_CREATE_LOBBY
};

enum Scenes { MENU, SELECT_LOBBY, CREATE_LOBBY_SCENE, GAME };

void initScenes();
