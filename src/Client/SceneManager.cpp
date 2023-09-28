/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** SceneManager implementation
*/

#include "SceneManager.hpp"
#include "ClientSystems.hpp"
#include "Raylib.hpp"
#include "Registry.hpp"
#include "SystemManagersDirector.hpp"

// to suppr
#include "CustomTypes.hpp"

constexpr int screenWidth    = 800;
constexpr int screenHeight   = 600;
constexpr int playerData     = 10;
const std::string musicPath  = "assets/Audio/Musics/Title.mp3";
const std::string soundPath  = "assets/Audio/Sounds/fire.ogg";
const std::string playerPath = "assets/R-TypeSheet/r-typesheet14.gif";
const Types::Rect spriteRect = {2, 2, 48, 48};
const Types::CollisionRect collisionRect = {46, 46};
const Raylib::Vector2 textPos            = {20, 50};
constexpr float musicVolume              = 0.02F;
constexpr float soundVolume              = 0.1F;
constexpr float fontScale                = 2.0F;
const float playerWidth                  = 50.0F;
const float playerHeight                 = 50.0F;
const Raylib::Color darkBlue                   = {0, 0, 139, 255};
const Raylib::Color darkGray                   = {169, 169, 169, 255};

// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
bool SceneManager::_init             = false;
SceneManager SceneManager::_instance = SceneManager();
// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

// To remove
static void playSoundWithKey()
{
    Registry::components<Raylib::Music> arrMusics =
        Registry::getInstance().getComponents<Raylib::Music>();
    Registry::components<Raylib::Sound> arrSounds =
        Registry::getInstance().getComponents<Raylib::Sound>();

    for (auto &music : arrMusics) {
        if (!music.has_value()) {
            continue;
        }
        if (music.value().getPath() == musicPath
            && Raylib::isKeyPressed(Raylib::KeyboardKey::KB_SPACE)) {
            music.value().setNeedToPlay(true);
        }
    }
    for (auto &sound : arrSounds) {
        if (!sound.has_value()) {
            continue;
        }
        if (sound.value().getPath() == soundPath
            && Raylib::isKeyPressed(Raylib::KeyboardKey::KB_ENTER)) {
            sound.value().setNeedToPlay(true);
        }
    }
}
// To remove
static void addTestComponents()
{
    Registry::getInstance().addEntity();
    Registry::getInstance().getComponents<Types::Position>().back() = {
        playerData,
        playerData};
    Registry::getInstance().getComponents<Raylib::Sprite>().back() = {
        playerPath,
        playerWidth,
        playerHeight};
    Registry::getInstance().getComponents<Types::Rect>().back() = spriteRect;
    Registry::getInstance().getComponents<Types::CollisionRect>().back() =
        collisionRect;
    SparseArray<std::size_t> &playerId =
        Registry::getInstance().getCustomSparseArray<std::size_t>(
            CustomIndex::PLAYER);
    playerId.add();
    playerId.back() =
        std::optional<std::size_t>(Registry::getInstance().getEntitiesNb() - 1);
    Registry::getInstance().getComponents<Raylib::Music>().back() = {
        musicPath,
        musicVolume};
    Registry::getInstance().getComponents<Raylib::Sound>().back() = {
        soundPath,
        soundVolume};
    Registry::getInstance().getComponents<Raylib::Text>().back() = {
        "Press space to play music, enter to play sound",
        textPos,
        fontScale,
        darkBlue};
}

static void initRaylib()
{
    Raylib::initWindow(screenWidth, screenHeight, "R-Bus");
    Raylib::setWindowState(Raylib::ConfigFlags::WINDOW_RESIZABLE);
    Raylib::setTargetFPS(
        Raylib::getMonitorRefreshRate(Raylib::getCurrentMonitor()));
    Raylib::initAudioDevice();
}

static void initSystemManagers()
{
    auto &director = Systems::SystemManagersDirector::getInstance();

    for (auto systems : Systems::getSystemsGroups()) {
        director.addSystemManager(systems);
    }
    initRaylib();
    // to remove
    addTestComponents();
}

SceneManager &SceneManager::getInstance()
{
    if (!_init) {
        _init = true;
        initSystemManagers();
    }
    return _instance;
}

SceneManager::SceneManager() : _currentScene(Scene::MAIN_GAME), _stop(false)
{
}

static void destroyRaylib()
{
    Raylib::closeAudioDevice();
    Raylib::closeWindow();
}

int SceneManager::run()
{
    auto &director = Systems::SystemManagersDirector::getInstance();

    try {
        while (!_stop && !Raylib::windowShouldClose()) {
            Raylib::beginDrawing();
            Raylib::clearBackground(darkGray);
            auto scene = _scenes.at(_currentScene);
            // to remove
            playSoundWithKey();
            for (auto &systemManager : scene) {
                director.getSystemManager(systemManager).updateSystems();
            }
            Raylib::endDrawing();
        }
        destroyRaylib();
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return ReturnValue::ERROR;
    }
    return ReturnValue::OK;
}

void SceneManager::changeScene(Scene scene)
{
    _currentScene = scene;
    Registry::getInstance().clear();
}

void SceneManager::stop()
{
    _stop = true;
}
