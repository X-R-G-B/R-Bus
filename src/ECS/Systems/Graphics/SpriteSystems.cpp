/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** SpriteSystems implementation
*/

#include "SpriteSystems.hpp"
#include "GraphicsCustomTypes.hpp"
#include "Logger.hpp"
#include "Maths.hpp"
#include "SharedValues.hpp"

namespace Systems {

    static Raylib::Vector2 calculatePosition(const float x, const float y)
    {
        Raylib::Vector2 pos = {0, 0};

        pos.x = (x * static_cast<float>(Raylib::getScreenWidth())) / denominator;
        pos.y = (y * static_cast<float>(Raylib::getScreenHeight())) / denominator;
        return (pos);
    }

    static Raylib::Vector2 calculateSize(Raylib::Sprite &sprite)
    {
        Raylib::Vector2 size = {0, 0};

        size.x = (sprite.getWidth() * static_cast<float>(Raylib::getScreenWidth())) / denominator;
        size.y = (sprite.getHeight() * static_cast<float>(Raylib::getScreenHeight())) / denominator;
        return (size);
    }

    static std::vector<Types::Rect> getCurrentList(Types::AnimRect &animRect)
    {
        switch (animRect.currentRectList) {
            case Types::RectListType::MOVE: return animRect.moveRects;
            case Types::RectListType::DEAD: return animRect.deadRects;
            case Types::RectListType::ATTACK: return animRect.attackRects;
            default: return {animRect.defaultRect};
        }
    }

    static void incrementRect(Types::AnimRect &animRect)
    {
        if (animRect.currentRectList != Types::RectListType::DEFAULTRECT) {
            animRect.currentRectInList++;
            if (animRect.currentRectInList == getCurrentList(animRect).size()) {
                animRect.currentRectInList = 0;
            }
        }
    }

    constexpr std::size_t elapsedBetweenAnim = 120;

    static void updateAnimRect(std::size_t id, Types::AnimRect &animRect, Types::Rect &rect)
    {
        Clock &clock_ = Registry::getInstance().getClock();
        static std::unordered_map<std::size_t, std::size_t> clockIds;
        auto clockId = clockIds.find(id);
        if (clockId == clockIds.end()) {
            clockIds[id] = clock_.create();
        }

        if (clock_.elapsedMillisecondsSince(clockIds[id]) < elapsedBetweenAnim) {
            return;
        }
        std::vector<Types::Rect> rects = getCurrentList(animRect);
        if (animRect.currentRectInList < rects.size()) {
            rect = rects[animRect.currentRectInList];
        } else {
            Logger::error(
                "Attempt to access id : " + std::to_string(animRect.currentRectInList) + " in list of size "
                + std::to_string(getCurrentList(animRect).size()));
            animRect.currentRectList   = Types::RectListType::DEFAULTRECT;
            animRect.currentRectInList = 0;
            return;
        }
        incrementRect(animRect);
        clock_.decreaseMilliseconds(clockIds[id], elapsedBetweenAnim);
    }

    void GraphicsSystems::rectIncrementation(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        Registry &registry                                = Registry::getInstance();
        Registry::components<Types::AnimRect> arrAnimRect = registry.getComponents<Types::AnimRect>();
        Registry::components<Types::Rect> arrRect         = registry.getComponents<Types::Rect>();
        std::vector<std::size_t> ids =
            registry.getEntitiesByComponents({typeid(Types::AnimRect), typeid(Types::Rect)});

        for (auto id : ids) {
            updateAnimRect(id, arrAnimRect[id], arrRect[id]);
        }
    }

    void GraphicsSystems::rectRenderer(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        Registry &registry                                = Registry::getInstance();
        Registry::components<Types::Position> arrPosition = registry.getComponents<Types::Position>();
        Registry::components<Types::RectangleShape> arrRect =
            registry.getComponents<Types::RectangleShape>();
        std::vector<std::size_t> ids =
            registry.getEntitiesByComponents({typeid(Types::RectangleShape), typeid(Types::Position)});

        for (auto id : ids) {
            Types::Position &position        = arrPosition[id];
            Types::RectangleShape &rectangle = arrRect[id];

            float x = (Maths::intToFloatConservingDecimals(position.x)
                       * static_cast<float>(Raylib::getScreenWidth()))
                / denominator;
            float y = (Maths::intToFloatConservingDecimals(position.y)
                       * static_cast<float>(Raylib::getScreenHeight()))
                / denominator;

            float width  = (rectangle.width * static_cast<float>(Raylib::getScreenWidth())) / denominator;
            float height = (rectangle.height * static_cast<float>(Raylib::getScreenHeight())) / denominator;

            DrawRectangle(
                static_cast<int>(x),
                static_cast<int>(y),
                static_cast<int>(width),
                static_cast<int>(height),
                PURPLE);
        }
    }

