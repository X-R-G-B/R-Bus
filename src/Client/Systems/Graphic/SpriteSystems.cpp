/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** SpriteSystems implementation
*/

#include "SpriteSystems.hpp"
#include "CustomTypes.hpp"
#include "Raylib.hpp"
#include "SharedValues.hpp"

namespace Systems {
    static std::vector<Types::Rect> getCurrentList(Types::AnimRect &animRect)
    {
        switch (animRect.currentRectList) {
            case Types::RectListType::MOVE: return animRect.moveRects;
            case Types::RectListType::DEAD: return animRect.deadRects;
            case Types::RectListType::ATTACK: return animRect.attackRects;
            default: return {animRect.defaultRect};
        }
    }

    void GraphicSystems::rectIncrementation(
        std::size_t /*unused*/,
        std::size_t /*unused*/)
    {
        Registry &registry = Registry::getInstance();
        Registry::components<Types::AnimRect> arrAnimRect =
            registry.getComponents<Types::AnimRect>();
        std::vector<std::size_t> ids = arrAnimRect.getExistingsId();
        Registry::components<Types::Rect> arrRect =
            registry.getComponents<Types::Rect>();

        for (auto id : ids) {
            Types::AnimRect &animRect = arrAnimRect[id];
            if (arrRect.exist(id)) {
                arrRect[id] =
                    getCurrentList(animRect)[animRect.currentRectInList];
            }
            if (animRect.currentRectList != Types::RectListType::DEFAULTRECT) {
                animRect.currentRectInList++;
                if (animRect.currentRectInList
                    == getCurrentList(animRect).size()) {
                    animRect.currentRectInList = 0;
                }
            }
        }
    }

    void
    GraphicSystems::rectRenderer(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        Registry &registry = Registry::getInstance();
        Registry::components<Types::Position> arrPosition =
            registry.getComponents<Types::Position>();
        Registry::components<Types::RectangleShape> arrRect =
            registry.getComponents<Types::RectangleShape>();
        std::vector<std::size_t> rectShapeIndexes = arrRect.getExistingsId();

        for (auto id : rectShapeIndexes) {
            if (!arrPosition.exist(id)) {
                continue;
            }
            Types::Position &position        = arrPosition[id];
            Types::RectangleShape &rectangle = arrRect[id];

            float x =
                (position.x * static_cast<float>(Raylib::getScreenWidth()))
                / denominator;
            float y =
                (position.y * static_cast<float>(Raylib::getScreenHeight()))
                / denominator;

            float width =
                (rectangle.width * static_cast<float>(Raylib::getScreenWidth()))
                / denominator;
            float height = (rectangle.height
                            * static_cast<float>(Raylib::getScreenHeight()))
                / denominator;

            DrawRectangle(
                static_cast<int>(x),
                static_cast<int>(y),
                static_cast<int>(width),
                static_cast<int>(height),
                PURPLE);
        }
    }

    static void
    drawSpriteWithoutRect(Types::Position &position, Raylib::Sprite &sprite)
    {
        float scale               = 1.0F;
        float rotation            = 0;
        const Raylib::Color tint  = Raylib::White;
        Raylib::Vector2 spritePos = {0, 0};

        float x = (position.x * static_cast<float>(Raylib::getScreenWidth()))
            / denominator;
        float y = (position.y * static_cast<float>(Raylib::getScreenHeight()))
            / denominator;

        scale =
            (sprite.getWidth() * static_cast<float>(Raylib::getScreenWidth()))
            / denominator / static_cast<float>(sprite.getTextureWidth());
        spritePos = {x, y};

        sprite.drawEx(spritePos, rotation, scale, tint);
    }

    static void drawSpriteWithRect(
        Types::Position &position,
        Raylib::Sprite &sprite,
        Types::Rect &rect)
    {
        Raylib::Vector2 origin   = {0, 0};
        float rotation           = 0;
        const Raylib::Color tint = Raylib::White;

        float x = (position.x * static_cast<float>(Raylib::getScreenWidth()))
            / denominator;
        float y = (position.y * static_cast<float>(Raylib::getScreenHeight()))
            / denominator;

        float width =
            (sprite.getWidth() * static_cast<float>(Raylib::getScreenWidth()))
            / denominator;
        float height =
            (sprite.getHeight() * static_cast<float>(Raylib::getScreenHeight()))
            / denominator;

        sprite.drawPro(
            Raylib::Rectangle(rect.x, rect.y, rect.width, rect.height),
            Raylib::Rectangle(x, y, width, height),
            origin,
            rotation,
            tint);
    }

    static void renderEntityList(std::vector<std::size_t> list)
    {
        Registry &registry = Registry::getInstance();
        Registry::components<Raylib::Sprite> arrSprite =
            registry.getComponents<Raylib::Sprite>();
        Registry::components<Types::Rect> arrRect =
            registry.getComponents<Types::Rect>();
        Registry::components<Types::Position> arrPosition =
            registry.getComponents<Types::Position>();

        for (auto id : list) {
            if (arrPosition.exist(id)) {
                if (arrRect.exist(id)) {
                    drawSpriteWithRect(
                        arrPosition[id],
                        arrSprite[id],
                        arrRect[id]);
                } else {
                    drawSpriteWithoutRect(arrPosition[id], arrSprite[id]);
                }
            }
        }
    }

    void GraphicSystems::spriteRenderer(
        std::size_t /*unused*/,
        std::size_t /*unused*/)
    {
        Registry &registry = Registry::getInstance();
        std::vector<std::vector<std::size_t>> backLayers =
            registry.getBackLayers();
        std::vector<std::size_t> defaultLayer = registry.getDefaultLayer();
        std::vector<std::vector<std::size_t>> frontLayers =
            registry.getFrontLayers();

        for (auto list : backLayers) {
            renderEntityList(list);
        }
        renderEntityList(defaultLayer);
        for (auto list : frontLayers) {
            renderEntityList(list);
        }
    }

    std::vector<std::function<void(std::size_t, std::size_t)>>
    GraphicSystems::getSpriteSystems()
    {
        return {rectIncrementation, rectRenderer, spriteRenderer};
    }
} // namespace Systems
