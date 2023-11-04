/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Menu
*/

#include "MenuSystems.hpp"
#include "B-luga-graphics/Raylib/Graphics/Graphics.hpp"
#include "B-luga-graphics/AnimRect.hpp"
#include "B-luga-graphics/GraphicsCustomTypes.hpp"
#include "B-luga-physics/ECSCustomTypes.hpp"
#include "B-luga/Maths/Maths.hpp"
#include "B-luga/SceneManager.hpp"
#include "Menu.hpp"
#include "SelectLobbySystems.hpp"
#include "Parallax.hpp"
#include "ResourcesManager.hpp"
#include "init.hpp"

namespace Systems {
    namespace Menu {

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
            Registry::components<::Systems::SelectLobbySystems::LobbyStatus> arrLobbyStatus =
                Registry::getInstance().getComponents<::Systems::SelectLobbySystems::LobbyStatus>();
            std::vector<std::size_t> ids = Registry::getInstance().getEntitiesByComponents(
                {typeid(Types::InputBox), typeid(Raylib::TextShared)});

            for (auto id : ids) {
                if (arrInputBox[id].selected == true && !arrLobbyStatus.exist(id)) {
                    insertText(id, arrInputBox);
                }
            }
        }

        void manageInputBox(std::size_t, std::size_t)
        {
            std::size_t idEntity = 0;

            if (Raylib::MouseInput::isMouseButtonPressed(Raylib::MouseButton::MOUSE_BTN_LEFT)) {
                ::Menu::checkClick(idEntity);
                setInputBoxSelected(idEntity);
                return;
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
            constexpr std::size_t delay = 150;
            Registry::components<Types::InputBox> arrInputBox =
                Registry::getInstance().getComponents<Types::InputBox>();
            std::vector<std::size_t> ids = Registry::getInstance().getEntitiesByComponents(
                {typeid(Types::InputBox), typeid(Raylib::Text)});
            static auto clockId = Registry::getInstance().getClock().create();

            if (Raylib::KeyboardInput::isKeyDown(Raylib::KeyboardKey::KB_BACKSPACE)
                && Registry::getInstance().getClock().elapsedMillisecondsSince(clockId) > delay) {
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
            if (Raylib::MouseInput::isMouseButtonDown(Raylib::MouseButton::MOUSE_BTN_LEFT)) {
                arrAnimRect[id].changeRectList(Types::RectListType::SELECTED);
            }
            if (Raylib::MouseInput::isMouseButtonReleased(Raylib::MouseButton::MOUSE_BTN_LEFT)) {
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
            if (Scene::SceneManager::getInstance().getCurrentScene() != Scenes::MENU) {
                SystemManagersDirector::getInstance().getSystemManager(managerId).removeSystem(systemId);
                return;
            }
            try {
                Parallax::initParalax();
                nlohmann::json jsonData =
                    Json::getInstance().getDataByJsonType<nlohmann::json>("menu", JsonType::MENU);
                ::Menu::MenuBuilder::getInstance().initMenuSceneEntity(
                    Json::getInstance().getDatasFromList(jsonData));
            } catch (std::runtime_error &err) {
                Logger::info(err.what());
            }
            SystemManagersDirector::getInstance().getSystemManager(managerId).removeSystem(systemId);
        }

        void quitScene(std::size_t /*unused*/, std::size_t /*unused*/)
        {
            if (Raylib::KeyboardInput::isKeyPressed(Raylib::KeyboardKey::KB_ESCAPE)) {
                switch (Scene::SceneManager::getInstance().getCurrentScene()) {
                    case MENU: Scene::SceneManager::getInstance().stop(); break;
                    case CREATE_LOBBY_SCENE:
                        Scene::SceneManager::getInstance().changeScene(SELECT_LOBBY);
                        break;
                    case SELECT_LOBBY:
                        Scene::SceneManager::getInstance().changeScene(MENU);
                        break;
                    case GAME:
                        Scene::SceneManager::getInstance().changeScene(SELECT_LOBBY);
                        break;
                }
            }
        }

        // this system is not in the good file for the moment
        void initHud(std::size_t managerId, std::size_t systemId)
        {
            if (Scene::SceneManager::getInstance().getCurrentScene() != Scene::Scene::MAIN_GAME) {
                return;
            }
            try {
                nlohmann::json jsonData =
                    Json::getInstance().getDataByJsonType<nlohmann::json>("gameHud", JsonType::HUD);
                ::Menu::MenuBuilder::getInstance().initMenuSceneEntity(
                    Json::getInstance().getDatasFromList(jsonData));
            } catch (std::runtime_error &err) {
                Logger::info(err.what());
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
                checkInputDeletion,
                Systems::moveEntities,
                initHud,
                quitScene};
        }
    } // namespace Menu
} // namespace Systems
