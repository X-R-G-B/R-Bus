/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** TextSystems
*/

#pragma once

#include "GraphicsCustomTypes.hpp"
#include "Maths.hpp"
#include "Raylib.hpp"
#include "SharedValues.hpp"

namespace Systems {
    namespace GraphicsSystems {

        static void setFontSizeResponsive(Raylib::Text &text, Types::FontSize &fsz)
        {
            float fontSize = (fsz.fsz * static_cast<float>(GetScreenWidth())) / denominator;

            text.setCurrentFontSize(fontSize);
        }

        static void setPositionResponsive(Raylib::Text &text, Types::Position &pos)
        {
            float x = (Maths::intToFloatConservingDecimals(pos.x) * static_cast<float>(GetScreenWidth()))
                      / denominator;
            float y = (Maths::intToFloatConservingDecimals(pos.y) * static_cast<float>(GetScreenHeight()))
                      / denominator;

            text.setPixelPosition({x, y});
        }

        static void drawTextResponsive(std::size_t id, Raylib::Text &text)
        {
            Registry::components<Raylib::Color> arrColor =
                Registry::getInstance().getComponents<Raylib::Color>();
            Registry::components<Types::Position> arrPosition =
                Registry::getInstance().getComponents<Types::Position>();
            Registry::components<Types::FontSize> arrFsz =
                Registry::getInstance().getComponents<Types::FontSize>();

            Types::FontSize defaultFsz = {text.getFontSize()};
            setFontSizeResponsive(text, defaultFsz);
            if (arrFsz.exist(id)) {
                setFontSizeResponsive(text, arrFsz[id]);
            }

            Types::Position defaultPosition = {
                Maths::floatToIntConservingDecimals(text.getPosition().x),
                Maths::floatToIntConservingDecimals(text.getPosition().y)};
            setPositionResponsive(text, defaultPosition);
            if (arrPosition.exist(id)) {
                setPositionResponsive(text, arrPosition[id]);
            }
            text.setColor(text.getColor());
            if (arrColor.exist(id)) {
                text.setColor(arrColor[id]);
            }
            text.draw();
        }

        void textRenderer(std::size_t /*unused*/, std::size_t /*unused*/)
        {
            std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
            Registry::components<Raylib::Text> arrText = Registry::getInstance().getComponents<Raylib::Text>();

            std::vector<std::size_t> ids = arrText.getExistingsId();

            for (auto &id : ids) {
                drawTextResponsive(id, arrText[id]);
            }
        }

        std::vector<std::function<void(std::size_t, std::size_t)>> getTextSystems()
        {
            return {textRenderer};
        }
    } // namespace GraphicsSystems
} // namespace Systems
