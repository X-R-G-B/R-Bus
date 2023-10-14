#include <fstream>
#include <sstream>
#include <string>
#include "CustomTypes.hpp"
#include "ECSCustomTypes.hpp"
#include "Graphics.hpp"
#include "Json.hpp"
#include "Logger.hpp"
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
            Json::getInstance().getDataFromJson(elem, "spritePath"),
            Json::getInstance().getDataFromJson(elem, "width"),
            Json::getInstance().getDataFromJson(elem, "height"),
            id};
        Types::Position position = {Types::Position(Json::getInstance().getDataFromJson(elem, "position"))};
        Types::Velocity velocity = {Types::Velocity(Json::getInstance().getDataFromJson(elem, "velocity"))};

        if (Json::getInstance().isDataExist(elem, "rect")) {
            Types::Rect rect = {Types::Rect(Json::getInstance().getDataFromJson(elem, "rect"))};
            Registry::getInstance().getComponents<Types::Rect>().insertBack((rect));
        }

        if (maxOffsideParallax > 0) {
            position.x =
                Maths::additionWithDecimal(position.x, Maths::addIntegerDecimals(maxOffsideParallax));
        }
        Types::Parallax paralax = {
            Maths::integrerToDecimalWithTwoDecimals(position.x),
            Maths::integrerToDecimalWithTwoDecimals(position.y)};

        Registry::getInstance().getComponents<Raylib::Sprite>().insertBack(parralax);
        Registry::getInstance().getComponents<Types::Position>().insertBack(position);
        Registry::getInstance().getComponents<Types::Velocity>().insertBack(velocity);
        Registry::getInstance().getComponents<Types::Parallax>().insertBack(paralax);
        Registry::getInstance().setToBackLayers(id);
    }

    void initParalax(std::size_t managerId, std::size_t systemId)
    {
        std::vector<nlohmann::basic_json<>> parallaxData =
            Json::getInstance().getDataByJsonType("parallax", JsonType::DEFAULT_PARALLAX);

        for (auto &elem : parallaxData) {
            initParallaxEntity(elem);
            if (Json::getInstance().isDataExist(elem, "copy")
                && Json::getInstance().getDataFromJson(elem, "copy") == true) {
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
        if (Maths::removeIntegerDecimals(arrPosition[id].x) <= maxOutParallaxLeft) {
            if (arrParallax[id].x >= maxOutParallaxRight) {
                arrPosition[id].x = Maths::decimalToIntegrerWithTwoDecimals(arrParallax[id].x);
            } else {
                arrPosition[id].x = Maths::decimalToIntegrerWithTwoDecimals(arrParallax[id].x)
                    + Maths::addIntegerDecimals(maxOutParallaxRight);
            }
            arrPosition[id].y = Maths::decimalToIntegrerWithTwoDecimals(arrParallax[id].y);
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
} // namespace Systems::ParallaxSystems
