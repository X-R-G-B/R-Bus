/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Menu
*/

#include "Raylib.hpp"
#include "CustomTypes.hpp"
#include "Menu.hpp"
#include "Maths.hpp"
#include "NitworkClient.hpp"
#include "SceneManager.hpp"
#include "EventsSystems.hpp"

namespace Systems {
    namespace Menu {

        constexpr float maxPercent = 100.0F;

        static void initInputBox(nlohmann::json &elem)
        {
            Registry::getInstance().addEntity();
            
            Types::RectangleShape rectangle({elem["width"], elem["height"]});
            Types::Position position(Json::getInstance().getDataFromJson(elem, "position"));
            Types::CollisionRect collisionRect(Json::getInstance().getDataFromJson(elem, "collisionRect"));

            Registry::getInstance().getComponents<Types::Position>().insertBack(position);
            Registry::getInstance().getComponents<Types::CollisionRect>().insertBack(collisionRect);
            Registry::getInstance().getComponents<Types::RectangleShape>().insertBack(rectangle);
            std::cout << "Ntmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm" << std::endl;
        }

        static void initButtonFromSprite(nlohmann::json &elem, std::function<void()> &callback)
        {
            nlohmann::basic_json<> animRectData =
                Json::getInstance().getDataFromJson(elem, "animRect");
            Types::Button button(callback);

            Registry::getInstance().addEntity();
            Types::SpriteDatas spriteDatas(
                Json::getInstance().getDataFromJson(elem, "spritePath"),
                Json::getInstance().getDataFromJson(elem, "width"),
                Json::getInstance().getDataFromJson(elem, "height"),
                FRONTLAYER,
            static_cast<std::size_t>(FRONT));
            Types::Rect rect = {
                Types::Rect(Json::getInstance().getDataFromJson(elem, "rect"))};
            Types::Position position = {
            Types::Position(Json::getInstance().getDataFromJson(elem, "position"))};
            Types::CollisionRect collisionRect = {Types::CollisionRect(
                Json::getInstance().getDataFromJson(elem, "collisionRect"))};
            Types::AnimRect animRect(rect, animRectData, Types::RectListType::MOVE);

            Registry::getInstance().getComponents<Types::Position>().insertBack(position);
            Registry::getInstance().getComponents<Types::Rect>().insertBack(rect);
            Registry::getInstance().getComponents<Types::AnimRect>().insertBack(animRect);
            Registry::getInstance().getComponents<Types::CollisionRect>().insertBack(collisionRect);
            Registry::getInstance().getComponents<Types::SpriteDatas>().insertBack(spriteDatas);
            Registry::getInstance().getComponents<Types::Button>().insertBack(button);
        }

        static void initButtonFromRectangle(nlohmann::json &elem)
        {
            std::cout << "Button from rect" << std::endl;
        }

        static void initButton(nlohmann::json &elem, Material mat, std::function<void()> &callback)
        {
            switch (mat)
            {
            case Material::RECTANGLE :
                initButtonFromRectangle(elem);
                break;
            case Material::SPRITE :
                initButtonFromSprite(elem, callback);
                break;
            default:
                initButtonFromRectangle(elem);
                break;
            }
        }

        static void checkType(nlohmann::json &elem, ObjectType type, std::function<void()> callback)
        {
            switch (type)
            {
                case ObjectType::BUTTON :
                    initButton(elem, elem["from"].get<Material>(), callback);
                    break;
                case ObjectType::TEXT :
                    break;
                case ObjectType::INPUT_BOX :
                    initInputBox(elem);
                    break;
                default:
                    break;
            }
        }

        void checkMousePosition(std::size_t, std::size_t)
        {
            Registry::components<Types::CollisionRect> arrCollisionRect =
                Registry::getInstance().getComponents<Types::CollisionRect>();
            Registry::components<Types::Position> arrPosition =
                Registry::getInstance().getComponents<Types::Position>();
            Registry::components<Types::AnimRect> arrAnimRect =
                Registry::getInstance().getComponents<Types::AnimRect>();
            Registry::components<Types::Button> arrButton =
                Registry::getInstance().getComponents<Types::Button>();
            std::vector<std::size_t> ids = Registry::getInstance().getEntitiesByComponents({
                typeid(Types::CollisionRect), typeid(Types::AnimRect), typeid(Types::Position), typeid(Types::Button)
            });

            for (auto id : ids) {
                Raylib::Rectangle rect(Maths::intToFloatConservingDecimals(arrPosition[id].x), Maths::intToFloatConservingDecimals(arrPosition[id].y), Maths::intToFloatConservingDecimals(arrCollisionRect[id].width), Maths::intToFloatConservingDecimals(arrCollisionRect[id].height));
                Raylib::Vector2 mousePos(Raylib::getMousePosition().x, Raylib::getMousePosition().y);

                mousePos.x = (mousePos.x * maxPercent) / Raylib::getScreenWidth();
                mousePos.y = (mousePos.y * maxPercent) / Raylib::getScreenHeight();

                if (Raylib::checkCollisionPointRec(mousePos, rect)) {
                    arrAnimRect[id].changeRectList(Types::RectListType::HOVER);
                    if (Raylib::isMouseButtonPressed(Raylib::MouseButton::MOUSE_BTN_LEFT)) {
                        arrButton[id].callback();
                    }
                    return;
                }
                arrAnimRect[id].changeRectList(Types::RectListType::UNDEFINED);
            }
        }

        static void toCall()
        {
            Nitwork::NitworkClient::getInstance().addInitMsg();
            Nitwork::NitworkClient::getInstance().addReadyMsg();
            Scene::SceneManager::getInstance().changeScene(Scene::Scene::MAIN_GAME);
        }

        void initMenu(std::size_t managerId, std::size_t systemId)
        {
            nlohmann::json connectButton = Json::getInstance().getDataByVector({"menu", "connect"}, JsonType::MENU);
            nlohmann::json inputBox = Json::getInstance().getDataByVector({"menu", "ip"}, JsonType::MENU);

            checkType(connectButton, connectButton["type"].get<ObjectType>(), toCall);
            checkType(inputBox, inputBox["type"].get<ObjectType>(), toCall);
            SystemManagersDirector::getInstance().getSystemManager(managerId).removeSystem(systemId);
        }

        std::vector<std::function<void(std::size_t, std::size_t)>> getMenuSystems()
        {
            return {initMenu, checkMousePosition, Systems::EventsSystems::changeScene};
        }
    }
}
