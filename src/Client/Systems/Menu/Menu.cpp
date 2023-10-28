/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Menu
*/

#include "Menu.hpp"
#include "CustomTypes.hpp"
#include "Maths.hpp"
#include "NitworkClient.hpp"
#include "Raylib.hpp"
#include "SystemManager.hpp"
#include "SceneManager.hpp"
#include "ButtonCallbacks.hpp"

namespace Menu {
    // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
    MenuBuilder MenuBuilder::_instance = MenuBuilder();
    // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

    constexpr float maxPercent = 100.0F;

    static void initFromSprite(nlohmann::json &elem)
    {
        Types::SpriteDatas spriteDatas(
            Json::getInstance().getDataFromJson(elem, "spritePath"),
            Json::getInstance().getDataFromJson(elem, "width"),
            Json::getInstance().getDataFromJson(elem, "height"),
            FRONTLAYER,
            static_cast<std::size_t>(FRONT));
        Types::Rect rect = {Types::Rect(Json::getInstance().getDataFromJson(elem, "rect"))};
        nlohmann::basic_json<> animRectData = Json::getInstance().getDataFromJson(elem, "animRect");
        Types::AnimRect animRect(rect, animRectData);
        Registry::getInstance().getComponents<Types::AnimRect>().insertBack(animRect);
        Registry::getInstance().getComponents<Types::SpriteDatas>().insertBack(spriteDatas);
        Registry::getInstance().getComponents<Types::Rect>().insertBack(rect);
    }

    static void initInputBox(nlohmann::json &elem)
    {
        std::size_t id = Registry::getInstance().addEntity();

        Types::Position position(Json::getInstance().getDataFromJson(elem, "position"));
        Types::CollisionRect collisionRect(Json::getInstance().getDataFromJson(elem, "collisionRect"));
        Types::FontSize fsz({Json::getInstance().getDataFromJson(elem, "textSize")});
        std::string text = Json::getInstance().isDataExist(elem, "text")
            ? Json::getInstance().getDataFromJson(elem, "text")
            : "";
        std::string name = Json::getInstance().isDataExist(elem, "name")
            ? Json::getInstance().getDataFromJson(elem, "name")
            : "";
        Raylib::Text textComp(text);
        std::size_t maxChar(Json::getInstance().getDataFromJson(elem, "maxChar"));
        Types::InputBox inputBox(text, name, maxChar);
        auto search = Types::colorMatchStrings.find(Json::getInstance().getDataFromJson(elem, "color").get<std::string>());

        Raylib::Color color = search != Types::colorMatchStrings.end() ? Types::colorMatchStrings.at(Json::getInstance().getDataFromJson(elem, "color")) : Raylib::White;
        Registry::getInstance().getComponents<Raylib::Color>().insertBack(color);

        if (!Json::getInstance().isDataExist(elem, "spritePath")) {
            Types::RectangleShape rectangle({elem["width"], elem["height"]});
            Registry::getInstance().getComponents<Types::RectangleShape>().insertBack(rectangle);
            Registry::getInstance().setToFrontLayers(id);
        } else {
            initFromSprite(elem);
        }
        Registry::getInstance().getComponents<Types::FontSize>().insertBack(fsz);
        Registry::getInstance().getComponents<Raylib::Text>().insertBack(textComp);
        Registry::getInstance().getComponents<Types::InputBox>().insertBack(inputBox);
        Registry::getInstance().getComponents<Types::Position>().insertBack(position);
        Registry::getInstance().getComponents<Types::CollisionRect>().insertBack(collisionRect);
    }

    static void initButtonFromSprite(nlohmann::json &elem, std::function<void()> &callback)
    {
        nlohmann::basic_json<> animRectData = Json::getInstance().getDataFromJson(elem, "animRect");
        Types::Button button(callback);

        Registry::getInstance().addEntity();
        Types::SpriteDatas spriteDatas(
            Json::getInstance().getDataFromJson(elem, "spritePath"),
            Json::getInstance().getDataFromJson(elem, "width"),
            Json::getInstance().getDataFromJson(elem, "height"),
            FRONTLAYER,
            static_cast<std::size_t>(FRONT));
        Types::Rect rect         = {Types::Rect(Json::getInstance().getDataFromJson(elem, "rect"))};
        Types::Position position = {Types::Position(Json::getInstance().getDataFromJson(elem, "position"))};
        Types::CollisionRect collisionRect = {
            Types::CollisionRect(Json::getInstance().getDataFromJson(elem, "collisionRect"))};
        Types::AnimRect animRect(rect, animRectData, Types::RectListType::MOVE);

        Registry::getInstance().getComponents<Types::Position>().insertBack(position);
        Registry::getInstance().getComponents<Types::Rect>().insertBack(rect);
        Registry::getInstance().getComponents<Types::AnimRect>().insertBack(animRect);
        Registry::getInstance().getComponents<Types::CollisionRect>().insertBack(collisionRect);
        Registry::getInstance().getComponents<Types::SpriteDatas>().insertBack(spriteDatas);
        Registry::getInstance().getComponents<Types::Button>().insertBack(button);
    }

    static void initButtonFromRectangle(nlohmann::json &elem, std::function<void()> &callback)
    {
        std::size_t id = Registry::getInstance().addEntity();

        Types::Position position = {Types::Position(Json::getInstance().getDataFromJson(elem, "position"))};
        Types::CollisionRect collisionRect = {
            Types::CollisionRect(Json::getInstance().getDataFromJson(elem, "collisionRect"))};
        Types::Button button(callback);

        Types::RectangleShape rectangle(
            {static_cast<float>(collisionRect.width), static_cast<float>(collisionRect.height)});
        Registry::getInstance().getComponents<Types::RectangleShape>().insertBack(rectangle);
        Registry::getInstance().getComponents<Types::Position>().insertBack(position);
        Registry::getInstance().getComponents<Types::CollisionRect>().insertBack(collisionRect);
        Registry::getInstance().getComponents<Types::Button>().insertBack(button);
        Registry::getInstance().setToFrontLayers(id);
    }

    static void initButton(nlohmann::json &elem, std::function<void()> &callback)
    {
        if (Json::isDataExist(elem, "spritePath")) {
            initButtonFromSprite(elem, callback);
        } else {
            initButtonFromRectangle(elem, callback);
        }
    }

    void MenuBuilder::initMenuEntity(nlohmann::json &elem, std::function<void()> callback)
    {
        switch (elem["type"].get<ObjectType>()) {
            case ObjectType::BUTTON: initButton(elem, callback); break;
            case ObjectType::TEXT: break; // no clickable text for now
            case ObjectType::INPUT_BOX: initInputBox(elem); break;
            default: Logger::error("Object type is undefined, check your json data"); break;
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
        Raylib::Vector2 mousePos(Raylib::getMousePosition().x, Raylib::getMousePosition().y);

        mousePos.x = (mousePos.x * maxPercent) / Raylib::getScreenWidth();
        mousePos.y = (mousePos.y * maxPercent) / Raylib::getScreenHeight();

        return Raylib::checkCollisionPointRec(mousePos, rect);
    }

    MenuBuilder &MenuBuilder::getInstance()
    {
        return _instance;
    }
} // namespace Menu
