/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** EventsSystems implementation
*/

#include "EventsSystems.hpp"
#include "CustomTypes.hpp"
#include "Raylib.hpp"
#include "Registry.hpp"
#include "SceneManager.hpp"

namespace Systems {
    static void checkAnimRect(std::size_t id)
    {
        Registry::components<Types::AnimRect> arrAnimRect =
            Registry::getInstance().getComponents<Types::AnimRect>();

        if (arrAnimRect.exist(id)) {
            Types::AnimRect &anim = arrAnimRect[id];
            if (anim.currentRectList != Types::RectListType::MOVE) {
                anim.changeRectList(Types::RectListType::MOVE);
            }
        }
    }

    void EventsSystems::playerMovement(
        std::size_t /*unused*/,
        std::size_t /*unused*/)
    {
        Registry &registry = Registry::getInstance();
        Registry::components<Types::Position> arrPosition =
            registry.getComponents<Types::Position>();
        std::vector<std::size_t> playerId =
            registry.getComponents<Types::Player>().getExistingsId();

        for (std::size_t id : playerId) {
            if (Raylib::isKeyDown(Raylib::KeyboardKey::KB_RIGHT)) {
                checkAnimRect(id);
                arrPosition[id].x += 1;
            }
            if (Raylib::isKeyDown(Raylib::KeyboardKey::KB_LEFT)) {
                checkAnimRect(id);
                arrPosition[id].x -= 1;
            }
            if (Raylib::isKeyDown(Raylib::KeyboardKey::KB_UP)) {
                checkAnimRect(id);
                arrPosition[id].y -= 1;
            }
            if (Raylib::isKeyDown(Raylib::KeyboardKey::KB_DOWN)) {
                checkAnimRect(id);
                arrPosition[id].y += 1;
            }
        }
    }

    const Types::Rect spriteRect = {300, 121, 32, 10};
    const std::string bulletPath = "assets/R-TypeSheet/r-typesheet1.gif";
    constexpr float playerWidth  = 5.0F;
    constexpr float playerHeight = 5.0F;
    const Types::CollisionRect collisionRect = {1, 1};
    const Types::Velocity velocity           = {-0.7F, 0.0F};
    const Types::Missiles missileType        = {"classic"};
    const Types::Health health               = {1};
    const Types::Dammage dammage             = {10};

    void playerShootBullet(std::size_t, std::size_t)
    {
        Registry::components<Types::Player> arrPlayer =
            Registry::getInstance().getComponents<Types::Player>();
        Registry::components<Types::Position> arrPosition =
            Registry::getInstance().getComponents<Types::Position>();
        Registry::components<Types::Rect> arrRect =
            Registry::getInstance().getComponents<Types::Rect>();

        std::vector<std::size_t> ids = arrPlayer.getExistingsId();

        if (Raylib::isKeyDown(Raylib::KeyboardKey::KB_SPACE)) {
            for (auto &id : ids) {
                if (arrPosition.exist(id) && arrRect.exist(id)) {
                    std::size_t entityId = Registry::getInstance().addEntity();
                    Registry::getInstance()
                        .getComponents<Types::Position>()
                        .insertBack({arrPosition[id].x, arrPosition[id].y});
                    Registry::getInstance()
                        .getComponents<Raylib::Sprite>()
                        .insertBack(
                            {bulletPath, playerWidth, playerHeight, entityId});
                    Registry::getInstance()
                        .getComponents<Types::Rect>()
                        .insertBack(spriteRect);
                    Registry::getInstance()
                        .getComponents<Types::CollisionRect>()
                        .insertBack(collisionRect);
                    Registry::getInstance()
                        .getComponents<Types::Missiles>()
                        .insertBack(missileType);
                    Registry::getInstance()
                        .getComponents<Types::Velocity>()
                        .insertBack(velocity);
                    Registry::getInstance()
                        .getComponents<Types::Health>()
                        .insertBack(health);
                    Registry::getInstance()
                        .getComponents<Types::Dammage>()
                        .insertBack(dammage);
                    Registry::getInstance().setToFrontLayers(entityId);
                }
            }
        }
    }

    void
    EventsSystems::changeScene(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        if (Raylib::isKeyDown(Raylib::KeyboardKey::KB_J)) {
            SceneManager &sceneManager = SceneManager::getInstance();
            if (sceneManager.getCurrentScene() == MAIN_GAME) {
                sceneManager.changeScene(MENU);
            } else {
                sceneManager.changeScene(MAIN_GAME);
            }
        }
    }

    std::vector<std::function<void(std::size_t, std::size_t)>>
    EventsSystems::getEventSystems()
    {
        return {playerMovement, changeScene, playerShootBullet};
    }
} // namespace Systems
