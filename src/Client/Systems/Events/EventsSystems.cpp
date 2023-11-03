/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** EventsSystems implementation
*/

#include "EventsSystems.hpp"
#include "CustomTypes.hpp"
#include "Json.hpp"
#include "Logger.hpp"
#include "Maths.hpp"
#include "NitworkClient.hpp"
#include "Raylib.hpp"
#include "Registry.hpp"
#include "SceneManager.hpp"
#include "Systems.hpp"

namespace Systems {

    // BULLET SYSTEMS
    static const std::unordered_map<enum missileTypes_e, Raylib::KeyboardKey> bulletKeyMap = {
        {CLASSIC,   Raylib::KeyboardKey::KB_SPACE},
        {FAST,      Raylib::KeyboardKey::KB_C    },
        {BOUNCE,    Raylib::KeyboardKey::KB_V    },
        {PERFORANT, Raylib::KeyboardKey::KB_B    },
    };

    static std::size_t getClockIdFromMissileType(enum missileTypes_e type)
    {
        static std::size_t clockIdClassic   = Registry::getInstance().getClock().create(true);
        static std::size_t clockIdFast      = Registry::getInstance().getClock().create(true);
        static std::size_t clockIdBounce    = Registry::getInstance().getClock().create(true);
        static std::size_t clockIdPerforant = Registry::getInstance().getClock().create(true);

        switch (type) {
            case CLASSIC: return clockIdClassic;
            case FAST: return clockIdFast;
            case BOUNCE: return clockIdBounce;
            case PERFORANT: return clockIdPerforant;
            default: break;
        }
        throw std::runtime_error("Unknown missile type");
    }

    static bool isBulletTimeElapsed(std::size_t clockId)
    {
        Clock &clock_          = Registry::getInstance().getClock();
        Json &json             = Json::getInstance();
        std::string bulletType = "classic";
        if (clockId == getClockIdFromMissileType(FAST)) {
            bulletType = "fast";
        } else if (clockId == getClockIdFromMissileType(BOUNCE)) {
            bulletType = "bounce";
        } else if (clockId == getClockIdFromMissileType(PERFORANT)) {
            bulletType = "perforant";
        }
        nlohmann::json bulletData = json.getJsonObjectById<std::string>(JsonType::BULLETS, bulletType, "bullets");
        float waitTimeBullet      = json.getDataFromJson<float>(bulletData, "waitTimeBullet");

        if (clock_.elapsedMillisecondsSince(clockId) < waitTimeBullet) {
            return false;
        }
        return true;
    }

    static bool checkBulletRequirements(struct Types::Missiles &missile)
    {
        bool isKeyPressed = false;

        for (auto &key : bulletKeyMap) {
            if (Raylib::isKeyDown(key.second)) {
                missile.type = key.first;
                isKeyPressed = true;
                break;
            }
        }
        if (isKeyPressed == false
            || isBulletTimeElapsed(getClockIdFromMissileType(missile.type)) == false) {
            return false;
        }
        return true;
    }

    static Types::Position
    adjustMissilePosition(Types::Position &pos, missileTypes_e typeOfMissile, std::size_t id)
    {
        Types::Position newPos                            = pos;
        Json &json                                        = Json::getInstance();
        Registry &registry                                = Registry::getInstance();
        Registry::components<Types::CollisionRect> arrCol = registry.getComponents<Types::CollisionRect>();
//TODO
        if (arrCol.exist(id)) {
            Types::CollisionRect &col = arrCol[id];
            float posX                = Maths::intToFloatConservingDecimals(pos.x)
                + (Maths::intToFloatConservingDecimals(col.width) / 2.F);
            float posY = Maths::intToFloatConservingDecimals(pos.y)
                + (Maths::intToFloatConservingDecimals(col.height) / 2.F);
            newPos.x = Maths::floatToIntConservingDecimals(posX);
            newPos.y = Maths::floatToIntConservingDecimals(posY);
        }
        nlohmann::json bulletData =
            json.getJsonObjectById<std::string>(JsonType::BULLETS, getMissileId(typeOfMissile), "bullets");
        Types::CollisionRect collisionRect =
            json.getDataFromJson<Types::CollisionRect>(bulletData, "collisionRect");
        int halfSprite = Maths::divisionWithTwoIntDecimals(collisionRect.width, 200);
        newPos.y       = Maths::subtractionWithTwoIntDecimals(newPos.y, halfSprite);
        newPos.x       = pos.x;
        return newPos;
    }

