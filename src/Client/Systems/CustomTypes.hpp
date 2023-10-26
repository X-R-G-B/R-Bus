/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Custom Types
*/

#pragma once

#include <string>
#include <vector>
#include "ECSCustomTypes.hpp"
#ifdef CLIENT
    #include "Raylib.hpp"
#endif

namespace Types {

    struct Parallax {
            float x;
            float y;
    };

    struct RectangleShape {
            float width;
            float height;
    };

    struct FontSize {
            float fsz;
    };

    struct Origin {
            float x;
            float y;
    };

    struct Rotation {
            float rotate;
    };

    struct Scale {
            float size;
    };

    struct Button {
        Button(std::function<void()> &fct) : callback(fct) {};
        std::function<void()> callback;
    };

    struct InputBox {
        InputBox(std::string &textStr, std::string &boxName,std::size_t &maxCharacters, bool isSelected = false) :  name(boxName), text(textStr), maxChar(maxCharacters), selected(isSelected){};
        std::string name;
        std::string text;
        std::size_t maxChar;
        bool selected;
    };

#ifdef CLIENT
    struct Color {
            Raylib::Color color;
    };

    static const std::unordered_map<std::string, Raylib::Color> colorMatchStrings({
        {"darkgray", Raylib::DarkGray},
        {"yellow", Raylib::Yellow},
        {"gold", Raylib::Gold},
        {"orange", Raylib::Orange},
        {"pink", Raylib::Pink},
        {"red", Raylib::Red},
        {"maroon", Raylib::Maroon},
        {"green", Raylib::Green},
        {"lime", Raylib::Lime},
        {"darkgreen", Raylib::DarkGreen},
        {"skyblue", Raylib::SkyBlue},
        {"blue", Raylib::Blue},
        {"darkblue", Raylib::DarkBlue},
        {"purple", Raylib::Purple},
        {"violet", Raylib::Violet},
        {"darkpurple", Raylib::DarkPurple},
        {"beige", Raylib::Beige},
        {"brown", Raylib::Brown},
        {"darkbrown", Raylib::DarkBrown},
        {"white", Raylib::White},
        {"black", Raylib::Black},
        {"blank", Raylib::Blank},
        {"magenta", Raylib::Magenta},
        {"raywhite", Raylib::RayWhite}
    });
#endif

} // namespace Types

#include "AnimRect.hpp"
