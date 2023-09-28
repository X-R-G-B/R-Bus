/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Geometry
*/

#include "Geometry.hpp"

namespace Raylib {
    Vector2::Vector2(float x, float y) : _x(x), _y(y)
    {
    }

    float Vector2::X() const
    {
        return _x;
    }
    float Vector2::Y() const
    {
        return _y;
    }

    void Vector2::X(float x)
    {
        _x = x;
    }
    void Vector2::Y(float y)
    {
        _y = y;
    }

    Vector3::Vector3(float x, float y, float z) : _x(x), _y(y), _z(z)
    {
    }

    float Vector3::X() const
    {
        return _x;
    }
    float Vector3::Y() const
    {
        return _y;
    }
    float Vector3::Z() const
    {
        return _z;
    }

    void Vector3::X(float x)
    {
        _x = x;
    }
    void Vector3::Y(float y)
    {
        _y = y;
    }
    void Vector3::Z(float z)
    {
        _z = z;
    }

    Vector4::Vector4(float x, float y, float z, float w)
        : _x(x),
          _y(y),
          _z(z),
          _w(w)
    {
    }

    float Vector4::X() const
    {
        return _x;
    }
    float Vector4::Y() const
    {
        return _y;
    }
    float Vector4::Z() const
    {
        return _z;
    }
    float Vector4::W() const
    {
        return _w;
    }

    void Vector4::X(float x)
    {
        _x = x;
    }
    void Vector4::Y(float y)
    {
        _y = y;
    }
    void Vector4::Z(float z)
    {
        _z = z;
    }
    void Vector4::W(float w)
    {
        _w = w;
    }

    Rectangle::Rectangle(float x, float y, float width, float height)
        : _x(x),
          _y(y),
          _width(width),
          _height(height)
    {
    }

    float Rectangle::X() const
    {
        return _x;
    }
    float Rectangle::Y() const
    {
        return _y;
    }
    float Rectangle::Width() const
    {
        return _width;
    }
    float Rectangle::Height() const
    {
        return _height;
    }

    void Rectangle::X(float x)
    {
        _x = x;
    }
    void Rectangle::Y(float y)
    {
        _y = y;
    }
    void Rectangle::Width(float width)
    {
        _width = width;
    }
    void Rectangle::Height(float height)
    {
        _height = height;
    }

    Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
        : _r(r),
          _g(g),
          _b(b),
          _a(a)
    {
    }

    uint8_t Color::R() const
    {
        return _r;
    }
    uint8_t Color::G() const
    {
        return _g;
    }
    uint8_t Color::B() const
    {
        return _b;
    }
    uint8_t Color::A() const
    {
        return _a;
    }

    void Color::R(uint8_t r)
    {
        _r = r;
    }
    void Color::G(uint8_t g)
    {
        _g = g;
    }
    void Color::B(uint8_t b)
    {
        _b = b;
    }
    void Color::A(uint8_t a)
    {
        _a = a;
    }
} // namespace Raylib