    static void
    drawSpriteWithoutRect(Types::Position &position, Raylib::Sprite &sprite, std::size_t entityId)
    {
        Registry::components<Types::Rotation> arrRotation =
            Registry::getInstance().getComponents<Types::Rotation>();
        Registry::components<Types::Color> arrColor = Registry::getInstance().getComponents<Types::Color>();
        float scale                                 = 1.0F;
        float rotation                              = 0;
        Raylib::Color tint                          = Raylib::White;
        Raylib::Vector2 spritePos                   = {0, 0};

        rotation = arrRotation.exist(entityId) ? arrRotation[entityId].rotate : rotation;
        tint     = arrColor.exist(entityId) ? arrColor[entityId].color : tint;
        scale    = (sprite.getWidth() * static_cast<float>(Raylib::getScreenWidth())) / denominator
            / static_cast<float>(sprite.getTextureWidth());
        spritePos = calculatePosition(
            Maths::intToFloatConservingDecimals(position.x),
            Maths::intToFloatConservingDecimals(position.y));
        sprite.drawEx(spritePos, rotation, scale, tint);
    }

    static void drawSpriteWithRect(
        Types::Position &position,
        Raylib::Sprite &sprite,
        Types::Rect &rect,
        std::size_t entityId)
    {
        Registry::components<Types::Rotation> arrRotation =
            Registry::getInstance().getComponents<Types::Rotation>();
        Registry::components<Types::Color> arrColor = Registry::getInstance().getComponents<Types::Color>();
        Registry::components<Types::Origin> arrOrigin =
            Registry::getInstance().getComponents<Types::Origin>();
        Types::Origin origin = {0, 0};
        Raylib::Vector2 size = {0, 0};
        Raylib::Vector2 pos  = {0, 0};
        float rotation       = 0;
        Raylib::Color tint   = Raylib::White;

        origin   = arrOrigin.exist(entityId) ? Types::Origin({arrOrigin[entityId].x, arrOrigin[entityId].y})
                                             : origin;
        rotation = arrRotation.exist(entityId) ? arrRotation[entityId].rotate : rotation;
        tint     = arrColor.exist(entityId) ? arrColor[entityId].color : tint;
        pos      = calculatePosition(
            Maths::intToFloatConservingDecimals(position.x),
            Maths::intToFloatConservingDecimals(position.y));
        size = calculateSize(sprite);

        sprite.drawPro(
            Raylib::Rectangle(rect.x, rect.y, rect.width, rect.height),
            Raylib::Rectangle(pos.x, pos.y, size.x, size.y),
            Raylib::Vector2(origin.x, origin.y),
            rotation,
            tint);
    }

    static void renderEntityList(std::vector<std::size_t> list)
    {
        Registry &registry                                = Registry::getInstance();
        Registry::components<Raylib::Sprite> arrSprite    = registry.getComponents<Raylib::Sprite>();
        Registry::components<Types::Rect> arrRect         = registry.getComponents<Types::Rect>();
        Registry::components<Types::Position> arrPosition = registry.getComponents<Types::Position>();

        for (auto id : list) {
            if (arrPosition.exist(id) && arrSprite.exist(id)) {
                if (arrRect.exist(id)) {
                    drawSpriteWithRect(arrPosition[id], arrSprite[id], arrRect[id], id);
                } else {
                    drawSpriteWithoutRect(arrPosition[id], arrSprite[id], id);
                }
            }
        }
    }

    void GraphicsSystems::spriteRenderer(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        Registry &registry                                = Registry::getInstance();
        std::vector<std::vector<std::size_t>> backLayers  = registry.getBackLayers();
        std::vector<std::size_t> defaultLayer             = registry.getDefaultLayer();
        std::vector<std::vector<std::size_t>> frontLayers = registry.getFrontLayers();

        for (auto list : backLayers) {
            renderEntityList(list);
        }
        renderEntityList(defaultLayer);
        for (auto list : frontLayers) {
            renderEntityList(list);
        }
    }

    void GraphicsSystems::createSprite(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        auto &arrSpriteDatas = Registry::getInstance().getComponents<Types::SpriteDatas>();
        auto &arrSprite      = Registry::getInstance().getComponents<Raylib::Sprite>();

        auto ids = arrSpriteDatas.getExistingsId();
        for (auto id : ids) {
            auto &spriteDatas = arrSpriteDatas[id];
            Raylib::Sprite sprite(
                spriteDatas.fileName,
                Maths::intToFloatConservingDecimals(spriteDatas.width),
                Maths::intToFloatConservingDecimals(spriteDatas.height),
                id);
            arrSprite.insert(id, sprite);
            switch (spriteDatas.layer) {
                case BACKLAYER:
                    Registry::getInstance().setToBackLayers(
                        id,
                        static_cast<enum BackLayers>(spriteDatas.layerSide));
                    break;
                case FRONTLAYER: Registry::getInstance().setToDefaultLayer(id); break;
                case DEFAULTLAYER:
                    Registry::getInstance().setToFrontLayers(
                        id,
                        static_cast<enum FrontLayers>(spriteDatas.layerSide));
                    break;
            }
        }
        arrSpriteDatas.clear();
    }

    std::vector<std::function<void(std::size_t, std::size_t)>> GraphicsSystems::getSpriteSystems()
    {
        return {rectIncrementation, rectRenderer, spriteRenderer, createSprite};
    }
} // namespace Systems
