#include "ClientNetwork.hpp"
#include <algorithm>
#include "ECSCustomTypes.hpp"
#include "Json.hpp"
#include "Maths.hpp"
#include "NitworkClient.hpp"
#include "Raylib.hpp"
#include "Registry.hpp"
#include "SceneManager.hpp"
#include "SystemManagersDirector.hpp"
#include "Systems.hpp"

namespace Systems {
    void receiveLifeUpdate(std::any &any, boost::asio::ip::udp::endpoint & /* unused */)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        auto msg                                        = std::any_cast<struct msgLifeUpdate_s>(any);
        Registry &registry                              = Registry::getInstance();
        Registry::components<struct health_s> arrHealth = registry.getComponents<struct health_s>();
        std::vector<std::size_t> ids                    = Registry::getInstance().getEntitiesByComponents(
            {typeid(struct health_s), typeid(Types::Player)});

        if (ids.empty()) {
            return;
        }
        struct health_s &life = arrHealth[ids[0]];
        if (life.hp != msg.life.hp) {
            life.hp = msg.life.hp;
        }
    }

    void receiveEnemyDeath(std::any &any, boost::asio::ip::udp::endpoint & /* unused */)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        const auto enemyDeath                      = std::any_cast<struct msgEnemyDeath_s>(any);
        Registry::components<Types::Enemy> enemies = Registry::getInstance().getComponents<Types::Enemy>();
        auto &arrHealth              = Registry::getInstance().getComponents<struct health_s>();
        std::vector<std::size_t> ids = enemies.getExistingsId();

        for (auto id : ids) {
            if (enemies[id].getConstId().id == enemyDeath.enemyId.id) {
                if (arrHealth.exist(id)) {
                    arrHealth[id].hp = 0;
                } else {
                    Logger::fatal("\n\n\n!!!! Enemy has no health component, but is alive !!!!\n\n\n");
                    Registry::getInstance().removeEntity(id);
                }
                return;
            }
        }
    }

    void receiveMissileDeath(std::any &any, boost::asio::ip::udp::endpoint & /* unused */)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        const auto missileDeath      = std::any_cast<struct msgMissileDeath_s>(any);
        auto &missiles               = Registry::getInstance().getComponents<Types::Missiles>();
        auto &arrHealth              = Registry::getInstance().getComponents<struct health_s>();
        std::vector<std::size_t> ids = missiles.getExistingsId();

        for (auto id : ids) {
            if (missiles[id].constId == missileDeath.missileId) {
                if (arrHealth.exist(id)) {
                    arrHealth[id].hp = 0;
                } else {
                    Logger::fatal("\n\n\n!!!! Missile has no health component, but is alive !!!!\n\n\n");
                    // TODO : remove missile
                    Registry::getInstance().removeEntity(id);
                }
                return;
            }
        }
    }

    void handleStartWave(std::any &any, boost::asio::ip::udp::endpoint & /* unused */)
    {
        auto &director = SystemManagersDirector::getInstance();
        std::lock_guard<std::mutex> lock(director.mutex);
        const auto wave = std::any_cast<struct msgStartWave_s>(any);
        Types::Enemy::setEnemyNb(wave.enemyNb);
        director.getSystemManager(static_cast<std::size_t>(Scene::SystemManagers::GAME))
            .addSystem(initWave);
        Logger::info("Wave started");
    }

    void receiveNewEnemy(std::any &any, boost::asio::ip::udp::endpoint & /* unused */)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        const auto newEnemy = std::any_cast<struct msgNewEnemy_s>(any);

        Logger::debug("ROLLBACK RECREATE ENEMY !!!!!");
        Types::Position pos = {
            Maths::addIntDecimals(newEnemy.enemyInfos.pos.x),
            Maths::addIntDecimals(newEnemy.enemyInfos.pos.y)};
        initEnemy(newEnemy.enemyInfos.type, pos, true, newEnemy.enemyInfos.id);
        struct health_s hp = newEnemy.enemyInfos.life;
        Registry::getInstance().getComponents<struct health_s>().insertBack(hp);
    }

    static void createNewPlayer(const struct msgCreatePlayer_s &newPlayer)
    {
        Registry &registry   = Registry::getInstance();
        auto &arrPlayer      = registry.getComponents<Types::Player>();
        auto &arrOtherPlayer = registry.getComponents<Types::OtherPlayer>();
        auto idsPlayer       = registry.getEntitiesByComponents(
            {typeid(Types::Position), typeid(Types::Player), typeid(struct health_s)});
        auto idsOtherPlayer = registry.getEntitiesByComponents(
            {typeid(Types::Position), typeid(Types::OtherPlayer), typeid(struct health_s)});
        auto player =
            std::find_if(idsPlayer.begin(), idsPlayer.end(), [&arrPlayer, &newPlayer](std::size_t id) {
                return arrPlayer[id].constId == newPlayer.playerId;
            });
        auto otherPlayers = std::find_if(
            idsOtherPlayer.begin(),
            idsOtherPlayer.end(),
            [&arrOtherPlayer, &newPlayer](std::size_t id) {
                return arrOtherPlayer[id].constId == newPlayer.playerId;
            });

        if (player != idsPlayer.end() || otherPlayers != idsOtherPlayer.end()) {
            auto id = player != idsPlayer.end() ? *player : *otherPlayers;
            Registry::getInstance().removeEntity(id);
        }
        initPlayer(newPlayer.playerId, newPlayer.pos, newPlayer.life, newPlayer.isOtherPlayer);
    }

    void receiveNewPlayer(std::any &any, boost::asio::ip::udp::endpoint & /* unused */)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        const auto newPlayer = std::any_cast<struct msgCreatePlayer_s>(any);

        if (newPlayer.isOtherPlayer) {
            Logger::info("New Ally created with id: " + std::to_string(newPlayer.playerId));
        } else {
            Logger::info("Your player id is: " + std::to_string(newPlayer.playerId));
        }
        createNewPlayer(newPlayer);
    }

    void receiveRelativePosition(std::any &any, boost::asio::ip::udp::endpoint & /* unused */)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        const struct msgPositionRelativeBroadcast_s &msg =
            std::any_cast<struct msgPositionRelativeBroadcast_s>(any);
        Types::Position position = {Maths::addIntDecimals(msg.pos.x), Maths::addIntDecimals(msg.pos.y)};
        auto &arrPos             = Registry::getInstance().getComponents<Types::Position>();
        auto &arrOtherPlayers    = Registry::getInstance().getComponents<Types::OtherPlayer>();
        auto ids                 = Registry::getInstance().getEntitiesByComponents(
            {typeid(Types::Position), typeid(Types::OtherPlayer)});
        auto otherPlayer = std::find_if(ids.begin(), ids.end(), [&arrOtherPlayers, &msg](std::size_t id) {
            return arrOtherPlayers[id].constId == msg.playerId;
        });

        if (otherPlayer == ids.end()) {
            return;
        }
        Logger::trace(
            "Other player id: " + std::to_string(msg.playerId)
            + " relative position: " + std::to_string(Maths::intToFloatConservingDecimals(position.x)) + " "
            + std::to_string(Maths::intToFloatConservingDecimals(position.y)));
        arrPos[*otherPlayer].x = Maths::additionWithTwoIntDecimals(arrPos[*otherPlayer].x, position.x);
        arrPos[*otherPlayer].y = Maths::additionWithTwoIntDecimals(arrPos[*otherPlayer].y, position.y);
    }

    void sendInitPacket(std::size_t managerId, std::size_t systemId)
    {
        if (Scene::SceneManager::getInstance().getCurrentScene() != Scene::Scene::MAIN_GAME) {
            return;
        }
        Nitwork::NitworkClient::getInstance().addInitMsg();
        SystemManagersDirector::getInstance().getSystemManager(managerId).removeSystem(systemId);
    }

    void sendPositionRelative(std::size_t /* unused */, std::size_t /* unused */)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        constexpr std::size_t delay = 10;
        static auto clockId         = Registry::getInstance().getClock().create();
        static std::pair<std::size_t, Types::Position> positionPlayerCached;
        Registry &registry = Registry::getInstance();

        if (registry.getClock().elapsedMillisecondsSince(clockId) < delay) {
            return;
        }
        auto ids = registry.getEntitiesByComponents({typeid(Types::Position), typeid(Types::Player)});
        auto arrPosition = registry.getComponents<Types::Position>();

        if (ids.empty()) {
            return;
        }
        if (positionPlayerCached.first != ids[0]) {
            positionPlayerCached.first  = ids[0];
            positionPlayerCached.second = arrPosition[ids[0]];
        }
        auto &posCached = positionPlayerCached.second;
        const auto &pos = arrPosition[positionPlayerCached.first];
        if (pos.x != posCached.x || pos.y != posCached.y) {
            registry.getClock().decreaseMilliseconds(clockId, delay);
            struct position_relative_s msg = {
                .x = static_cast<char>(
                    Maths::removeIntDecimals(Maths::subtractionWithTwoIntDecimals(pos.x, posCached.x))),
                .y = static_cast<char>(
                    Maths::removeIntDecimals(Maths::subtractionWithTwoIntDecimals(pos.y, posCached.y))),
            };
            Logger::trace(
                "send pos relative\n\n" + std::to_string(msg.x) + " " + std::to_string(msg.y) + "\n\n");
            posCached.x = pos.x;
            posCached.y = pos.y;
            Nitwork::NitworkClient::getInstance().addPositionRelativeMsg(msg);
        }
    }

    void sendPositionAbsolute(std::size_t /* unused */, std::size_t /* unused */)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        constexpr std::size_t delay                = 1;
        static auto clockId                        = Registry::getInstance().getClock().create();
        Registry &registry                         = Registry::getInstance();
        static struct position_absolute_s position = {0, 0};

        if (registry.getClock().elapsedSecondsSince(clockId) < delay) {
            return;
        }
        auto ids = registry.getEntitiesByComponents({typeid(Types::Position), typeid(Types::Player)});
        auto arrPosition = registry.getComponents<Types::Position>();

        if (ids.empty()) {
            return;
        }
        struct position_absolute_s msg = {
            .x = Maths::removeIntDecimals(arrPosition[ids[0]].x),
            .y = Maths::removeIntDecimals(arrPosition[ids[0]].y),
        };
        if (position.x == msg.x && position.y == msg.y) {
            return;
        }
        registry.getClock().decreaseSeconds(clockId, delay);
        position = msg;
        Nitwork::NitworkClient::getInstance().addPositionAbsoluteMsg(msg);
    }

    void receiveNewBullet(std::any &any, boost::asio::ip::udp::endpoint & /* unused*/)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        auto &missiles = Registry::getInstance().getComponents<Types::Missiles>();
        auto &health   = Registry::getInstance().getComponents<struct health_s>();

        const struct msgNewBullet_s &msgNewBullet = std::any_cast<struct msgNewBullet_s>(any);

        struct Types::Position position = {
            Maths::addIntDecimals(msgNewBullet.pos.x),
            Maths::addIntDecimals(msgNewBullet.pos.y),
        };
        struct Types::Missiles missileType = {static_cast<missileTypes_e>(msgNewBullet.missileType)};
        auto id                            = Systems::createMissile(position, missileType);
        if (!missiles.exist(id) || !health.exist(id)) {
            Logger::error("Error: missile not created");
            return;
        }
        missiles[id].constId               = msgNewBullet.id;
        health[id].hp                      = msgNewBullet.life;
    }

    void receiveBroadcastAbsolutePosition(std::any &any, boost::asio::ip::udp::endpoint & /* unused*/)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);

        const struct msgPositionAbsoluteBroadcast_s &msg =
            std::any_cast<struct msgPositionAbsoluteBroadcast_s>(any);
        struct Types::Position position = {
            Maths::addIntDecimals(msg.pos.x),
            Maths::addIntDecimals(msg.pos.y),
        };
        auto &arrPos          = Registry::getInstance().getComponents<Types::Position>();
        auto &arrOtherPlayers = Registry::getInstance().getComponents<Types::OtherPlayer>();
        auto ids              = Registry::getInstance().getEntitiesByComponents(
            {typeid(Types::Position), typeid(Types::OtherPlayer)});
        auto otherPlayer = std::find_if(ids.begin(), ids.end(), [&arrOtherPlayers, &msg](std::size_t id) {
            return arrOtherPlayers[id].constId == msg.playerId;
        });

        if (otherPlayer == ids.end()) {
            return;
        }
        arrPos[*otherPlayer] = position;
    }

    void receivePlayerDeath(std::any &any, boost::asio::ip::udp::endpoint & /* unused */)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        const auto playerDeath = std::any_cast<struct msgPlayerDeath_s>(any);
        auto &arrPlayer        = Registry::getInstance().getComponents<Types::Player>();
        auto &arrOtherPlayers  = Registry::getInstance().getComponents<Types::OtherPlayer>();
        auto &arrHealth        = Registry::getInstance().getComponents<struct health_s>();
        auto playersIds        = Registry::getInstance().getEntitiesByComponents(
            {typeid(Types::Player), typeid(struct health_s)});
        auto otherPlayersIds = Registry::getInstance().getEntitiesByComponents(
            {typeid(Types::OtherPlayer), typeid(struct health_s)});

        for (auto &id : playersIds) {
            if (arrPlayer[id].constId == playerDeath.playerId) {
                arrHealth[id].hp = 0;
                return;
            }
        }
        for (auto &id : otherPlayersIds) {
            if (arrOtherPlayers[id].constId == playerDeath.playerId) {
                Logger::fatal("Other player death: " + std::to_string(playerDeath.playerId));
                arrHealth[id].hp = 0;
                return;
            }
        }
    }

    void handleNewLobbyMsg(std::any &data, boost::asio::ip::udp::endpoint & /* unused */)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        const struct msgLobbyInfo_s &newLobby = std::any_cast<struct msgLobbyInfo_s>(data);
        auto &registry                        = Registry::getInstance();
        auto &arrLobby                        = registry.getComponents<struct lobby_s>();
        auto lobbyIds                         = arrLobby.getExistingsId();
        auto isAlreadyPresent =
            std::find_if(lobbyIds.begin(), lobbyIds.end(), [&arrLobby, &newLobby](std::size_t id) {
                return std::string(arrLobby[id].lobbyInfos.ip) == std::string(newLobby.lobby.lobbyInfos.ip)
                    && arrLobby[id].lobbyInfos.port == newLobby.lobby.lobbyInfos.port;
            });

        if (isAlreadyPresent != lobbyIds.end()) {
            return;
        }
        registry.addEntity();
        struct lobby_s lobbyToAdd = {newLobby.lobby};
        arrLobby.insertBack(lobbyToAdd);
        Logger::info(
            "New lobby added: " + std::string(newLobby.lobby.lobbyInfos.ip) + ":"
            + std::to_string(newLobby.lobby.lobbyInfos.port));
    }

    void receiveMsgConnectMainServerResp(bool isOk)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);

        if (!isOk) {
            // TODO: Show error on GUI
            Logger::error("Server Not OK!");
            return;
        }
        Scene::SceneManager::getInstance().changeScene(Scene::Scene::SELECT_LOBBY);
        Logger::info("Server OK!");
    }

    void receiveConnectMainServerResp(std::any &data, boost::asio::ip::udp::endpoint & /* unused */)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        const struct msgConnectMainServerResp_s &msg =
            std::any_cast<struct msgConnectMainServerResp_s>(data);

        if (msg.magick != MAGICK_CONNECT_MAIN_SERVER_RESP) {
            Logger::error("MAGICK_CONNECT_MAIN_SERVER_RESP is not the same");
            return;
        }
        Scene::SceneManager::getInstance().changeScene(Scene::Scene::SELECT_LOBBY);
        Logger::info("Server OK!");
    }

    void receiveConnectLobbyResp(std::any &data, boost::asio::ip::udp::endpoint & /* unused */)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        const struct msgConnectLobbyResp_s &msg = std::any_cast<struct msgConnectLobbyResp_s>(data);

        if (msg.magick != MAGICK_CONNECT_LOBBY_RESP) {
            Logger::error("MAGICK_CONNECT_LOBBY_RESP is not the same");
            return;
        }
        if (msg.isOk == 1) {
            Scene::SceneManager::getInstance().changeScene(Scene::Scene::MAIN_GAME);
        } else {
            Logger::info("Lobby is full");
        }
    }

    void quitGame(std::size_t /* unused */, std::size_t /* unused */)
    {
        if (Raylib::isKeyPressed(Raylib::KeyboardKey::KB_ESCAPE)) {
            if (Scene::SceneManager::getInstance().getCurrentScene() == Scene::Scene::MAIN_GAME) {
                Nitwork::NitworkClient::getInstance().disconnectLobby();
                Scene::SceneManager::getInstance().changeScene(Scene::Scene::SELECT_LOBBY);
            }
        }
    }

    std::vector<std::function<void(std::size_t, std::size_t)>> getNetworkSystems()
    {
        return {quitGame, sendInitPacket, sendPositionRelative, sendPositionAbsolute};
    }
} // namespace Systems
