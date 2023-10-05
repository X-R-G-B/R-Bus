/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** TextSystems implementation
*/

#include "TextSystems.hpp"
#include "Raylib.hpp"
#include "SharedValues.hpp"

namespace Systems {

    static void drawTextResponsive(Raylib::Text &text)
    {
        float x   = (text.x() * static_cast<float>(GetScreenWidth())) / denominator;
        float y   = (text.y() * static_cast<float>(GetScreenHeight())) / denominator;
        float fsz = (text.getFontSize() * static_cast<float>(GetScreenWidth())) / denominator;

        text.setPixelPosition({x, y});
        text.setCurrentFontSize(fsz);
        text.draw();
    }

    void GraphicSystems::textRenderer(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        Registry::components<Raylib::Text> arrText = Registry::getInstance().getComponents<Raylib::Text>();

        for (auto &textIt : arrText) {
            drawTextResponsive(textIt);
        }
    }

    std::vector<std::function<void(std::size_t, std::size_t)>> GraphicSystems::getTextSystems()
    {
        return {textRenderer};
    }
} // namespace Systems
