#include "IPlugin.hpp"
#include "SystemManagersDirector.hpp"

class PluginHandler
{
    public:
        static void addNewPlugin(IPlugin &plugin, std::size_t managerId = -1)
        {
            plugin.initPlugin();
            if (static_cast<int>(managerId) > -1) {
                Systems::SystemManagersDirector::getInstance().addSystemManager(managerId, plugin.getSystems());
            }
        }
};
