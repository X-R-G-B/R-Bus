#pragma once

#include <fstream>
#include <string>
#include "GraphicsCustomTypes.hpp"
#include "Json.hpp"
#include "Maths.hpp"
#include "Registry.hpp"
#include "SystemManagersDirector.hpp"


namespace Systems::ParallaxSystems {

    constexpr int maxOutParallaxLeft  = -100;
    constexpr int maxOutParallaxRight = 100;

    static void initParallaxEntity(nlohmann::basic_json<> &elem, const int maxOffsideParallax = 0)
    {
        std::size_t id          = Registry::getInstance().addEntity();
        Raylib::Sprite parralax = {
            Json::getInstance().getDataFromJson<std::string>(elem, "spritePath"),
            Json::getInstance().getDataFromJson<float>(elem, "width"),
            Json::getInstance().getDataFromJson<float>(elem, "height"),
            id};

        Types::Position position = Json::getInstance().getDataFromJson<Types::Position>(elem, "position");
        Types::Velocity velocity = Json::getInstance().getDataFromJson<Types::Velocity>(elem, "velocity");

        if (Json::getInstance().isDataExist(elem, "rect")) {
            Types::Rect rect = Json::getInstance().getDataFromJson<Types::Rect>(elem, "rect");
            Registry::getInstance().getComponents<Types::Rect>().insertBack((rect));
        }

        if (maxOffsideParallax > 0) {
            Maths::addNormalIntToDecimalInt(position.x, maxOffsideParallax);
        }
        Types::Parallax inst = {
            Maths::intToFloatConservingDecimals(position.x),
            Maths::intToFloatConservingDecimals(position.y)};
        Types::NoRemoveOutside noRem;

        Registry::getInstance().getComponents<Types::NoRemoveOutside>().insertBack(noRem);
        Registry::getInstance().getComponents<Raylib::Sprite>().insertBack(parralax);
        Registry::getInstance().getComponents<Types::Position>().insertBack(position);
        Registry::getInstance().getComponents<Types::Velocity>().insertBack(velocity);
        Registry::getInstance().getComponents<Types::Parallax>().insertBack(inst);
        Registry::getInstance().setToBackLayers(id);
    }

    void initParalax(std::size_t managerId, std::size_t systemId)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        std::vector<nlohmann::basic_json<>> parallaxData =
            Json::getInstance().getDataByJsonType("parallax", JsonType::DEFAULT_PARALLAX);

        for (auto &elem : parallaxData) {
            initParallaxEntity(elem);
            if (Json::getInstance().isDataExist(elem, "copy")
                && Json::getInstance().getDataFromJson<bool>(elem, "copy") == true) {
                initParallaxEntity(elem, maxOutParallaxRight);
            }
        }
        SystemManagersDirector::getInstance().getSystemManager(managerId).removeSystem(systemId);
    }

    static void resetParallaxPosition(
        std::size_t id,
        Registry::components<Types::Parallax> &arrParallax,
        Registry::components<Types::Position> &arrPosition)
    {
        if (Maths::intToFloatConservingDecimals(arrPosition[id].x) <= maxOutParallaxLeft) {
            Maths::addNormalIntToDecimalInt(arrPosition[id].x, maxOutParallaxRight * 2);
            arrPosition[id].y = Maths::floatToIntConservingDecimals(arrParallax[id].y);
        }
    }

    void manageParallax(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        Registry::components<Types::Position> arrPosition =
            Registry::getInstance().getComponents<Types::Position>();
        Registry::components<Types::Parallax> arrParallax =
            Registry::getInstance().getComponents<Types::Parallax>();

        std::vector<std::size_t> ids = Registry::getInstance().getEntitiesByComponents(
            {typeid(Types::Position), typeid(Types::Parallax), typeid(Types::Velocity)});

        for (auto &id : ids) {
            resetParallaxPosition(id, arrParallax, arrPosition);
        }
    }


    std::vector<std::function<void(std::size_t, std::size_t)>> getParallaxSystems()
    {
        return {
            initParalax,
            manageParallax,
        };
    }
}
