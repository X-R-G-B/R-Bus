#include "ClientNetwork.hpp"
#include "ECSCustomTypes.hpp"
#include "NitworkClient.hpp"
#include "Registry.hpp"

namespace Systems {
    static bool getPlayerId(std::size_t &id)
    {
        std::vector<std::size_t> ids = Registry::getInstance().getEntitiesByComponents(
            {typeid(struct health_s), typeid(Types::Player)});
        if (ids.empty()) {
            return false;
        }
        id = ids[0];
        return true;
    }

    void receiveLifeUpdate(std::any &any, boost::asio::ip::udp::endpoint &)
    {
        struct msgLifeUpdate_s msg                      = std::any_cast<struct msgLifeUpdate_s>(any);
        Registry &registry                              = Registry::getInstance();
        Registry::components<struct health_s> arrHealth = registry.getComponents<struct health_s>();
        std::size_t id                                  = 0;

        if (!getPlayerId(id)) {
            return;
        }
        struct health_s &life = arrHealth[id];
        if (life.hp != msg.life.hp) {
            life.hp = msg.life.hp;
        }
    }

    void receiveEnemyDeath(std::any &any, boost::asio::ip::udp::endpoint &)
    {
        struct msgEnemyDeath_s enemyDeath          = std::any_cast<struct msgEnemyDeath_s>(any);
        Registry::components<Types::Enemy> enemies = Registry::getInstance().getComponents<Types::Enemy>();
        std::size_t id                             = 0;

        if (!getPlayerId(id)) {
            return;
        }
        if (enemies[id].constId.value == enemyDeath.enemyId.value) {
            Registry::getInstance().removeEntity(id);
            return;
        }
    }
} // namespace Systems