    void playerShootBullet(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        struct Types::Missiles missile                    = {CLASSIC};
        Registry &registry                                = Registry::getInstance();
        Clock &clock_                                     = registry.getClock();
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
            Types::Position pos = adjustMissilePosition(arrPosition[id], missile.type, id);
            Nitwork::NitworkClient::getInstance().addNewBulletMsg(
                {Maths::removeIntDecimals(pos.x), Maths::removeIntDecimals(pos.y)},
                missile.type);
            createMissile(pos, missile);
            clock_.restart(getClockIdFromMissileType(missile.type));
        }
    }

    // END OF BULLET SYSTEMS

    static void
    checkAnimRect(std::size_t id, Clock &clock_, std::size_t clockId, Types::Direction direction)
    {
        Registry::components<Types::AnimRect> arrAnimRect =
            Registry::getInstance().getComponents<Types::AnimRect>();

        clock_.restart(clockId);

        if (arrAnimRect.exist(id)) {
            Types::AnimRect &anim = arrAnimRect[id];
            anim.changeRectList(Types::RectListType::MOVE);
            anim.changeDirection(direction);
        }
    }

    constexpr std::size_t elapsedBetweenMove = 15;

    void EventsSystems::playerMovement(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        bool isKeyPressed = false;
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
                checkAnimRect(id, clock_, clockId, Types::Direction::RIGHT);
                Maths::addFloatToDecimalInt(arrPos[id].x, 1.F);
                isKeyPressed = true;
            }
            if (Raylib::isKeyDown(Raylib::KeyboardKey::KB_LEFT)) {
                checkAnimRect(id, clock_, clockId, Types::Direction::LEFT);
                Maths::subFloatToDecimalInt(arrPos[id].x, 1.F);
                isKeyPressed = true;
            }
            if (Raylib::isKeyDown(Raylib::KeyboardKey::KB_UP)) {
                checkAnimRect(id, clock_, clockId, Types::Direction::UP);
                Maths::subFloatToDecimalInt(arrPos[id].y, 1.F);
                isKeyPressed = true;
            }
            if (Raylib::isKeyDown(Raylib::KeyboardKey::KB_DOWN)) {
                checkAnimRect(id, clock_, clockId, Types::Direction::DOWN);
                Maths::addFloatToDecimalInt(arrPos[id].y, 1.F);
                isKeyPressed = true;
            }
            if (isKeyPressed == false) {
                checkAnimRect(id, clock_, clockId, Types::Direction::NONE);
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

    static bool isGameWin()
    {
        Registry &registry = Registry::getInstance();
        std::vector<std::size_t> idsPlayer =
            registry.getEntitiesByComponents({typeid(Types::Player)});
        std::vector<std::size_t> idsOtherPlayer =
            registry.getEntitiesByComponents({typeid(Types::OtherPlayer)});

        if (idsPlayer.empty() == true && idsOtherPlayer.empty() == true) {
            return false;
        }
        return true;
    }

    static void modifEndGameText(const std::string &endGameMessage)
    {
        bool found                            = false;
        const Raylib::Vector2 pos             = {0, 2};
        constexpr std::size_t fontSize        = 2;
        const std::string textKeywordWaveEnd  = "WaveText";
        const std::string textKeyWordGameEnd  = "endGameText";

        std::vector<std::size_t> ids =
            Registry::getInstance().getEntitiesByComponents({typeid(Raylib::Text)});
        auto &textArray            = Registry::getInstance().getComponents<Raylib::Text>();

        for (auto &id : ids) {
            if (textArray[id].getKeyword() == textKeyWordGameEnd) {
                textArray[id].setText(endGameMessage);
                found = true;
            }
            if (textArray[id].getKeyword() == textKeywordWaveEnd) {
                Registry::getInstance().removeEntity(id);
            }
        }

        if (found == false) {
            Registry::getInstance().addEntity();
            Raylib::Text endGameText =
                Raylib::Text(endGameMessage, pos, fontSize, Raylib::WHITE, textKeyWordGameEnd);
            Registry::getInstance().getComponents<Raylib::Text>().insertBack(endGameText);
        }
    }

    void EventsSystems::handleEndGameEvent(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        constexpr std::size_t secondBeforeEnd = 5;
        static std::size_t clockId            = Registry::getInstance().getClock().create(false);
        std::size_t elapsedSeconds = Registry::getInstance().getClock().elapsedSecondsSince(clockId);
        std::string seconds        = std::to_string(secondBeforeEnd - elapsedSeconds);
        std::string endGameMessage;

        if (isGameWin() == true) {
            endGameMessage =
                "You win! Redirecting to menu in " + seconds + (seconds == "1" ? " second" : " seconds")
        } else {
            endGameMessage =
                "You lose! Redirecting to menu in " + seconds + (seconds == "1" ? " second" : " seconds")
        }

        modifEndGameText(endGameMessage);

        if (Registry::getInstance().getClock().elapsedSecondsSince(clockId) >= secondBeforeEnd) {
            Scene::SceneManager::getInstance().changeScene(Scene::Scene::MENU);
        }
    }

    std::vector<std::function<void(std::size_t, std::size_t)>> EventsSystems::getEventSystems()
    {
        return {playerMovement, changeScene, playerShootBullet};
    }
} // namespace Systems
