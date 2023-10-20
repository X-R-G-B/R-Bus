/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** EventsSystems implementation
*/

#include "EventsSystems.hpp"
#include "CustomTypes.hpp"
#include "Maths.hpp"
#include "NitworkClient.hpp"
#include "Raylib.hpp"
#include "Registry.hpp"
#include "SceneManager.hpp"
#include "Systems.hpp"
#include "Logger.hpp"
#include "Json.hpp"

namespace Systems {

    // BULLET SYSTEMS

    static const std::unordered_map<enum missileTypes_e, Raylib::KeyboardKey> bulletKeyMap = {
        {CLASSIC, Raylib::KeyboardKey::KB_SPACE},
        {FAST, Raylib::KeyboardKey::KB_F},
        {BOUNCE, Raylib::KeyboardKey::KB_B},
    };

    static std::size_t getClockIdFromMissileType(enum missileTypes_e type)
    {
        static std::size_t clockIdClassic = Registry::getInstance().getClock().create(true);
        static std::size_t clockIdFast    = Registry::getInstance().getClock().create(true);
        static std::size_t clockIdBounce  = Registry::getInstance().getClock().create(true);

        switch (type) {
            case CLASSIC: return clockIdClassic;
            case FAST: return clockIdFast;
            case BOUNCE: return clockIdBounce;
            default: break;
        }
        throw std::runtime_error("Unknown missile type");
    }

    static bool isBulletTimeElapsed(std::size_t clockId)
    {
        Clock &clock_ = Registry::getInstance().getClock();
        Json &json = Json::getInstance();
        std::string bulletType = "default";
        if (clockId == getClockIdFromMissileType(FAST)) {
            bulletType = "fast";
        } else if (clockId == getClockIdFromMissileType(BOUNCE)) {
            bulletType = "bounce";
        }
        nlohmann::json bulletData = json.getJsonObjectById(JsonType::BULLETS, bulletType);
        float waitTimeBullet = json.getDataFromJson<float>(bulletData, "waitTimeBullet");

        if (clock_.elapsedMillisecondsSince(clockId) < waitTimeBullet) {
            return false;
        }
        return true;
    }

    static bool checkBulletRequirements(struct Types::Missiles &missile)
    {
        Clock &clock_                           = Registry::getInstance().getClock();
        static const std::size_t waitTimeBullet = 500;
        bool isKeyPressed                       = false;

        for (auto &key : bulletKeyMap) {
            if (Raylib::isKeyDown(key.second)) {
                missile.type = key.first;
                isKeyPressed = true;
            }
        }
        if (isKeyPressed == false
            || isBulletTimeElapsed(getClockIdFromMissileType(missile.type)) == false) {
            return false;
        }
        return true;
    }

    void playerShootBullet(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        struct Types::Missiles missile = {CLASSIC};
        Registry &registry                                = Registry::getInstance();
        Clock &clock_ = registry.getClock();
        Registry::components<Types::Position> arrPosition = registry.getComponents<Types::Position>();
        Registry::components<struct health_s> arrHealth   = registry.getComponents<struct health_s>();
        std::vector<std::size_t> ids =
            registry.getEntitiesByComponents({typeid(Types::Player), typeid(Types::Position)});

        if (checkBulletRequirements(missile) == false) {
            return;
        }

        for (auto &id : ids) {
            // send bullet to server
            if (arrHealth.exist(id) && arrHealth[id].hp <= 0) {
                continue;
            }
            Nitwork::NitworkClient::getInstance().addNewBulletMsg(
                {Maths::removeIntDecimals(arrPosition[id].x), Maths::removeIntDecimals(arrPosition[id].y)},
                missile.type);
            createMissile(arrPosition[id], missile);
            clock_.restart(getClockIdFromMissileType(missile.type));
        }
    }

    // END OF BULLET SYSTEMS

    static void checkAnimRect(std::size_t id, Clock &clock_, std::size_t clockId)
    {
        Registry::components<Types::AnimRect> arrAnimRect =
            Registry::getInstance().getComponents<Types::AnimRect>();

        clock_.restart(clockId);

        if (arrAnimRect.exist(id)) {
            Types::AnimRect &anim = arrAnimRect[id];
            if (anim.currentRectList != Types::RectListType::MOVE) {
                anim.changeRectList(Types::RectListType::MOVE);
            }
        }
    }

    constexpr std::size_t elapsedBetweenMove = 20;

    void EventsSystems::playerMovement(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        Registry &registry                              = Registry::getInstance();
        Registry::components<Types::Position> arrPos    = registry.getComponents<Types::Position>();
        Registry::components<struct health_s> arrHealth = registry.getComponents<struct health_s>();
        std::vector<std::size_t> ids                    = registry.getEntitiesByComponents(
            {typeid(Types::Player), typeid(Types::Position), typeid(struct health_s)});
        Clock &clock_              = registry.getClock();
        static std::size_t clockId = clock_.create(true);

        for (auto id : ids) {
            if (clock_.elapsedMillisecondsSince(clockId) < elapsedBetweenMove || arrHealth[id].hp <= 0) {
                continue;
            }
            if (Raylib::isKeyDown(Raylib::KeyboardKey::KB_RIGHT)) {
                checkAnimRect(id, clock_, clockId);
                Maths::addNormalIntToDecimalInt(arrPos[id].x, 1);
            }
            if (Raylib::isKeyDown(Raylib::KeyboardKey::KB_LEFT)) {
                checkAnimRect(id, clock_, clockId);
                Maths::subNormalIntToDecimalInt(arrPos[id].x, 1);
            }
            if (Raylib::isKeyDown(Raylib::KeyboardKey::KB_UP)) {
                checkAnimRect(id, clock_, clockId);
                Maths::subNormalIntToDecimalInt(arrPos[id].y, 1);
            }
            if (Raylib::isKeyDown(Raylib::KeyboardKey::KB_DOWN)) {
                checkAnimRect(id, clock_, clockId);
                Maths::addNormalIntToDecimalInt(arrPos[id].y, 1);
            }
        }
    }

    void EventsSystems::changeScene(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        if (Raylib::isKeyDown(Raylib::KeyboardKey::KB_J)) {
            auto &sceneManager = Scene::SceneManager::getInstance();
            if (sceneManager.getCurrentScene() == Scene::Scene::MAIN_GAME) {
                sceneManager.changeScene(Scene::Scene::MENU);
            } else {
                sceneManager.changeScene(Scene::Scene::MAIN_GAME);
            }
        }
    }

    std::vector<std::function<void(std::size_t, std::size_t)>> EventsSystems::getEventSystems()
    {
        return {playerMovement, changeScene, playerShootBullet};
    }
} // namespace Systems
