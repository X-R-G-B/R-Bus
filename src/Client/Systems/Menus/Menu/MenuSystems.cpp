/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Menu
*/

#include "MenuSystems.hpp"
#include "CustomTypes.hpp"
#include "Maths.hpp"
#include "Menu.hpp"
#include "SceneManager.hpp"
#include "SelectLobbySystems.hpp"
#include "Systems.hpp"

namespace Systems {
    namespace Menu {

        constexpr float maxPercent = 100.0F;

        static void setAllInputBoxFalse()
        {
            Registry::components<Types::InputBox> arrInputBox =
                Registry::getInstance().getComponents<Types::InputBox>();
            Registry::components<Types::AnimRect> arrAnimRect =
                Registry::getInstance().getComponents<Types::AnimRect>();
            std::vector<std::size_t> ids =
                Registry::getInstance().getEntitiesByComponents({typeid(Types::InputBox)});

            for (auto id : ids) {
                if (arrInputBox[id].selected) {
                    if (arrAnimRect.exist(id)) {
                        arrAnimRect[id].changeRectList(Types::RectListType::DEFAULT_RECT);
                    }
                    arrInputBox[id].selected = false;
                }
            }
        }

        static void setInputBoxSelected(const std::size_t &id)
        {
            Registry::components<Types::InputBox> arrInputBox =
                Registry::getInstance().getComponents<Types::InputBox>();
            Registry::components<Types::Button> arrButton =
                Registry::getInstance().getComponents<Types::Button>();
            Registry::components<Types::AnimRect> arrAnimRect =
                Registry::getInstance().getComponents<Types::AnimRect>();

            if (!arrButton.exist(id)) {
                setAllInputBoxFalse();
            }
            if (arrInputBox.exist(id)) {
                arrInputBox[id].selected = true;
                if (arrAnimRect.exist(id)) {
                    arrAnimRect[id].changeRectList(Types::RectListType::SELECTED);
                }
            }
        }

        static void insertText(std::size_t id, Registry::components<Types::InputBox> &arrInputBox)
        {
            Registry::components<Raylib::Text> arrText =
                Registry::getInstance().getComponents<Raylib::Text>();
            int key = Raylib::getCharPressed();

            if ((key >= ' ') && (key <= '}') && (arrInputBox[id].text.size() < arrInputBox[id].maxChar)) {
                arrInputBox[id].text += static_cast<char>(key);
                arrText[id].setCurrentText(arrInputBox[id].text);
            }
        }

        void checkTextInput(std::size_t /*unused*/, std::size_t /*unused*/)
        {
            Registry::components<Types::InputBox> arrInputBox =
                Registry::getInstance().getComponents<Types::InputBox>();
            std::vector<std::size_t> ids = Registry::getInstance().getEntitiesByComponents(
                {typeid(Types::InputBox), typeid(Raylib::Text)});

            for (auto id : ids) {
                if (arrInputBox[id].selected == true) {
                    insertText(id, arrInputBox);
                }
            }
        }

        void manageInputBox(std::size_t, std::size_t)
        {
            std::size_t idEntity = 0;

            if (Raylib::isMouseButtonPressed(Raylib::MouseButton::MOUSE_BTN_LEFT)) {
                ::Menu::checkClick(idEntity);
                setInputBoxSelected(idEntity);
                return;
            }
        }

        static void deleteInputBoxChar(std::size_t id, Registry::components<Types::InputBox> &arrInputBox)
        {
            Registry::components<Raylib::Text> arrText =
                Registry::getInstance().getComponents<Raylib::Text>();

            if (arrInputBox[id].text.size() > 0) {
                arrInputBox[id].text.pop_back();
                arrText[id].setCurrentText(arrInputBox[id].text);
            }
        }

        void checkInputDeletion(std::size_t, std::size_t)
        {
            constexpr std::size_t delay = 150;
            Registry::components<Types::InputBox> arrInputBox =
                Registry::getInstance().getComponents<Types::InputBox>();
            std::vector<std::size_t> ids = Registry::getInstance().getEntitiesByComponents(
                {typeid(Types::InputBox), typeid(Raylib::Text)});
            static auto clockId = Registry::getInstance().getClock().create();

            if (Raylib::isKeyDown(Raylib::KeyboardKey::KB_BACKSPACE) && Registry::getInstance().getClock().elapsedMillisecondsSince(clockId) > delay) {
                for (auto id : ids) {
                    if (arrInputBox[id].selected) {
                        Registry::getInstance().getClock().restart(clockId);
                        deleteInputBoxChar(id, arrInputBox);
                        return;
                    }
                }
            }
        }

