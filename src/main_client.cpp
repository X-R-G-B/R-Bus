#include <iostream>
#include <string>
#include "ClientSystems.hpp"
#include "CustomTypes.hpp"
#include "Raylib.hpp"
#include "Registry.hpp"
#include "SystemManagersDirector.hpp"

void init()
{
    Registry& registry               = Registry::getInstance();
    const int initialEntityPositionX = 0;
    const int initialEntityPositionY = 0;
    const int collisionRectWidth     = 10;
    const int collisionRectHeight    = 20;
    const std::string spriteImagePath =
        "./assets/R-TypeSheet/r-typesheet18.gif";
    const int spriteWidth      = 10;
    const int spriteHeight     = 20;
    const float rectX          = 2.0F;
    const float rectY          = 5.0F;
    const float rectWidth      = 30.5F;
    const float rectHeight     = 25.2F;
    const Raylib::Vector2 textPosition = {50, 50};
    const float fontSize = 5.0F;
    const std::string fontPath = "assets/Fonts/soliden/SolidenTrial-Black.otf";
    const std::string soundEffectPath = "assets/Audio/Sounds/yes.ogg";
    const std::string musicStreamPath = "assets/Audio/Musics/Title.mp3";

    registry.addEntity();
    registry.getComponents<Types::Position>().back() = {
        initialEntityPositionX,
        initialEntityPositionY};
    registry.getComponents<Types::CollisionRect>().back() = {
        collisionRectWidth,
        collisionRectHeight};
    registry.getComponents<Raylib::Sprite>().back() = {
        spriteImagePath,
        spriteWidth,
        spriteHeight};
    registry.getComponents<Types::Rect>()
        .back() = {rectX, rectY, rectWidth, rectHeight};
    registry.getComponents<Types::Player>().back() = Types::Player(true);
    registry.addEntity();
    registry.getComponents<Types::Position>().back() = {
        initialEntityPositionX,
        initialEntityPositionY};
    registry.getComponents<Types::RectangleShape>().back() = {
        collisionRectWidth,
        collisionRectHeight};
    registry.addEntity();
    registry.getComponents<Raylib::Text>().back() = {
        std::string("Hello World!"),
        textPosition,
        fontSize,
        Raylib::Purple};
    registry.addEntity();
    registry.getComponents<Raylib::Sound>().back() =
        Raylib::Sound(soundEffectPath);
    registry.addEntity();
    registry.getComponents<Raylib::Music>().back() =
        Raylib::Music(musicStreamPath);
}

int main()
{
    const int screenWidth  = 800;
    const int screenHeight = 600;
    int monitor            = 0;
    int fps                = 0;

    Raylib::setConfigFlags(FLAG_WINDOW_RESIZABLE);

    Systems::SystemManagersDirector& director =
        Systems::SystemManagersDirector::getInstance();
    director.addSystemManager(Systems::EventsSystems::eventSystems);
    director.addSystemManager(Systems::GraphicSystems::graphicSystems);
    std::vector<std::size_t> managersIds = {1, 0, 2};

    Raylib::initWindow(
        screenWidth,
        screenHeight,
        "R-Type");
    monitor = Raylib::getCurrentMonitor();
    fps = Raylib::getMonitorRefreshRate(monitor);
    Raylib::setTargetFPS(fps);
    Raylib::initAudioDevice();
    init();
    while (!WindowShouldClose()) {
        Raylib::beginDrawing();
        Raylib::clearBackground(Raylib::Green);
        for (auto id : managersIds) {
            director.getSystemManager(id).updateSystems();
        }
        Raylib::endDrawing();
    }
    Raylib::closeAudioDevice();
    Raylib::closeWindow();
}
