/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Menu
*/

#include "Menu.hpp"
#include "B-luga-graphics/AnimRect.hpp"
#include "B-luga-graphics/GraphicsCustomTypes.hpp"
#include "B-luga-physics/ECSCustomTypes.hpp"
#include "B-luga/Maths/Maths.hpp"
#include "B-luga/SceneManager.hpp"
#include "ButtonCallbacks.hpp"
#include "NitworkClient.hpp"

namespace Menu {
    // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
    MenuBuilder MenuBuilder::_instance = MenuBuilder();
    // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

    constexpr float maxPercent = 100.0F;

    static std::size_t initSpriteFromJson(nlohmann::json &elem)
    {
        std::size_t id = Registry::getInstance().addEntity();
        Types::SpriteDatas spriteDatas(
            Json::getInstance().getDataFromJson<std::string>(elem, "spritePath"),
            Json::getInstance().getDataFromJson<int>(elem, "width"),
            Json::getInstance().getDataFromJson<int>(elem, "height"),
            FRONTLAYER,
            static_cast<std::size_t>(FRONT));
        if (Json::isDataExist(elem, "rect")) {
            Types::Rect rect = {
                Types::Rect(Json::getInstance().getDataFromJson<Types::Rect>(elem, "rect"))};
            if (Json::isDataExist(elem, "animRect")) {
                Types::AnimRect animRect(
                    rect,
                    Json::getInstance().getDataFromJson<nlohmann::json>(elem, "animRect"));
                Registry::getInstance().getComponents<Types::AnimRect>().insertBack(animRect);
            }
            Registry::getInstance().getComponents<Types::Rect>().insertBack(rect);
        }
        Types::Position position = {
            Types::Position(Json::getInstance().getDataFromJson<Types::Position>(elem, "position"))};
        Registry::getInstance().getComponents<Types::Position>().insertBack(position);
        Registry::getInstance().getComponents<Types::SpriteDatas>().insertBack(spriteDatas);
        return (id);
    }

    static void initFromSprite(nlohmann::json &elem)
    {
        Types::SpriteDatas spriteDatas(
            Json::getInstance().getDataFromJson<std::string>(elem, "spritePath"),
            Json::getInstance().getDataFromJson<int>(elem, "width"),
            Json::getInstance().getDataFromJson<int>(elem, "height"),
            FRONTLAYER,
            static_cast<std::size_t>(FRONT));
        if (Json::isDataExist(elem, "rect")) {
            Types::Rect rect = {
                Types::Rect(Json::getInstance().getDataFromJson<Types::Rect>(elem, "rect"))};
            if (Json::isDataExist(elem, "animRect")) {
                Types::AnimRect animRect(
                    rect,
                    Json::getInstance().getDataFromJson<nlohmann::json>(elem, "animRect"));
                Registry::getInstance().getComponents<Types::AnimRect>().insertBack(animRect);
            }
            Registry::getInstance().getComponents<Types::Rect>().insertBack(rect);
        }
        Registry::getInstance().getComponents<Types::SpriteDatas>().insertBack(spriteDatas);
    }

    static void initInputBoxText(nlohmann::json &elem)
    {
        std::string text = Json::isDataExist(elem, "text")
            ? Json::getInstance().getDataFromJson<std::string>(elem, "text")
            : "";
        std::string name = Json::isDataExist(elem, "name")
            ? Json::getInstance().getDataFromJson<std::string>(elem, "name")
            : "";
        Types::FontSize fsz({Json::getInstance().getDataFromJson<float>(elem, "textSize")});
        auto textComp    = Raylib::Text::fromText(text);
        std::size_t maxChar(Json::getInstance().getDataFromJson<std::size_t>(elem, "maxChar"));
        Types::InputBox inputBox(text, name, maxChar);
        auto search =
            Types::colorMatchStrings.find(Json::getInstance().getDataFromJson<std::string>(elem, "color"));
        Raylib::Color color = search != Types::colorMatchStrings.end()
            ? Types::colorMatchStrings.at(Json::getInstance().getDataFromJson<std::string>(elem, "color"))
            : Raylib::Color(Raylib::ColorDef::White);
        Registry::getInstance().getComponents<Raylib::Color>().insertBack(color);

        Raylib::Text textComp(text);

        if (Json::isDataExist(elem, "textPosition")) {
            Types::Position tmpPos(
                Json::getInstance().getDataFromJson<Types::Position>(elem, "textPosition"));
            Raylib::Vector2 textPos(
                Maths::intToFloatConservingDecimals(tmpPos.x),
                Maths::intToFloatConservingDecimals(tmpPos.y));
            textComp.setPosition(textPos);
        }
        Registry::getInstance().getComponents<Raylib::Text>().insertBack(textComp);
        Registry::getInstance().getComponents<Types::InputBox>().insertBack(inputBox);
        Registry::getInstance().getComponents<Types::FontSize>().insertBack(fsz);
        Registry::getInstance().getComponents<Raylib::Color>().insertBack(color);
    }

