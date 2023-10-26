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
#include "SceneManager.hpp"

namespace Systems {
    namespace Menu {

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
            Raylib::Color color(
                Types::colorMatchStrings.at(Json::getInstance().getDataFromJson(elem, "color")));
            Raylib::Text textComp(text);
            std::size_t maxChar(Json::getInstance().getDataFromJson(elem, "maxChar"));
            Types::InputBox inputBox(text, name, maxChar);

            if (!Json::getInstance().isDataExist(elem, "spritePath")) {
                Types::RectangleShape rectangle({elem["width"], elem["height"]});
                Registry::getInstance().getComponents<Types::RectangleShape>().insertBack(rectangle);
                Registry::getInstance().setToFrontLayers(id);
            } else {
                initFromSprite(elem);
            }
            Registry::getInstance().getComponents<Types::FontSize>().insertBack(fsz);
            Registry::getInstance().getComponents<Raylib::Color>().insertBack(color);
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
            Types::Position position = {
                Types::Position(Json::getInstance().getDataFromJson(elem, "position"))};
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

            Types::Position position = {
                Types::Position(Json::getInstance().getDataFromJson(elem, "position"))};
            Types::CollisionRect collisionRect = {
                Types::CollisionRect(Json::getInstance().getDataFromJson(elem, "collisionRect"))};
            Types::Button button(callback);

            Types::RectangleShape rectangle({static_cast<float>(collisionRect.width), static_cast<float>(collisionRect.height)});
            Registry::getInstance().getComponents<Types::RectangleShape>().insertBack(rectangle);
            Registry::getInstance().getComponents<Types::Position>().insertBack(position);
            Registry::getInstance().getComponents<Types::CollisionRect>().insertBack(collisionRect);
            Registry::getInstance().getComponents<Types::Button>().insertBack(button);
            Registry::getInstance().setToFrontLayers(id);
        }

        static void initButton(nlohmann::json &elem, Material mat, std::function<void()> &callback)
        {
            switch (mat) {
                case Material::RECTANGLE: initButtonFromRectangle(elem, callback); break;
                case Material::SPRITE: initButtonFromSprite(elem, callback); break;
                default: initButtonFromRectangle(elem, callback); break;
            }
        }

        static void defaultCallBack()
        {
            Logger::debug("Clicked");
        }

        static void
        checkType(nlohmann::json &elem, ObjectType type, std::function<void()> callback = defaultCallBack)
        {
            switch (type) {
                case ObjectType::BUTTON: initButton(elem, elem["from"].get<Material>(), callback); break;
                case ObjectType::TEXT: break;
                case ObjectType::INPUT_BOX: initInputBox(elem); break;
                default: break;
            }
        }

        static bool checkIsInsideRect(const std::size_t &id)
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

        static void setAllInputBoxFalse()
        {
            Registry::components<Types::InputBox> arrInputBox =
                Registry::getInstance().getComponents<Types::InputBox>();
            std::vector<std::size_t> ids =
                Registry::getInstance().getEntitiesByComponents({typeid(Types::InputBox)});

            for (auto id : ids) {
                arrInputBox[id].selected = false;
            }
        }

        static void setInputBoxSelected(const std::size_t &id)
        {
            Registry::components<Types::InputBox> arrInputBox =
                Registry::getInstance().getComponents<Types::InputBox>();

            setAllInputBoxFalse();
            arrInputBox[id].selected = true;
        }

        void checkTextInput(std::size_t, std::size_t)
        {
            Registry::components<Types::InputBox> arrInputBox =
                Registry::getInstance().getComponents<Types::InputBox>();
            Registry::components<Raylib::Text> arrText =
                Registry::getInstance().getComponents<Raylib::Text>();
            std::vector<std::size_t> ids = Registry::getInstance().getEntitiesByComponents(
                {typeid(Types::InputBox), typeid(Raylib::Text)});

            for (auto id : ids) {
                if (arrInputBox[id].selected == true) {
                    int key = GetCharPressed();
                    if ((key >= 32) && (key <= 125)
                        && (arrInputBox[id].text.size() < arrInputBox[id].maxChar)) {
                        arrInputBox[id].text += static_cast<char>(key);
                        arrText[id].setCurrentText(arrInputBox[id].text);
                    }
                }
            }
        }

        static bool checkClick(std::size_t &idEntity)
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

        static bool checkInputBoxGoodId(const std::size_t &idEntity)
        {
            std::vector<std::size_t> ids =
                Registry::getInstance().getEntitiesByComponents({typeid(Types::InputBox)});

            for (auto id : ids) {
                if (id == idEntity) {
                    setInputBoxSelected(idEntity);
                    return (true);
                }
            }
            return (false);
        }

        void manageInputBox(std::size_t, std::size_t)
        {
            std::size_t idEntity = 0;

            if (Raylib::isMouseButtonPressed(Raylib::MouseButton::MOUSE_BTN_LEFT)) {
                if (!checkClick(idEntity)) {
                    setAllInputBoxFalse();
                    return;
                }
                if (checkInputBoxGoodId(idEntity)) {
                    return;
                }
                setAllInputBoxFalse();
            }
        }

        void checkInputDeletion(std::size_t, std::size_t)
        {
            Registry::components<Types::InputBox> arrInputBox =
                Registry::getInstance().getComponents<Types::InputBox>();
            Registry::components<Raylib::Text> arrText =
                Registry::getInstance().getComponents<Raylib::Text>();
            std::vector<std::size_t> ids = Registry::getInstance().getEntitiesByComponents(
                {typeid(Types::InputBox), typeid(Raylib::Text)});

            for (auto id : ids) {
                if (arrInputBox[id].selected && Raylib::isKeyPressed(Raylib::KeyboardKey::KB_BACKSPACE)) {
                    if (arrInputBox[id].text.size() > 0) {
                        arrInputBox[id].text.pop_back();
                        arrText[id].setCurrentText(arrInputBox[id].text);
                    }
                }
            }
        }

