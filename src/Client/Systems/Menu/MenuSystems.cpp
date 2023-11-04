/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Menu
*/

#include "MenuSystems.hpp"
#include "B-luga-graphics/AnimRect.hpp"
#include "B-luga-graphics/GraphicsCustomTypes.hpp"
#include "B-luga-physics/ECSCustomTypes.hpp"
#include "B-luga/Maths/Maths.hpp"
#include "B-luga/SceneManager.hpp"
#include "Menu.hpp"
#include "Parallax.hpp"
#include "ResourcesManager.hpp"
#include "init.hpp"

namespace Systems {
    namespace Menu {

        constexpr float maxPercent = 100.0F;

        static void setAllInputBoxFalse()
        {
            Registry::components<Types::InputBox> arrInputBox =
                Registry::getInstance().getComponents<Types::InputBox>();
            std::vector<std::size_t> ids =
                Registry::getInstance().getEntitiesByComponents({typeid(Types::InputBox)});

            for (auto id : ids) {
                if (arrInputBox[id].selected) {
                    arrInputBox[id].selected = false;
                }
            }
        }

        static void setInputBoxSelected(const std::size_t &id)
        {
            Registry::components<Types::InputBox> arrInputBox =
                Registry::getInstance().getComponents<Types::InputBox>();

            setAllInputBoxFalse();
            if (arrInputBox.exist(id)) {
                arrInputBox[id].selected = true;
            }
        }

        static void insertText(std::size_t id, Registry::components<Types::InputBox> &arrInputBox)
        {
            Registry::components<Raylib::TextShared> arrText =
                Registry::getInstance().getComponents<Raylib::TextShared>();
            int key = Raylib::KeyboardInput::getCharPressed();

            if ((key >= ' ') && (key <= '}') && (arrInputBox[id].text.size() < arrInputBox[id].maxChar)) {
                arrInputBox[id].text += static_cast<char>(key);
                arrText[id]->setCurrentText(arrInputBox[id].text);
            }
        }

        void checkTextInput(std::size_t /*unused*/, std::size_t /*unused*/)
        {
            Registry::components<Types::InputBox> arrInputBox =
                Registry::getInstance().getComponents<Types::InputBox>();
            std::vector<std::size_t> ids = Registry::getInstance().getEntitiesByComponents(
                {typeid(Types::InputBox), typeid(Raylib::TextShared)});

            for (auto id : ids) {
                if (arrInputBox[id].selected == true) {
                    insertText(id, arrInputBox);
                }
            }
        }

        void manageInputBox(std::size_t, std::size_t)
        {
            std::size_t idEntity = 0;

            if (Raylib::MouseInput::isMouseButtonPressed(Raylib::MouseButton::MOUSE_BTN_LEFT)) {
                if (!::Menu::checkClick(idEntity)) {
                    setAllInputBoxFalse();
                    return;
                }
                setInputBoxSelected(idEntity);
            }
        }

        static void deleteInputBoxChar(std::size_t id, Registry::components<Types::InputBox> &arrInputBox)
        {
            auto &arrText = Registry::getInstance().getComponents<Raylib::TextShared>();

            if (arrInputBox[id].text.size() > 0) {
                arrInputBox[id].text.pop_back();
                arrText[id]->setCurrentText(arrInputBox[id].text);
            }
        }

        void checkInputDeletion(std::size_t, std::size_t)
        {
            Registry::components<Types::InputBox> arrInputBox =
                Registry::getInstance().getComponents<Types::InputBox>();
            std::vector<std::size_t> ids = Registry::getInstance().getEntitiesByComponents(
                {typeid(Types::InputBox), typeid(Raylib::TextShared)});

            for (auto id : ids) {
                if (arrInputBox[id].selected
                    && Raylib::KeyboardInput::isKeyPressed(Raylib::KeyboardKey::KB_BACKSPACE)) {
                    deleteInputBoxChar(id, arrInputBox);
                }
            }
        }

        void hoverInputBox(std::size_t /*unused*/, std::size_t /*unused*/)
        {
            Registry::components<Types::AnimRect> arrAnimRect =
                Registry::getInstance().getComponents<Types::AnimRect>();
            std::vector<std::size_t> ids = Registry::getInstance().getEntitiesByComponents(
                {typeid(Types::CollisionRect), typeid(Types::Position), typeid(Types::InputBox)});

            for (auto id : ids) {
                if (::Menu::checkIsInsideRect(id)) {
                    if (arrAnimRect.exist(id)) {
                        arrAnimRect[id].changeRectList(Types::RectListType::HOVER);
                    }
                    // Raylib::setMouseCursor(MOUSE_CURSOR_IBEAM);
                    return;
                }
                // Raylib::setMouseCursor(MOUSE_CURSOR_DEFAULT);
                if (arrAnimRect.exist(id)) {
                    arrAnimRect[id].changeRectList(Types::RectListType::UNDEFINED);
                }
            }
        }

        void pressButton(std::size_t /*unused*/, std::size_t /*unused*/)
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
                if (::Menu::checkIsInsideRect(id)) {
                    arrAnimRect[id].changeRectList(Types::RectListType::HOVER);
                    if (Raylib::MouseInput::isMouseButtonPressed(Raylib::MouseButton::MOUSE_BTN_LEFT)) {
                        arrButton[id].callback();
                    }
                    return;
                }
                arrAnimRect[id].changeRectList(Types::RectListType::UNDEFINED);
            }
        }

        void initMenu(std::size_t managerId, std::size_t systemId)
        {
            if (Scene::SceneManager::getInstance().getCurrentScene() != Scenes::MENU) {
                SystemManagersDirector::getInstance().getSystemManager(managerId).removeSystem(systemId);
                return;
            }
            Parallax::initParalax();
            nlohmann::json connectButton = Json::getInstance().getDataByVector(
                ResourcesManager::getPathByJsonType(JsonType::MENU_DATA),
                {"menu", "connect"});
            nlohmann::json inputBoxIp = Json::getInstance().getDataByVector(
                ResourcesManager::getPathByJsonType(JsonType::MENU_DATA),
                {"menu", "ip"});
            nlohmann::json inputBoxHost = Json::getInstance().getDataByVector(
                ResourcesManager::getPathByJsonType(JsonType::MENU_DATA),
                {"menu", "host"});

            try {
                ::Menu::MenuBuilder::getInstance().initMenuEntity(
                    connectButton,
                    ::Menu::Callback::initConnection);
                ::Menu::MenuBuilder::getInstance().initMenuEntity(inputBoxHost);
                ::Menu::MenuBuilder::getInstance().initMenuEntity(inputBoxIp);
            } catch (std::runtime_error &err) {
                Logger::error(
                    "Counldn't load menu correctly, verify your json data : " + std::string(err.what()));
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
