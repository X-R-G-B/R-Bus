#include "ClientNetwork.hpp"
#include "ECSCustomTypes.hpp"
#include "NitworkClient.hpp"
#include "Registry.hpp"

namespace Systems {
    void receiveLifeUpdate(std::any &any, boost::asio::ip::udp::endpoint &endpoint)
    {
        struct msgLifeUpdate_s msg                      = std::any_cast<struct msgLifeUpdate_s>(any);
        Registry &registry                              = Registry::getInstance();
        Registry::components<struct health_s> arrHealth = registry.getComponents<struct health_s>();
        std::vector<std::size_t> ids =
            registry.getEntitiesByComponents({typeid(struct health_s), typeid(Types::Player)});

        for (auto id : ids) {
            struct health_s &life = arrHealth[id];
            if (life.hp != msg.life.hp) {
                life.hp = msg.life.hp;
            }
        }
    }
} // namespace Systems
