#include "ECSCustomTypes.hpp"
#include "NitworkClient.hpp"
#include "ClientNetwork.hpp"
#include "Registry.hpp"

namespace Systems {
    void receiveLifeUpdate(std::any &any, boost::asio::ip::udp::endpoint &endpoint)
    {
        struct msgLifeUpdate_s msg = std::any_cast<struct msgLifeUpdate_s>(any);
        Registry &registry = Registry::getInstance();
        Registry::components<struct health_s> arrHealth = registry.getComponents<struct health_s>();
        std::vector<std::size_t> ids = registry.getEntitiesByComponents();
    }
}