        void hoverInputBox(std::size_t /*unused*/, std::size_t /*unused*/)
        {
            Registry::components<Types::AnimRect> arrAnimRect =
                Registry::getInstance().getComponents<Types::AnimRect>();
            Registry::components<Types::InputBox> arrInputBox =
                Registry::getInstance().getComponents<Types::InputBox>();
            std::vector<std::size_t> ids = Registry::getInstance().getEntitiesByComponents(
                {typeid(Types::CollisionRect), typeid(Types::Position), typeid(Types::InputBox)});

            for (auto id : ids) {
                if (::Menu::checkIsInsideRect(id)) {
                    if (arrAnimRect.exist(id) && !arrInputBox[id].selected) {
                        arrAnimRect[id].changeRectList(Types::RectListType::HOVER);
                    }
                    return;
                }
                if (arrAnimRect.exist(id) && !arrInputBox[id].selected) {
                    arrAnimRect[id].changeRectList(Types::RectListType::DEFAULT_RECT);
                }
            }
        }

        static void manageClick(std::size_t id, Registry::components<Types::AnimRect> &arrAnimRect)
        {
            Registry::components<Types::Button> arrButton =
                Registry::getInstance().getComponents<Types::Button>();

            arrAnimRect[id].changeRectList(Types::RectListType::HOVER);
            if (Raylib::isMouseButtonDown(Raylib::MouseButton::MOUSE_BTN_LEFT)) {
                arrAnimRect[id].changeRectList(Types::RectListType::SELECTED);
            }
            if (Raylib::isMouseButtonReleased(Raylib::MouseButton::MOUSE_BTN_LEFT)) {
                arrButton[id].callback();
            }
        }

        void pressButton(std::size_t /*unused*/, std::size_t /*unused*/)
        {
            Registry::components<Types::AnimRect> arrAnimRect =
                Registry::getInstance().getComponents<Types::AnimRect>();

            std::vector<std::size_t> ids = Registry::getInstance().getEntitiesByComponents(
                {typeid(Types::CollisionRect),
                 typeid(Types::AnimRect),
                 typeid(Types::Position),
                 typeid(Types::Button)});

            for (auto id : ids) {
                if (::Menu::checkIsInsideRect(id)) {
                    manageClick(id, arrAnimRect);
                    return;
                }
                arrAnimRect[id].changeRectList(Types::RectListType::UNDEFINED);
            }
        }

        void initMenu(std::size_t managerId, std::size_t systemId)
        {
            if (Scene::SceneManager::getInstance().getCurrentScene() != Scene::Scene::MENU) {
                SystemManagersDirector::getInstance().getSystemManager(managerId).removeSystem(systemId);
                return;
            }
            try {
                nlohmann::json jsonData = Json::getInstance().getDataByJsonType<nlohmann::json>("menu", JsonType::MENU);
                ::Menu::MenuBuilder::getInstance().initMenuSceneEntity(Json::getInstance().getDatasFromList(jsonData));

            } catch (std::runtime_error &err) {
                Logger::info(err.what());
            }
            SystemManagersDirector::getInstance().getSystemManager(managerId).removeSystem(systemId);
        }

        void quitScene(std::size_t /*unused*/, std::size_t /*unused*/)
        {
            if (Raylib::isKeyPressed(Raylib::KeyboardKey::KB_ESCAPE)) {
                switch (Scene::SceneManager::getInstance().getCurrentScene()) {
                    case Scene::Scene::MENU: Scene::SceneManager::getInstance().stop(); break;
                    case Scene::Scene::CREATE_LOBBY:
                        Scene::SceneManager::getInstance().changeScene(Scene::Scene::SELECT_LOBBY);
                        break;
                    case Scene::Scene::SELECT_LOBBY:
                        Scene::SceneManager::getInstance().changeScene(Scene::Scene::MENU);
                        break;
                    case Scene::Scene::MAIN_GAME:
                        Scene::SceneManager::getInstance().changeScene(Scene::Scene::SELECT_LOBBY);
                        break;
                    case Scene::Scene::SCENE_MAX: break;
                }
            }
        }

        std::vector<std::function<void(std::size_t, std::size_t)>> getMenuSystems()
        {
            return {
                initMenu,
                pressButton,
                manageInputBox,
                hoverInputBox,
                checkTextInput,
                checkInputDeletion,
                Systems::moveEntities,
                quitScene};
        }
    } // namespace Menu
} // namespace Systems
