#include "init.hpp"
#include "B-luga-physics/ECSSystems.hpp"
#include "B-luga/PluginHandler.hpp"
#include "B-luga/SceneManager.hpp"
#include "B-luga/SystemManagers/SystemManagersDirector.hpp"
#include "GameSystems.hpp"

#ifdef CLIENT
    #include "B-luga-graphics/GraphicsSystems.hpp"
    #include "ClientNetwork.hpp"
    #include "EventsSystems.hpp"
    #include "MenuSystems.hpp"
    #include "SelectLobbySystems.hpp"
#endif

static std::vector<std::size_t> enumListTosizet(std::vector<SystemManagers> list)
{
    std::vector<std::size_t> res;
    for (auto &item : list) {
        res.push_back(static_cast<std::size_t>(item));
    }
    return res;
}

void initScenes()
{
    auto &director  = Systems::SystemManagersDirector::getInstance();
    auto ecsPlugin  = Systems::ECSPlugin();
    auto gamePlugin = Systems::GamePlugin();
#ifdef CLIENT
    auto graphicsPlugin = Systems::GraphicsSystems::GraphicsPlugin();
    std::lock_guard<std::mutex> lock(director.mutex);
    PluginHandler::addNewPlugin(graphicsPlugin, SystemManagers::GRAPHICS);
#endif
    PluginHandler::addNewPlugin(ecsPlugin, SystemManagers::ECSSYSTEMS);
    PluginHandler::addNewPlugin(gamePlugin, SystemManagers::GAME_LOGIC);

    std::map<SystemManagers, std::function<std::vector<std::function<void(std::size_t, std::size_t)>>()>>
        systems = {
#ifdef CLIENT
            {SystemManagers::EVENTS,        &Systems::EventsSystems::getEventsSystems    },
            {SystemManagers::CLIENTNETWORK, &Systems::getNetworkSystems                  },
            {SystemManagers::MENU_SYSTEMS,  &Systems::Menu::getMenuSystems               },
            {SystemManagers::NETWORK_MENU,  &Systems::SelectLobbySystems::getLobbySystems},
#endif
    };

    for (auto sys : systems) {
        director.addSystemManager(static_cast<std::size_t>(sys.first), sys.second());
    }
    auto &sceneManager = Scene::SceneManager::getInstance();
#ifdef CLIENT
    auto menu = enumListTosizet(
        {SystemManagers::GRAPHICS, SystemManagers::MENU_SYSTEMS, SystemManagers::ECSSYSTEMS});
    auto selectLobby = enumListTosizet(
        {SystemManagers::MENU_SYSTEMS,
         SystemManagers::NETWORK_MENU,
         SystemManagers::CLIENTNETWORK,
         SystemManagers::ECSSYSTEMS,
         SystemManagers::GRAPHICS});
    auto mainScene = enumListTosizet(
        {SystemManagers::EVENTS,
         SystemManagers::ECSSYSTEMS,
         SystemManagers::GAME_LOGIC,
         SystemManagers::CLIENTNETWORK,
         SystemManagers::GRAPHICS,
         SystemManagers::MENU_SYSTEMS});
    sceneManager.setScenes({menu, selectLobby, mainScene});
#else
    auto scene = enumListTosizet({SystemManagers::ECSSYSTEMS, SystemManagers::GAME_LOGIC});
    sceneManager.setScenes({scene});
#endif
}