    static std::size_t initInputBox(nlohmann::json &elem)
    {
        std::size_t id = Registry::getInstance().addEntity();

        Types::Position position(Json::getInstance().getDataFromJson<Types::Position>(elem, "position"));
        Types::CollisionRect collisionRect(
            Json::getInstance().getDataFromJson<Types::CollisionRect>(elem, "collisionRect"));

        initInputBoxText(elem);
        if (!Json::getInstance().isDataExist(elem, "spritePath")) {
            Types::RectangleShape rectangle(
                {Json::getInstance().getDataFromJson<float>(elem, "width"),
                 Json::getInstance().getDataFromJson<float>(elem, "height")});
            Registry::getInstance().getComponents<Types::RectangleShape>().insertBack(rectangle);
            Registry::getInstance().setToFrontLayers(id);
        } else {
            initFromSprite(elem);
        }
        Registry::getInstance().getComponents<Types::FontSize>().insertBack(fsz);
        Registry::getInstance().getComponents<Raylib::TextShared>().insertBack(textComp);
        Registry::getInstance().getComponents<Types::InputBox>().insertBack(inputBox);
        Registry::getInstance().getComponents<Types::Position>().insertBack(position);
        Registry::getInstance().getComponents<Types::CollisionRect>().insertBack(collisionRect);
        return (id);
    }

    static std::size_t initButtonFromSprite(nlohmann::json &elem, std::function<void()> &callback)
    {
        Types::Button button(callback);

        std::size_t id = Registry::getInstance().addEntity();
        initFromSprite(elem);
        Types::Position position = {
            Types::Position(Json::getInstance().getDataFromJson<Types::Position>(elem, "position"))};
        Types::CollisionRect collisionRect = {Types::CollisionRect(
            Json::getInstance().getDataFromJson<Types::CollisionRect>(elem, "collisionRect"))};

        Registry::getInstance().getComponents<Types::Position>().insertBack(position);
        Registry::getInstance().getComponents<Types::CollisionRect>().insertBack(collisionRect);
        Registry::getInstance().getComponents<Types::Button>().insertBack(button);
        return (id);
    }

    static std::size_t initButtonFromRectangle(nlohmann::json &elem, std::function<void()> &callback)
    {
        std::size_t id = Registry::getInstance().addEntity();

        Types::Position position = {
            Types::Position(Json::getInstance().getDataFromJson<Types::Position>(elem, "position"))};
        Types::CollisionRect collisionRect = {Types::CollisionRect(
            Json::getInstance().getDataFromJson<Types::CollisionRect>(elem, "collisionRect"))};
        Types::Button button(callback);

        Types::RectangleShape rectangle(
            {static_cast<float>(collisionRect.width), static_cast<float>(collisionRect.height)});
        Registry::getInstance().getComponents<Types::RectangleShape>().insertBack(rectangle);
        Registry::getInstance().getComponents<Types::Position>().insertBack(position);
        Registry::getInstance().getComponents<Types::CollisionRect>().insertBack(collisionRect);
        Registry::getInstance().getComponents<Types::Button>().insertBack(button);
        Registry::getInstance().setToFrontLayers(id);
        return (id);
    }

