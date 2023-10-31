#include "init.hpp"
#include "B-luga/SceneManager.hpp"
#include "B-luga/SystemManagers/SystemManagersDirector.hpp"
#include "B-luga-physics/ECSSystems.hpp"
#include "B-luga/GameSystems.hpp"
#include "B-luga/PluginHandler.hpp"

#ifdef CLIENT
    #include "B-luga-graphics/GraphicsSystems.hpp"
    #include "ClientNetwork.hpp"
    #include "EventsSystems.hpp"
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
    auto &director = Systems::SystemManagersDirector::getInstance();
    auto ecsPlugin = Systems::ECSPlugin();
    auto gamePlugin = Systems::GamePlugin();
#ifdef CLIENT
    auto graphicsPlugin = Systems::GraphicsSystems::GraphicsPlugin();
    std::lock_guard<std::mutex> lock(director.mutex);
    PluginHandler::addNewPlugin(graphicsPlugin, SystemManagers::GRAPHICS);
#endif
    PluginHandler::addNewPlugin(ecsPlugin, SystemManagers::ECSSYSTEMS);
    PluginHandler::addNewPlugin(gamePlugin, SystemManagers::GAME);

    std::map<SystemManagers, std::function<std::vector<std::function<void(std::size_t, std::size_t)>>()>> systems = {
#ifdef CLIENT
        {SystemManagers::EVENTS, &Systems::EventsSystems::getEventsSystems},
        {SystemManagers::CLIENTNETWORK, &Systems::getNetworkSystems},
#endif
    };

    for (auto sys : systems) {
        director.addSystemManager(static_cast<std::size_t>(sys.first), sys.second());
    }
    auto &sceneManager = Scene::SceneManager::getInstance();
    std::vector<std::size_t> scene;
#ifdef CLIENT
        scene = enumListTosizet({SystemManagers::EVENTS, SystemManagers::ECSSYSTEMS, SystemManagers::GAME, SystemManagers::CLIENTNETWORK, SystemManagers::GRAPHICS});
#else
        scene = enumListTosizet({SystemManagers::ECSSYSTEMS, SystemManagers::GAME});
#endif
    sceneManager.setScenes({scene, scene});
}
