/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** EventsSystems implementation
*/

#include "EventsSystems.hpp"
#include "B-luga-graphics/AnimRect.hpp"
#include "B-luga-graphics/GraphicsCustomTypes.hpp"
#include "GameSystems.hpp"
#include "B-luga/Json.hpp"
#include "B-luga/Logger.hpp"
#include "B-luga/Maths/Maths.hpp"
#include "NitworkClient.hpp"
#include "B-luga/Registry.hpp"
#include "B-luga/SceneManager.hpp"
#include "B-luga/SystemManagers/SystemManagersDirector.hpp"
#include "CreateMissiles.hpp"
#include "ResourcesManager.hpp"

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
        nlohmann::json bulletData = json.getJsonObjectById(ResourcesManager::getPathByJsonType(JsonType::BULLETS), bulletType, "bullets");
        float waitTimeBullet      = json.getDataFromJson<float>(bulletData, "waitTimeBullet");

        return clock_.elapsedMillisecondsSince(clockId) >= static_cast<std::size_t>(waitTimeBullet);
    }

    static bool checkBulletRequirements(struct Types::Missiles &missile)
    {
        bool isKeyPressed = false;

        for (const auto &key : bulletKeyMap) {
            if (Raylib::KeyboardInput::isKeyDown(key.second)) {
                missile.type = key.first;
                isKeyPressed = true;
                break;
            }
        }
        return isKeyPressed
            && isBulletTimeElapsed(getClockIdFromMissileType(missile.type));
    }

    static Types::Position adjustPlayerBulletPosition(Types::Position &playerPos, std::size_t id)
    {
        Registry &registry                                = Registry::getInstance();
        Registry::components<Types::CollisionRect> arrCol = registry.getComponents<Types::CollisionRect>();

        if (arrCol.exist(id)) {
            Types::CollisionRect &col = arrCol[id];
            float posX                = Maths::intToFloatConservingDecimals(playerPos.x)
                + (Maths::intToFloatConservingDecimals(col.width) / 2.F);
            float posY = Maths::intToFloatConservingDecimals(playerPos.y)
                + (Maths::intToFloatConservingDecimals(col.height) / 2.F);
            return {Maths::floatToIntConservingDecimals(posX), Maths::floatToIntConservingDecimals(posY)};
        }
        return {playerPos.x, playerPos.y};
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

        if (!checkBulletRequirements(missile)) {
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
            createMissile(adjustPlayerBulletPosition(arrPosition[id], id), missile);
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
            if (Raylib::KeyboardInput::isKeyDown(Raylib::KeyboardKey::KB_RIGHT)) {
                checkAnimRect(id, clock_, clockId, Types::Direction::RIGHT);
                Maths::addFloatToDecimalInt(arrPos[id].x, 1.F);
                isKeyPressed = true;
            }
            if (Raylib::KeyboardInput::isKeyDown(Raylib::KeyboardKey::KB_LEFT)) {
                checkAnimRect(id, clock_, clockId, Types::Direction::LEFT);
                Maths::subFloatToDecimalInt(arrPos[id].x, 1.F);
                isKeyPressed = true;
            }
            if (Raylib::KeyboardInput::isKeyDown(Raylib::KeyboardKey::KB_UP)) {
                checkAnimRect(id, clock_, clockId, Types::Direction::UP);
                Maths::subFloatToDecimalInt(arrPos[id].y, 1.F);
                isKeyPressed = true;
            }
            if (Raylib::KeyboardInput::isKeyDown(Raylib::KeyboardKey::KB_DOWN)) {
                checkAnimRect(id, clock_, clockId, Types::Direction::DOWN);
                Maths::addFloatToDecimalInt(arrPos[id].y, 1.F);
                isKeyPressed = true;
            }
            if (!isKeyPressed) {
                checkAnimRect(id, clock_, clockId, Types::Direction::NONE);
            }
        }
    }

    void EventsSystems::changeScene(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        if (Raylib::KeyboardInput::isKeyDown(Raylib::KeyboardKey::KB_J)) {
            auto &sceneManager = Scene::SceneManager::getInstance();
            if (sceneManager.getCurrentScene() == SceneType::MAIN_GAME) {
                sceneManager.changeScene(SceneType::MENU);
            } else {
                sceneManager.changeScene(SceneType::MAIN_GAME);
            }
        }
    }

    const std::string musicPath       = "assets/Audio/Musics/Title.mp3";
    const std::string soundPathShoot  = "assets/Audio/Sounds/laser.ogg";
    const std::string soundPathShoot2 = "assets/Audio/Sounds/laser2.ogg";
    const std::string soundPathShoot3 = "assets/Audio/Sounds/glitch.ogg";

    void EventsSystems::playSoundWithKey(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        Registry &registry                            = Registry::getInstance();
        Registry::components<Raylib::Music> arrMusics = registry.getComponents<Raylib::Music>();

        for (auto &music : arrMusics) {
            if (music.getPath() == musicPath && Raylib::KeyboardInput::isKeyPressed(Raylib::KeyboardKey::KB_M)) {
                music.setNeedToPlay(true);
            }
        }
    }

    void initAudio(std::size_t managerId, std::size_t systemId)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        constexpr float musicVolume = 0.60F;
        constexpr float soundVolume = 0.63F;

        Raylib::Music music(musicPath, musicVolume);
        Raylib::Sound soundEffectShoot(soundPathShoot, soundVolume);
        Raylib::Sound soundEffectShoot2(soundPathShoot2, soundVolume);
        Raylib::Sound soundEffectShoot3(soundPathShoot3, soundVolume);

        Registry::getInstance().addEntity();
        Registry::getInstance().getComponents<Raylib::Music>().insertBack(music);
        Registry::getInstance().addEntity();
        Registry::getInstance().getComponents<Raylib::Sound>().insertBack(soundEffectShoot);
        Registry::getInstance().addEntity();
        Registry::getInstance().getComponents<Raylib::Sound>().insertBack(soundEffectShoot2);
        Registry::getInstance().addEntity();
        Registry::getInstance().getComponents<Raylib::Sound>().insertBack(soundEffectShoot3);
        SystemManagersDirector::getInstance().getSystemManager(managerId).removeSystem(systemId);
    }

    std::vector<std::function<void(std::size_t, std::size_t)>> EventsSystems::getEventsSystems()
    {
        return {playerMovement, changeScene, playerShootBullet, playSoundWithKey, initAudio};
    }
} // namespace Systems
