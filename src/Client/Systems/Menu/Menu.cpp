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

    static void initFromSprite(nlohmann::json &elem)
    {
        Types::SpriteDatas spriteDatas(
            Json::getInstance().getDataFromJson<std::string>(elem, "spritePath"),
            Json::getInstance().getDataFromJson<int>(elem, "width"),
            Json::getInstance().getDataFromJson<int>(elem, "height"),
            FRONTLAYER,
            static_cast<std::size_t>(FRONT));
        Types::Rect rect = {Types::Rect(Json::getInstance().getDataFromJson<Types::Rect>(elem, "rect"))};
        nlohmann::basic_json<> animRectData =
            Json::getInstance().getDataFromJson<nlohmann::basic_json<>>(elem, "animRect");
        Types::AnimRect animRect(rect, animRectData);
        Registry::getInstance().getComponents<Types::AnimRect>().insertBack(animRect);
        Registry::getInstance().getComponents<Types::SpriteDatas>().insertBack(spriteDatas);
        Registry::getInstance().getComponents<Types::Rect>().insertBack(rect);
    }

    static void initInputBox(nlohmann::json &elem)
    {
        std::size_t id = Registry::getInstance().addEntity();

        Types::Position position(Json::getInstance().getDataFromJson<Types::Position>(elem, "position"));
        Types::CollisionRect collisionRect(
            Json::getInstance().getDataFromJson<Types::CollisionRect>(elem, "collisionRect"));
        Types::FontSize fsz({Json::getInstance().getDataFromJson<float>(elem, "textSize")});
        std::string text = Json::isDataExist(elem, "text")
            ? Json::getInstance().getDataFromJson<std::string>(elem, "text")
            : "";
        std::string name = Json::isDataExist(elem, "name")
            ? Json::getInstance().getDataFromJson<std::string>(elem, "name")
            : "";
        auto textComp    = Raylib::Text::fromText(text);
        std::size_t maxChar(Json::getInstance().getDataFromJson<std::size_t>(elem, "maxChar"));
        Types::InputBox inputBox(text, name, maxChar);
        auto search =
            Types::colorMatchStrings.find(Json::getInstance().getDataFromJson<std::string>(elem, "color"));

        Raylib::Color color = search != Types::colorMatchStrings.end()
            ? Types::colorMatchStrings.at(Json::getInstance().getDataFromJson<std::string>(elem, "color"))
            : Raylib::Color(Raylib::ColorDef::White);
        Registry::getInstance().getComponents<Raylib::Color>().insertBack(color);

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
    }

    static void initButtonFromSprite(nlohmann::json &elem, std::function<void()> &callback)
    {
        nlohmann::basic_json<> animRectData =
            Json::getInstance().getDataFromJson<nlohmann::basic_json<>>(elem, "animRect");
        Types::Button button(callback);

        Registry::getInstance().addEntity();
        Types::SpriteDatas spriteDatas(
            Json::getInstance().getDataFromJson<std::string>(elem, "spritePath"),
            Json::getInstance().getDataFromJson<int>(elem, "width"),
            Json::getInstance().getDataFromJson<int>(elem, "height"),
            FRONTLAYER,
            static_cast<std::size_t>(FRONT));
        Types::Rect rect = {Types::Rect(Json::getInstance().getDataFromJson<Types::Rect>(elem, "rect"))};
        Types::Position position = {
            Types::Position(Json::getInstance().getDataFromJson<Types::Position>(elem, "position"))};
        Types::CollisionRect collisionRect = {Types::CollisionRect(
            Json::getInstance().getDataFromJson<Types::CollisionRect>(elem, "collisionRect"))};
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
        switch (Json::getInstance().getDataFromJson<ObjectType>(elem, "type")) {
            case ObjectType::BUTTON: initButton(elem, callback); break;
            case ObjectType::TEXT: break; // no clickable text for now
            case ObjectType::INPUT_BOX: initInputBox(elem); break;
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