        void hoverInputBox(std::size_t, std::size_t)
        {
            Registry::components<Types::AnimRect> arrAnimRect =
                Registry::getInstance().getComponents<Types::AnimRect>();
            std::vector<std::size_t> ids = Registry::getInstance().getEntitiesByComponents(
                {typeid(Types::CollisionRect), typeid(Types::Position), typeid(Types::InputBox)});

            for (auto id : ids) {
                if (checkIsInsideRect(id)) {
                    if (arrAnimRect.exist(id)) {
                        arrAnimRect[id].changeRectList(Types::RectListType::HOVER);
                    }
                    Raylib::setMouseCursor(MOUSE_CURSOR_IBEAM);
                    return;
                }
                Raylib::setMouseCursor(MOUSE_CURSOR_DEFAULT);
                if (arrAnimRect.exist(id)) {
                    arrAnimRect[id].changeRectList(Types::RectListType::UNDEFINED);
                }
            }
        }

        void pressButton(std::size_t, std::size_t)
        {
            Registry::components<Types::AnimRect> arrAnimRect =
                Registry::getInstance().getComponents<Types::AnimRect>();
            Registry::components<Types::Button> arrButton =
                Registry::getInstance().getComponents<Types::Button>();
            std::vector<std::size_t> ids = Registry::getInstance().getEntitiesByComponents(
                {typeid(Types::CollisionRect),
                 typeid(Types::AnimRect),
                 typeid(Types::Position),
                 typeid(Types::Button)});

            for (auto id : ids) {
                if (checkIsInsideRect(id)) {
                    arrAnimRect[id].changeRectList(Types::RectListType::HOVER);
                    Raylib::setMouseCursor(MOUSE_CURSOR_ARROW);
                    if (Raylib::isMouseButtonPressed(Raylib::MouseButton::MOUSE_BTN_LEFT)) {
                        arrButton[id].callback();
                    }
                    return;
                }
                Raylib::setMouseCursor(MOUSE_CURSOR_DEFAULT);
                arrAnimRect[id].changeRectList(Types::RectListType::UNDEFINED);
            }
        }

        constexpr int PORT_MIN = 0;
        constexpr int PORT_MAX = 65535;

        static bool isNumber(const std::string &str)
        {
            return std::all_of(str.begin(), str.end(), ::isdigit);
        }

        static bool checkArgs(const std::string &ip, const std::string &port)
        {
            if (ip.empty()) {
                Logger::error("Invalid ip");
                return false;
            }
            if (!isNumber(port) || std::stoi(port) < PORT_MIN || std::stoi(port) > PORT_MAX) {
                Logger::error("Invalid port");
                return false;
            }
            return true;
        }

        static void getIpAndPort(std::string &ip, std::string &port)
        {
            Registry::components<Types::InputBox> arrInputBox =
                Registry::getInstance().getComponents<Types::InputBox>();
            std::vector<std::size_t> ids =
                Registry::getInstance().getEntitiesByComponents({typeid(Types::InputBox)});

            for (auto id : ids) {
                if (arrInputBox[id].name == "ip") {
                    ip = arrInputBox[id].text;
                }
                if (arrInputBox[id].name == "port") {
                    port = arrInputBox[id].text;
                }
            }
        }

        static void toCall()
        {
            std::string ip("");
            std::string port("");

            getIpAndPort(ip, port);
            if (!checkArgs(ip, port)) {
                return;
            }
            if (!Nitwork::NitworkClient::getInstance()
                     .startClient(std::stoi(port.c_str()), ip.c_str(), DEFAULT_THREAD_NB, TICKS)) {
                Logger::error("Error network couldn't connect");
                return;
            }
            Nitwork::NitworkClient::getInstance().addInitMsg();
            Nitwork::NitworkClient::getInstance().addReadyMsg();
            Scene::SceneManager::getInstance().changeScene(Scene::Scene::MAIN_GAME);
        }

        void initMenu(std::size_t managerId, std::size_t systemId)
        {
            nlohmann::json connectButton =
                Json::getInstance().getDataByVector({"menu", "connect"}, JsonType::MENU);
            nlohmann::json inputBoxIp = Json::getInstance().getDataByVector({"menu", "ip"}, JsonType::MENU);
            nlohmann::json inputBoxHost =
                Json::getInstance().getDataByVector({"menu", "host"}, JsonType::MENU);

            try {
                checkType(connectButton, connectButton["type"].get<ObjectType>(), toCall);
                checkType(inputBoxHost, inputBoxHost["type"].get<ObjectType>(), toCall);
                checkType(inputBoxIp, inputBoxIp["type"].get<ObjectType>(), toCall);
            } catch (std::runtime_error &err) {
                err.what();
                Logger::error("Counldn't load menu correctly, verify your json data");
            }
            SystemManagersDirector::getInstance().getSystemManager(managerId).removeSystem(systemId);
        }

        std::vector<std::function<void(std::size_t, std::size_t)>> getMenuSystems()
        {
            return {
                initMenu,
                pressButton,
                manageInputBox,
                hoverInputBox,
                checkTextInput,
                checkInputDeletion};
        }
    } // namespace Menu
} // namespace Systems
