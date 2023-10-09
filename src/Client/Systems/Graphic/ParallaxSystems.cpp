#include <fstream>
#include "CustomTypes.hpp"
#include "ECSCustomTypes.hpp"
#include "Graphics.hpp"
#include "Registry.hpp"
#include "SystemManagersDirector.hpp"
#include "nlohmann/json.hpp"

namespace Systems::ParallaxSystems {
    static nlohmann::json openJsonData(const std::string &path)
    {
        std::ifstream fileData(path);
        std::ostringstream input;
        nlohmann::json jsonData = {};

        if (fileData.is_open()) {
            input << fileData.rdbuf();
            if (nlohmann::json::accept(input.str())) {
                jsonData = nlohmann::json::parse(input.str());
                return jsonData;
            }
        }
        Registry::getInstance().getLogger().error("Could not load json file. An error occured");
        return jsonData;
    }

    constexpr int maxOutParallaxLeft  = -100;
    constexpr int maxOutParallaxRight = 100;

    static void initParallaxEntity(
        nlohmann::json_abi_v3_11_2::basic_json<> &parallaxData,
        const float maxOffsideParallax = 0)
    {
        std::size_t id = Registry::getInstance().addEntity();

        Types::Velocity velocity = {Types::Velocity(parallaxData["velocity"])};
        Raylib::Sprite sprite =
            {parallaxData["spritePath"], parallaxData["width"], parallaxData["height"], id};
        Types::Position position = {Types::Position(parallaxData["position"])};

        Registry::getInstance().getComponents<Raylib::Sprite>().insertBack(sprite);
        Registry::getInstance().getComponents<Types::Position>().insertBack(position);
        Registry::getInstance().getComponents<Types::Velocity>().insertBack(velocity);
        if (parallaxData["rect"] != nullptr) {
            Types::Rect rect = {Types::Rect(parallaxData["rect"])};
            Registry::getInstance().getComponents<Types::Rect>().insertBack(rect);
        }
        Registry::getInstance().setToBackLayers(id);
        Types::Parallax paralax = {};
        Registry::getInstance().getComponents<Types::Parallax>().insertBack(paralax);
        Registry::components<Types::Position> arrPosition =
            Registry::getInstance().getComponents<Types::Position>();
        if (maxOffsideParallax > 0) {
            arrPosition[id].x += maxOffsideParallax;
        }
    }

    const std::string parallaxFile = "assets/Json/parallaxData.json";

    void initParalax(std::size_t managerId, std::size_t systemId)
    {
        nlohmann::json jsonData = openJsonData(parallaxFile);

        for (auto &e : jsonData["parallax"]) {
            initParallaxEntity(e);
            if (e["copy"] != nullptr && e["copy"]) {
                initParallaxEntity(e, maxOutParallaxRight);
            }
        }
        SystemManagersDirector::getInstance().getSystemManager(managerId).removeSystem(systemId);
    }

    static void resetParallaxPosition(
        std::size_t id,
        Registry::components<Types::Parallax> &arrParallax,
        Registry::components<Types::Position> &arrPosition)
    {
        if (arrPosition[id].x <= maxOutParallaxLeft) {
            if (arrParallax[id].x >= maxOutParallaxRight) {
                arrPosition[id].x = arrParallax[id].x;
            } else {
                arrPosition[id].x = arrParallax[id].x + maxOutParallaxRight;
            }
            arrPosition[id].y = arrParallax[id].y;
        }
    }

    void manageParallax(std::size_t /*unused*/, std::size_t /*unused*/)
    {
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
