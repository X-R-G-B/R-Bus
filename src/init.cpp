#include "init.hpp"
#include "SceneManager.hpp"
#include "SystemManagersDirector.hpp"
#include "ECSSystems.hpp"
#include "GameSystems.hpp"

#ifdef GRAPHICS
    #include "GraphicsSystems.hpp"
#endif
#ifdef CLIENT
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

void initScenes(bool client)
{
    auto &director = Systems::SystemManagersDirector::getInstance();
    std::map<SystemManagers, std::function<std::vector<std::function<void(std::size_t, std::size_t)>>()>> systems = {
        {SystemManagers::ECSSYSTEMS, &Systems::getECSSystems},
        {SystemManagers::GAME, &Systems::getGameSystems},
    };
    if (client) {
#ifdef CLIENT
        systems.insert({SystemManagers::EVENTS, &Systems::EventsSystems::getEventsSystems});
        systems.insert({SystemManagers::CLIENTNETWORK, &Systems::getNetworkSystems});
#endif
#ifdef GRAPHICS
        systems.insert({SystemManagers::GRAPHICS, &Systems::GraphicsSystems::getGraphicsSystems});
#endif
    }

    std::lock_guard<std::mutex> lock(director.mutex);
    for (auto sys : systems) {
        director.addSystemManager(static_cast<std::size_t>(sys.first), sys.second());
    }
    auto &sceneManager = Scene::SceneManager::getInstance();
    std::vector<std::size_t> scene;
    if (client) {
        scene = enumListTosizet({SystemManagers::EVENTS, SystemManagers::ECSSYSTEMS, SystemManagers::GAME, SystemManagers::CLIENTNETWORK, SystemManagers::GRAPHICS});
    } else {
        scene = enumListTosizet({SystemManagers::ECSSYSTEMS, SystemManagers::GAME});
    }
    sceneManager.setScenes({scene, scene});
}
