/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Geometry
*/

#pragma once

#include <cstdint>
#include "raylib.h"

namespace Raylib {

    class Vector2 {
        public:
            Vector2(float x, float y);

            float X() const;
            float Y() const;

            void X(float x);
            void Y(float y);

        private:
            float _x;
            float _y;
    };

    class Vector3 {
        public:
            Vector3(float x, float y, float z);

            float X() const;
            float Y() const;
            float Z() const;

            void X(float x);
            void Y(float y);
            void Z(float z);

        private:
            float _x;
            float _y;
            float _z;
    };

    class Vector4 {
        public:
            Vector4(float x, float y, float z, float w);

            float X() const;
            float Y() const;
            float Z() const;
            float W() const;

            void X(float x);
            void Y(float y);
            void Z(float z);
            void W(float w);

        private:
            float _x;
            float _y;
            float _z;
            float _w;
    };

    class Rectangle {
        public:
            Rectangle(float x, float y, float width, float height);

            float X() const;
            float Y() const;
            float Width() const;
            float Height() const;

            void X(float x);
            void Y(float y);
            void Width(float width);
            void Height(float height);

        private:
            float _x;
            float _y;
            float _width;
            float _height;
    };

    class Color {
        public:
            Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

            uint8_t R() const;
            uint8_t G() const;
            uint8_t B() const;
            uint8_t A() const;

            void R(uint8_t r);
            void G(uint8_t g);
            void B(uint8_t b);
            void A(uint8_t a);

        private:
            uint8_t _r;
            uint8_t _g;
            uint8_t _b;
            uint8_t _a;
    };

    // COLOR CONSTANTS
    static const Color DarkGray   = DARKGRAY;
    static const Color Yellow     = YELLOW;
    static const Color Gold       = GOLD;
    static const Color Orange     = ORANGE;
    static const Color Pink       = PINK;
    static const Color Red        = RED;
    static const Color Maroon     = MAROON;
    static const Color Green      = GREEN;
    static const Color Lime       = LIME;
    static const Color DarkGreen  = DARKGREEN;
    static const Color SkyBlue    = SKYBLUE;
    static const Color Blue       = BLUE;
    static const Color DarkBlue   = DARKBLUE;
    static const Color Purple     = PURPLE;
    static const Color Violet     = VIOLET;
    static const Color DarkPurple = DARKPURPLE;
    static const Color Beige      = BEIGE;
    static const Color Brown      = BROWN;
    static const Color DarkBrown  = DARKBROWN;
    static const Color White      = WHITE;
    static const Color Black      = BLACK;
    static const Color Blank      = BLANK;
    static const Color Magenta    = MAGENTA;
    static const Color RayWhite   = RAYWHITE;
} // namespace Raylib
