#pragma once

enum SystemManagers { EVENTS, ECSSYSTEMS, GAME_LOGIC, CLIENTNETWORK, GRAPHICS, MENU_SYSTEMS, NETWORK_MENU, NETWORK_CREATE_LOBBY, NETWORK_CREATE_SERVER};

enum Scenes { MENU, CREATE_SERVER_SCENE, SELECT_LOBBY, CREATE_LOBBY_SCENE, GAME};

void initScenes();