    static std::size_t initText(nlohmann::json &elem)
    {
        std::size_t id   = Registry::getInstance().addEntity();
        std::string text = Json::isDataExist(elem, "text")
            ? Json::getInstance().getDataFromJson<std::string>(elem, "text")
            : "";
        Raylib::Text textComp(text);
        if (Json::isDataExist(elem, "size")) {
            Types::FontSize fsz({Json::getInstance().getDataFromJson<float>(elem, "size")});
            Registry::getInstance().getComponents<Types::FontSize>().insertBack(fsz);
        }
        if (Json::isDataExist(elem, "color")) {
            auto search = Types::colorMatchStrings.find(
                Json::getInstance().getDataFromJson<std::string>(elem, "color"));
            Raylib::Color color = search != Types::colorMatchStrings.end() ? Types::colorMatchStrings.at(
                                      Json::getInstance().getDataFromJson<std::string>(elem, "color"))
                                                                           : Raylib::White;
            Registry::getInstance().getComponents<Raylib::Color>().insertBack(color);
        }
        if (Json::isDataExist(elem, "position")) {
            Types::Position position = {
                Types::Position(Json::getInstance().getDataFromJson<Types::Position>(elem, "position"))};
            Registry::getInstance().getComponents<Types::Position>().insertBack(position);
        }
        Registry::getInstance().getComponents<Raylib::Text>().insertBack(textComp);
        return (id);
    }

    static std::size_t initButton(nlohmann::json &elem, std::function<void()> &callback)
    {
        if (Json::isDataExist(elem, "spritePath")) {
            return (initButtonFromSprite(elem, callback));
        } else {
            return (initButtonFromRectangle(elem, callback));
        }
    }

    // must be an element only no array allowed
    std::size_t MenuBuilder::initMenuEntity(nlohmann::json &elem)
    {
        if (elem.is_array()) {
            throw std::runtime_error("Error while loading entity : data is array");
        }
        std::function<void()> callback =
            Callback::callbacks.at(elem["callback"].get<Callback::CallbackType>());

        switch (Json::getInstance().getDataFromJson<ObjectType>(elem, "type")) {
            case ObjectType::BUTTON: return (initButton(elem, callback));
            case ObjectType::TEXT: return (initText(elem));
            case ObjectType::INPUT_BOX: return (initInputBox(elem));
            case ObjectType::SPRITE: return (initSpriteFromJson(elem));
            default: Logger::error("Object type is undefined, check your json data"); return (0);
        }
    }

    // Must be an array json data
    void MenuBuilder::initMenuSceneEntity(nlohmann::json sceneMenuData)
    {
        for (auto it : sceneMenuData) {
            try {
                initMenuEntity(it);
            } catch (std::runtime_error &err) {
                Logger::error(
                    "Counldn't load menu correctly, verify your json data : " + std::string(err.what()));
                throw std::runtime_error("Cannot load scene menu correctly.");
            }
        }
    }

    bool checkClick(std::size_t &idEntity)
    {
        std::vector<std::size_t> ids = Registry::getInstance().getEntitiesByComponents(
            {typeid(Types::CollisionRect), typeid(Types::Position)});

        for (auto id : ids) {
            if (checkIsInsideRect(id)) {
                idEntity = id;
                return (true);
            }
        }
        return (false);
    }

    bool checkIsInsideRect(const std::size_t &id)
    {
        Registry::components<Types::CollisionRect> arrCollisionRect =
            Registry::getInstance().getComponents<Types::CollisionRect>();
        Registry::components<Types::Position> arrPosition =
            Registry::getInstance().getComponents<Types::Position>();

        if (!arrCollisionRect.exist(id) || !arrPosition.exist(id)) {
            return (false);
        }
        Raylib::Rectangle rect(
            Maths::intToFloatConservingDecimals(arrPosition[id].x),
            Maths::intToFloatConservingDecimals(arrPosition[id].y),
            Maths::intToFloatConservingDecimals(arrCollisionRect[id].width),
            Maths::intToFloatConservingDecimals(arrCollisionRect[id].height));
        Raylib::Vector2 mousePos(
            Raylib::MouseInput::getMousePosition().x,
            Raylib::MouseInput::getMousePosition().y);

        mousePos.x = (mousePos.x * maxPercent) / Raylib::Window::getScreenWidth();
        mousePos.y = (mousePos.y * maxPercent) / Raylib::Window::getScreenHeight();

        return Raylib::checkCollisionPointRec(mousePos, rect);
    }

    MenuBuilder &MenuBuilder::getInstance()
    {
        return _instance;
    }
} // namespace Menu
