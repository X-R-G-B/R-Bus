/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** EventsSystems implementation
*/

#pragma once

#include "EventsSystems.hpp"
#include "GraphicsCustomTypes.hpp"
#include "GameSystems.hpp"
#include "Maths.hpp"
#include "NitworkClient.hpp"
#include "Registry.hpp"
#include "SceneManager.hpp"
#include "SystemManagersDirector.hpp"

namespace Systems {
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
            if (Raylib::KeyboardInput::isKeyDown(Raylib::KeyboardKey::KB_RIGHT)) {
                checkAnimRect(id, clock_, clockId);
                Maths::addNormalIntToDecimalInt(arrPos[id].x, 1);
            }
            if (Raylib::KeyboardInput::isKeyDown(Raylib::KeyboardKey::KB_LEFT)) {
                checkAnimRect(id, clock_, clockId);
                Maths::subNormalIntToDecimalInt(arrPos[id].x, 1);
            }
            if (Raylib::KeyboardInput::isKeyDown(Raylib::KeyboardKey::KB_UP)) {
                checkAnimRect(id, clock_, clockId);
                Maths::subNormalIntToDecimalInt(arrPos[id].y, 1);
            }
            if (Raylib::KeyboardInput::isKeyDown(Raylib::KeyboardKey::KB_DOWN)) {
                checkAnimRect(id, clock_, clockId);
                Maths::addNormalIntToDecimalInt(arrPos[id].y, 1);
            }
        }
    }

    void playerShootBullet(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        static const std::size_t waitTimeBullet           = 500;
        static const std::string soundPathShoot           = "assets/Audio/Sounds/laser.ogg";
        Registry &registry                                = Registry::getInstance();
        Registry::components<Types::Position> arrPosition = registry.getComponents<Types::Position>();
        Registry::components<struct health_s> arrHealth   = registry.getComponents<struct health_s>();
        Clock &clock_                                     = registry.getClock();
        static std::size_t clockId                        = clock_.create(true);
        Registry::components<Raylib::Sound> arrSounds     = registry.getComponents<Raylib::Sound>();
        std::vector<std::size_t> ids =
            registry.getEntitiesByComponents({typeid(Types::Player), typeid(Types::Position)});

        if (Raylib::KeyboardInput::isKeyDown(Raylib::KeyboardKey::KB_SPACE) == false
            || clock_.elapsedMillisecondsSince(clockId) < waitTimeBullet) {
            return;
        }

        for (auto &sound : arrSounds) {
            if (sound.getPath() == soundPathShoot) {
                sound.setNeedToPlay(true);
                break;
            }
        }

        for (auto &id : ids) {
            // send bullet to server
            if (arrHealth.exist(id) && arrHealth[id].hp <= 0) {
                continue;
            }
            Nitwork::NitworkClient::getInstance().addNewBulletMsg(
                {Maths::removeIntDecimals(arrPosition[id].x), Maths::removeIntDecimals(arrPosition[id].y)},
                CLASSIC);
            struct Types::Missiles missile = {
                .type = CLASSIC,
            };
            createMissile(arrPosition[id], missile);
            clock_.restart(clockId);
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

    const std::string musicPath      = "assets/Audio/Musics/Title.mp3";
    const std::string soundPathShoot = "assets/Audio/Sounds/laser.ogg";

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

        Registry::getInstance().addEntity();
        Registry::getInstance().getComponents<Raylib::Music>().insertBack(music);
        Registry::getInstance().getComponents<Raylib::Sound>().insertBack(soundEffectShoot);
        SystemManagersDirector::getInstance().getSystemManager(managerId).removeSystem(systemId);
    }

    std::vector<std::function<void(std::size_t, std::size_t)>> EventsSystems::getEventsSystems()
    {
        return {playerMovement, changeScene, playerShootBullet, playSoundWithKey, initAudio};
    }
} // namespace Systems
